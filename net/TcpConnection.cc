#include "net/TcpConnection.h"

#include "base/Logging.h"
#include "base/WeakCallback.h"
#include "net/Channel.h"
#include "net/EventLoop.h"
#include "net/Socket.h"
#include "net/SocketsOps.h"

#include <errno.h>

using namespace muduo;
using namespace muduo::net;




/**
 * 默认的连接回调是打印当前的连接状态
 * 每当 TcpConnection 的状态发生了变化的时候，都会通过回调来通知上层的模块，上层的模块根据状态的变化可以进行
 * 相应的处理
 * 如果上层不打算进行处理的话，默认的处理方式就是打印状态
*/
void muduo::net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{
	LOG_TRACE << conn->localAddress().toIpPort() << " -> "
			<< conn->peerAddress().toIpPort() << " is "
			<< (conn->connected() ? "UP" : "DOWN");
}

/**
 * 为什么默认的设置为这个？
 * 在完成了数据的读取之后，TCP 的上一层应该及时的将缓冲区的数据取走
 * 默认情况下，就是直接丢弃数据，不做任何的处理
 * 
 * 作为上层的模块，不存在不取 TcpConnection 当中的数据或者只取部分数据，一定是全部取出来
 * 进行处理，如果上层不打算取走数据，那么全部进行舍弃
*/
void muduo::net::defaultMessageCallback(const TcpConnectionPtr&,
										Buffer* buf,
										Timestamp)
{
	buf->retrieveAll();
	/**
	 * 舍弃 buf 缓冲区中的所有的数据，read_index, write_index 到初始位置
	*/
}

TcpConnection::TcpConnection(EventLoop* loop,
							 const string& nameArg,
							 int sockfd,
							 const InetAddress& localAddr,
							 const InetAddress& peerAddr)
	: loop_(CHECK_NOTNULL(loop)), /*TcpConnection 属于一个 eventloop*/
	  name_(nameArg),
	  state_(KConnecting), /*连接已经在 Connector 当中完成了建立*/
	  reading_(true),
	  socket_(new Socket(sockfd)),	/**完成操作系统层面的 套接字的连接过程*/
	  channel_(new Channel(loop, sockfd)),
	  localAddr_(localAddr),
	  peerAddr_(peerAddr),
	  highWaterMark_(64*1024*1024)  // 64 MB
/**
 * channel 并不知道自己处理的是什么事件，以及如何处理这些事件。各种可能的事件都会绑定到 channel 上面，
 * Tcp socket 的读写的操作，普通文件的读写的操作，定时器的相关的操作。那么 channel 处理这些事件的方式就是通过
 * 回调函数来进行实现 
 * 在这里，如果 channel 负责的是 Tcp 连接相关的事件，那么 TcpConnection  的相关函数就被注册为 channel 的
 * 回调函数
*/
{
	this->channel_->setReadCallback(
		std::bind(&TcpConnection::handleRead, this, _1) /**函数有一个参数，所以使用一个占位符号*/
	);
	this->channel_->setWriteCallback(
		std::bind(&TcpConnection::handleWrite, this)
	);
	this->channel_->setCloseCallback(
		std::bind(&TcpConnection::handleClose, this)
	);
	this->channel_->setErrorCallback(
		std::bind(&TcpConnection::handleError, this)
	);

	LOG_DEBUG << "TcpConnection::ctor[" <<  this->name_ << "] at " << this
			<< " fd=" << sockfd;
	socket_->setKeepAlive(true);
	/**
	 * 使用操作系统自带的保活机制，即每隔 2小时发送一个确认连接的包，另一端在接收到这个包的情况下
	 * 必须即使的返回 ack，否则的话，没有及时返回（定时器超时）那么，发送端会尝试多次发送确认连接的
	 * 包（一般为 9 次）。
	 * 如果另一端的机器遇到了故障重新启动，那么其会发送 RST 报文，表示自己重新启动。那么原端会断开这个连接，尝试重新进行握手过程
	*/
}

TcpConnection::~TcpConnection()
{
	LOG_DEBUG << "TcpConnection::dtor[" <<  name_ << "] at " << this
				<< " fd=" << channel_->fd()
				<< " state=" << stateToString();
	assert(state_ == KDisconnected);
}

bool TcpConnection::getTcpInfo(struct tcp_info* tcpi) const
{
	return socket_->getTcpInfo(tcpi);
}

string TcpConnection::getTcpInfoString() const
{
	char buf[1024];
	buf[0] = '\0';
	socket_->getTcpInfoString(buf, sizeof buf);
	return buf;
}

/**
 * 发送数据
*/
void TcpConnection::send(const void* data, int len)
{
	this->send(StringPiece(static_cast<const char*>(data), len));
}

void TcpConnection::send(const StringPiece& message)
{
	if (this->state_ == KConnected)
	{
		if (this->loop_->isInLoopThread())
		{
			this->sendInLoop(message);
		}
		else /**不在当前的 loop 的线程当中，把数据和发送函数压入到 loop 的执行队列当中*/
		{
			void (TcpConnection::*fp)(const StringPiece& message) = &TcpConnection::sendInLoop; /*函数指针*/
			this->loop_->runInLoop(
				std::bind(fp, this, message.as_string())
			);
		}
	}
}

/**
 * Buffer 是一种特殊的缓存机构，即可以读，也可以写
 * 非线程安全
*/
void TcpConnection::send(Buffer* buf)
{
	if (this->state_ == KConnected)
	{
		if (this->loop_->isInLoopThread())
		{
			this->sendInLoop(buf->peek(), buf->readableBytes());
			buf->retrieveAll();
		}
		else 
		{
			void (TcpConnection::*fp)(const StringPiece& ) = &TcpConnection::sendInLoop;
			this->loop_->runInLoop(
				std::bind(fp, this, buf->retrieveAllAsString())
			);
		}
	}
	/**
	 * 不需要担心没有发送成功的影响，因为 Buffer 中根据情况会自动进行调整
	 * 没有发送成功，Buffer 中的 index 不会发生变化
	*/
}

void TcpConnection::sendInLoop(const StringPiece& message)
{
	this->sendInLoop(message.data(), message.size());
}

/*回调*/
void TcpConnection::sendInLoop(const void* data, size_t len)
{
	this->loop_->assertInLoopThread();
	ssize_t nwrote = 0;
	size_t remaining = len;
	bool faultError = false;
	if (state_ == KDisconnected)
	{
		LOG_WARN << "disconnected, give up writing";
		return;
	}

	if (!this->channel_->isWriting() && outputBuffer_.readableBytes() == 0)
	{
		/**
		 * 如果 this->channel_->isWriting() == true 这个分支不会执行 ------- channel 可写
		 * 如果 this->channel_->isWriting() == false 那么这个分支将会被执行------ channel 和 epoll 没有监听相应的事件
		 * 发送缓冲区中的内容全部都发送完了，我们就直接绕过缓冲区，直接向 socket 发送我们的数据
		*/
		nwrote = sockets::write(channel_->fd(), data, len);
		if (nwrote >= 0) /**发送了部分的数据，可能全部发送完，也可能只发送了一部分*/
		{
			remaining = len - nwrote;
			/**
			 * 所有的数据全部写入到 socket 的缓冲区
			*/
			if (remaining == 0 && this->writeCompleteCallback_)
				this->loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
			/**
			 * 如果只有部分的数据写入到  socket 缓冲区，那么剩下的数据应该怎么办
			*/
		}
		else 
		{
			nwrote = 0;
			if (errno != EWOULDBLOCK)
			{
				LOG_SYSERR << "TcpConnection::sendInLoop";
				if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
				{
					faultError = true;
				}
			}            
		}
	}

	/**
	 * 有数据没有发送完
	 * 或者 channel 可写，内容写入缓冲区
	*/
	assert(remaining <= len);
	if (!faultError && remaining > 0)
	{
		size_t oldLen = outputBuffer_.readableBytes();  /**输出缓冲区还有数据的话，有多少数据*/
		/**
		 * oldLen 是当前的缓冲区中还有的数据，remaining 是当前发送端还需要发送的数据
		*/
		if (oldLen + remaining >= highWaterMark_ &&
			oldLen < highWaterMark_ &&
			highWaterMarkCallback_)
			this->loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
		
		this->outputBuffer_.append(static_cast<const char*>(data) + nwrote, remaining);
		/**
		 * 没有完全写入 socket 的数据，先写入缓冲区
		*/

		if (!channel_->isWriting())
		/**
		 * 如果 channel 被设置为不可写，那么将其设置为可写
		 * 如果不设置为 可写，那么在 epoll 事件触发的时候无法 channel 无法处理可写事件
		*/
			channel_->enableWriting();
	}
}

void TcpConnection::shutdown()
{
	if (this->state_ == KConnected)
	{
		this->setState(KDisconnecting); /*准备关闭状态*/
		loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
	}
}

/**
 * 为什么要求这些函数要在 loop_ 的线程中，主要原因就是，这些函数不是直接进行调用的，而都是以回调的形式
 * 在 loop_ 当中被调用，所以没有错误的情况下，这些函数自然是在 loop_ 的线程下进行处理的
*/
void TcpConnection::shutdownInLoop()
{
	this->loop_->assertInLoopThread();
	LOG_TRACE << "channel_->isWritting() == true";
	if (!channel_->isWriting())
	/**
	 * 缓冲区内有数据的话，那么无法进行 shutdowndown()
	 * 并且 shutdown 只关闭写端不关闭读端
	*/
	{
		LOG_TRACE << "channel_->isWriting() == false";
		socket_->shutdownWrite();
	}
}

void TcpConnection::forceClose()
{
	// FIXME: use compare and swap
	if (state_ == KConnected || state_ == KDisconnecting)
	{
		setState(KDisconnecting);
		loop_->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
	}
}

/**
 * 延时一段时间之后再关闭
*/
void TcpConnection::forceCloseWithDelay(double seconds)
{
	if (state_ == KConnected || state_ == KDisconnecting)
	{
		setState(KDisconnecting);
		loop_->runAfter(
				seconds,
				makeWeakCallback(shared_from_this(), &TcpConnection::forceClose)	// 存在编译上的问题
		);
	}
}

void TcpConnection::forceCloseInLoop()
{
	this->loop_->assertInLoopThread();
	if (this->state_ == KConnected || this->state_ == KDisconnecting)
	{
		this->handleClose();
	}
}

const char* TcpConnection::stateToString() const
{
	switch (state_)
	{
		case KDisconnected:
		return "kDisconnected";
		case KConnecting:
		return "kConnecting";
		case KConnected:
		return "kConnected";
		case KDisconnecting:
		return "kDisconnecting";
		default:
		return "unknown state";
	}
}

/**
 * Socket编程中，TCP_NODELAY选项是用来控制是否开启Nagle算法，该算法是为了提高较慢的广域网传输效率，减小小分组的报文个数，完整描述：
 * 该算法要求一个TCP连接上最多只能有一个未被确认的小分组，在该小分组的确认到来之前，不能发送其他小分组。
 * 在 Telnet 客户端中，键盘中每敲击一个键盘的值，就会发送一个字节的数据包给服务器端，这导致了一个 TCP 数据报的大部分都是报头部分。
 * 这导致了很大的浪费。如果信道很拥挤的话，那么频繁的小报文会导致拥堵。
 * Nagle 算法就是发送一个小报文后，在收到这个报文的 ack 之前，将即将发送的数据缓存起来在然会一起发送。
*/
void TcpConnection::setTcpNoDelay(bool on)
{
  	socket_->setTcpNoDelay(on);
}

void TcpConnection::startRead()
{
  	loop_->runInLoop(std::bind(&TcpConnection::startReadInLoop, this));
}

void TcpConnection::startReadInLoop()
{
	loop_->assertInLoopThread();
	if (!reading_ || !channel_->isReading())
	{
		channel_->enableReading();
		reading_ = true;
	}
}

void TcpConnection::stopRead()
{
  loop_->runInLoop(std::bind(&TcpConnection::stopReadInLoop, this));
}

void TcpConnection::stopReadInLoop()
{
	loop_->assertInLoopThread();
	if (reading_ || channel_->isReading())
	{
		channel_->disableReading();
		reading_ = false;
	}
}

/**
 * 完成连接的过程
*/
void TcpConnection::connectEstablished()
{
	loop_->assertInLoopThread();
	assert(state_ == KConnecting);
	setState(KConnected);
	channel_->tie(shared_from_this());
	channel_->enableReading();   /**数据达到时 poll 将会检测到*/

	connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
	this->loop_->assertInLoopThread();
	if (this->state_ == KConnected)
	{
		/**
		 * 从连接状态转换为断开状态。 epoll 不再检测这个连接 socket 任何的事件
		*/
		this->setState(KDisconnected);
		channel_->disableAll();

		connectionCallback_(shared_from_this());
	}
	channel_->remove();
}


void TcpConnection::handleRead(Timestamp receiveTime)
{
	this->loop_->assertInLoopThread();
	int savedErrno = 0;
	ssize_t n = this->inputBuffer_.readfd(channel_->fd(), &savedErrno); /**将socket 中的数据全部读取出来*/
	/*非阻塞*/

	if (n > 0)
	{
		/**
		 * 立刻通知高层的模块，应该取走这些接收到的数据
		*/
		this->messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
	}
	else if (n == 0)
		/**
		 * 没有读到任何的数据
		 * 有两种可能的情况，第一种是另一端使用了 close() 关闭了读端和写端。这个时候，我们发送任何的数据都是没有意义的
		 * 所以关闭这个 TcpConnection 禁止所有的读写的操作
		*/
		handleClose(); /*为什么？*/
	else 
	{
		errno = savedErrno;
		LOG_SYSERR << "TcpConnection::handleRead";
		handleError();
	}
}

void TcpConnection::handleWrite()
{
	this->loop_->assertInLoopThread();
	if (channel_->isWriting())
	{
		ssize_t n = sockets::write(	channel_->fd(),
									this->outputBuffer_.peek(),
									this->outputBuffer_.readableBytes());
		if (n > 0)	/**实际写入的字节的数量*/
		{
			this->outputBuffer_.retrieve(n);
			if (outputBuffer_.readableBytes() == 0) /**输出 buffer 当中已经没有数据可以发送了*/
			{
				/**
				 * 输出缓冲区空了，需要提醒上层的模块，需要向 buffer 里面输出数据了
				*/
				this->channel_->disableWriting();
				/**
				 * 将 channel 设置为 disableWriting() 那么下一次 send 数据可以直接向 socket 进行发送
				 * 但是这个操作需要 epoll 进行设置
				 * 原因也很简单，那就是如果缓冲区没有了数据，下一次可写事件被 epoll 触发之后，也没有数据可以发送
				 * 还不如在下一次循环当中不监测这个事件的可写状态，等待主动 send() 数据的时候直接写 socket 
				*/
				if (writeCompleteCallback_)
				{
					this->loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
				}
				if (this->state_ == KDisconnecting)	/**如果正在断开连接，关闭写*/
				{
					shutdownInLoop();
				}
			}
		}
		else	/*写失败了*/
		{
			LOG_SYSERR << "TcpConnection::handleWrite";
		}
	}
	else 	/* channel 不可写*/
	{	
		LOG_TRACE << "Connection fd = " << channel_->fd()
              << " is down, no more writing";
	}
}

/**
 * loop_ 调用回调
*/
void TcpConnection::handleClose()
{
	this->loop_->assertInLoopThread();
	LOG_TRACE << "fd = " << channel_->fd() << " state = " << stateToString();
	assert(state_ == KConnected || state_ == KDisconnecting);

	setState(KDisconnected);
	channel_->disableAll(); /*不再监听这个 sockfd 的任何的事件*/
	/**
	 * 事件自然也从 epoll 监听队列当中被移除
	*/

	TcpConnectionPtr guardThis(shared_from_this());
	connectionCallback_(guardThis);	/**更高层的模块传递过来的一个回调函数*/
	// 这个函数一定是最后调用
	closeCallback_(guardThis);
}

void TcpConnection::handleError()
{
	int err = sockets::getSocketError(channel_->fd());
	LOG_ERROR << "TcpConnection::handleError [" << name_
				<< "] - SO_ERROR = " << err << " " << strerror_tl(err);
}