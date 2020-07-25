#include "net/Connector.h"
#include "base/Logging.h"
#include "net/Channel.h"
#include "net/EventLoop.h"
#include "net/SocketsOps.h"


#include <errno.h>

using namespace muduo;
using namespace muduo::net;


const int Connector::kMaxRetryDelayMs; /*最大的重连延时为 30s */

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
	:   loop_(loop),
		serverAddr_(serverAddr),
		state_(KDisconnected),
		connect_(false),
		retryDelayMs_(kInitRetryDelayMs)
{
	LOG_DEBUG << "ctor[" << this << "]";
}

Connector::~Connector()
{
	LOG_DEBUG << "dtor[" << this << "]";
	assert(!channel_);
}

/**
 * 对外使用的接口
*/
void Connector::start()
{
	connect_ = true;
	loop_->runInLoop(std::bind(&Connector::startInLoop, this));
}

/**
 * 对外使用的接口,注意，这个函数的作用是终止连接的过程，而不是断开一个已经存在的连接
*/
void Connector::stop()
{
	connect_ = false;	/*表示不再执行连接的任务，在 retry() 函数中进行使用*/
	loop_->queueInLoop(std::bind(&Connector::stopInLoop, this)); // FIXME: unsafe
}

void Connector::startInLoop()
{
	this->loop_->assertInLoopThread();
	assert(this->state_ == KDisconnected);
	if (this->connect_)
		this->connect();
	else 
		LOG_DEBUG << "not connect";
}

void Connector::stopInLoop()
{
	loop_->assertInLoopThread();
	if (state_ == KConnecting)
	{
		setState(KDisconnected);
		int sockfd = removeAndResetChannel();
		retry(sockfd);
	}
}

void Connector::connect()
{
	int sockfd = sockets::createNonblockingOrDie(this->serverAddr_.family());
	int ret = sockets::connect(sockfd, this->serverAddr_.getSockAddr());
	/**
	 * 由于我们采用的是非阻塞形式的连接方式，所以connect 函数的返回一般不会直接返回 
	 * 正确的结果，最大的可能性是处在正在连接过程中，或者被中断打断的状态。也有可能出现错误
	*/
	int savedErrno = (ret == 0) ? 0 : errno;
	switch(savedErrno){
		case 0 :
		case EINPROGRESS :	/*正在进行连接的处理*/
		case EINTR :	/*当前的连接的过程被系统调用打断*/
		case EISCONN :	/*参数sockfd的socket已是连线状态*/
			this->connecting(sockfd);
			break;

		case EAGAIN: /*重新进行尝试*/
		case EADDRINUSE: /*地址已经被使用*/
		case EADDRNOTAVAIL: /*如果没有端口可用，返回这个错误*/
		case ECONNREFUSED: /*连接被拒绝*/
		case ENETUNREACH:	/*网络不可达*/
			retry(sockfd);
			break;

		case EACCES: /*没有权限*/
		case EPERM:	/*操作没有被允许*/
		case EAFNOSUPPORT:	/*地址家族没有被协议支持*/
		case EALREADY:	/*操作已经在进行中*/
		case EBADF:	/*错误的文件描述符*/
		case EFAULT:	/*错误的地址*/
		case ENOTSOCK: /*在非 socket 上面执行 socket 操作*/
			LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
			sockets::close(sockfd);
			break;
		
		default :
			LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
			sockets::close(sockfd);
			// connectErrorCallback_();
			break;
	}
}

/**
 * 对外使用的接口
*/
void Connector::restart()
{
	loop_->assertInLoopThread();
	setState(KDisconnected);
	retryDelayMs_ = kInitRetryDelayMs;
	connect_ = true;
	startInLoop();
}


void Connector::connecting(int sockfd)
{
	/**
	 * 如果正在连接中的话，那么我们不希望当前的线程阻塞在这个地方，等待连接的完成
	 * 所以我们选择将 socket 使用 epoll 来进行管理 。使用一个 channel 来调用相应的
	 * 回调来完成连接的后续的处理
	*/
	this->setState(KConnecting);
	assert(!this->channel_); /*这个 channel 要保证还没有被注册为回调*/
	this->channel_.reset(new Channel(loop_, sockfd));
	this->channel_->setWriteCallback(
		std::bind(&Connector::handleWrite, this)
	);
	this->channel_->setErrorCallback(
		std::bind(&Connector::handleError, this)
	);

	this->channel_->enableWriting();
}

int Connector::removeAndResetChannel()
{
	channel_->disableAll();
	channel_->remove();
	int sockfd = channel_->fd();
	// Can't reset channel_ here, because we are inside Channel::handleEvent
	loop_->queueInLoop(std::bind(&Connector::resetChannel, this)); // FIXME: unsafe
	return sockfd;
}

void Connector::resetChannel()
{
	channel_.reset();
}


/**
 * 这个 channel 加入到 epoll 当中后，sockfd 触发之后，调用了 channel 的回调函数
 * 进行相应的处理，那么这个 sockfd 事件和相应的 channel 应该从 epoll 当中移除，
 * 防止反复的触发
*/
void Connector::handleWrite()
{
	LOG_TRACE << "Connector::handleWrite " << state_;

	if (this->state_ == KConnecting)	/**正在连接当中，还没有完成连接*/
	{
		int sockfd = removeAndResetChannel(); /*连接已经完成， 这个 channel 已经没有必要继续监控了*/
		int err = sockets::getSocketError(sockfd);
		if (err)
		{
			LOG_WARN << "Connector::handleWrite - SO_ERROR = "
					<< err << " " << strerror_tl(err);
			retry(sockfd);
		}
		else if (sockets::isSelfConnect(sockfd))
		{
			LOG_WARN << "Connector::handleWrite - Self connect";
      		retry(sockfd);
		}
		else 
		{
			this->setState(KConnected);
			if (connect_)
				this->newConnectionCallback_(sockfd);
			else 
				sockets::close(sockfd);
		}
	}
	else 
		assert(this->state_ == KDisconnected);
}

void Connector::handleError()
{
	LOG_ERROR << "Connector::handleError state=" << state_;
	if (state_ == KConnecting)
	{
		int sockfd = removeAndResetChannel();
		int err = sockets::getSocketError(sockfd);
		LOG_TRACE << "SO_ERROR = " << err << " " << strerror_tl(err);
		retry(sockfd);
	}
}


void Connector::retry(int sockfd)
{
	sockets::close(sockfd);	
	/**注意在这里关闭 socket,否则多次 retry 会导致生成了大量的没有用的 socket*/
	this->setState(KDisconnected);
	if (this->connect_)
	{
		LOG_INFO << "Connector::retry - Retry connecting to " << serverAddr_.toIpPort()
             << " in " << retryDelayMs_ << " milliseconds. ";
		this->loop_->runAfter(retryDelayMs_ / 1000.0,
				std::bind(&Connector::startInLoop, shared_from_this()));
		this->retryDelayMs_ = std::min( this->retryDelayMs_ * 2, kMaxRetryDelayMs);
		/**每次连接失败，重试的延时都加倍*/
		/**
		 * channel 是被反复进行使用的，每一次 channel.fd() 在 poll 当中触发之后，调用相应的回调函数的时候就会从 
		 * poll 当中被删除，每一次重试的时候，又会被再次加入到 poll 当中去
		*/
	}
	else 
		LOG_DEBUG << "Do Not Connect";
}