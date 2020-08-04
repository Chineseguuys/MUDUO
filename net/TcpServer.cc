#include "net/TcpServer.h"

#include "base/Logging.h"
#include "net/Acceptor.h"
#include "net/EventLoop.h"
#include "net/EventLoopThreadPool.h"
#include "net/SocketsOps.h"

#include <stdio.h>  // snprintf

using namespace muduo;
using namespace muduo::net;


TcpServer::TcpServer(EventLoop* loop,
					 const InetAddress& listenAddr,
					 const string& nameArg,
					 Option option)
  : loop_(CHECK_NOTNULL(loop)),
	ipPort_(listenAddr.toIpPort()),
	name_(nameArg),
	acceptor_(new Acceptor(loop, listenAddr, option == kReusePort)),
	threadPool_(new EventLoopThreadPool(loop, name_)),
	connectionCallback_(defaultConnectionCallback),
	messageCallback_(defaultMessageCallback),
	nextConnId_(1)
{
	acceptor_->setNewConnectionCallback(
		std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
	loop_->assertInLoopThread();
	LOG_TRACE << "TcpServer::~TcpServer [" << name_ << "] destructing";

	for (auto& item : connections_)
	{
		TcpConnectionPtr conn(item.second);
		item.second.reset();
		conn->getLoop()->runInLoop(
		std::bind(&TcpConnection::connectDestroyed, conn));
	}
}


/**
 * 要使用多少个 子线程（每一个线程一个 loop 负责 io 事件） 来处理所有的 TCP 的 IO 事件
*/
void TcpServer::setThreadNum(int numThreads)
{
	assert(0 <= numThreads);
	threadPool_->setThreadNum(numThreads);
}

/**
 * 这个函数执行后， numThreads 个 ioloop 就开始事件循环
*/
void TcpServer::start()
{
	if (started_.getAndSet(1) == 0) /**started_ 的操作是原子的，不需要进行上锁，多线程安全*/
	{
		this->threadPool_->start(threadInitCallback_);

		assert(!acceptor_->listenning());
		/**
		 * 接收器开始工作，在 loop_ 的下一次循环当中，就会去执行下面的函数（之后执行一次）。后面的工作交给
		 * 已经注册在 loop_->poller 下面的 socketChannel_ 进行连接到达的处理
		 * 
		 * 通过 poll 的轮询查看是否有连接达到
		*/
		loop_->runInLoop(
			std::bind(&Acceptor::listen, get_pointer(acceptor_)));
	}
}


/**
 * accepter 将监听端口的 socket 添加到了 epoll 的队列当中，每当 socket 有事件到达的时候，就意味着
 * 有新的连接到达了，Accepter::handleRead() 是相应的回调函数，回调函数中通过调用 newConnection() 
 * 将这个新的连接通报给 TcpServer
*/

/**
 * 每一个新的连接，不是在当前的 loop 中进行处理，而是用 eventloopthreadpool 当中每一个子线程中的 loop 
 * 来进行处理，TcpServer 的 loop 只负责 Acceptor 的处理工作
 * 但是这和一个连接一个线程的处理方式不同，每一个子线程中的 loop 可以同时处理多个连接
*/
void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
	/**
	 * sockfd 返回一对 Tcp 连接的文件描述符
	*/
	this->loop_->assertInLoopThread();
	EventLoop* ioloop = this->threadPool_->getNextLoop();
	char buf[64];
  	snprintf(buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_);
  	++nextConnId_;
  	string connName = name_ + buf;

	LOG_INFO << "TcpServer::newConnection [" << name_
           << "] - new connection [" << connName
           << "] from " << peerAddr.toIpPort();

	InetAddress localAddr(sockets::getLocalAddr(sockfd));

	TcpConnectionPtr conn(new TcpConnection(
		ioloop,
		connName,
		sockfd,
		localAddr,
		peerAddr));
	connections_[connName] = conn; /**新的连接添加到 map 当中*/
	/**
	 * 一个 Tcp 服务端需要管理多个 Tcp 数据连接。使用 map 管理这些连接可以快速进行查找
	*/
	conn->setConnectionCallback(connectionCallback_);
	/**
	 * 连接的状态发生了变化，使用这个回调通知用户程序
	*/
	conn->setMessageCallback(messageCallback_);
	/**
	 * 有数据到达，使用这个回调来通知上层程序
	*/
	conn->setWriteCompleteCallback(writeCompleteCallback_);
	/**
	 * 如果一个数据链接需要关闭的时候，自然其要从 TcpServer 当中删除
	*/
	conn->setCloseCallback(
      std::bind(&TcpServer::removeConnection, this, _1)); // FIXME: unsafe
	ioloop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

/**
 * 一个 Tcp 连接传输完毕，需要进行的处理
*/
void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	this->loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}


void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	this->loop_->assertInLoopThread();
  	LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_
           << "] - connection " << conn->name();
	/**
	 * 将这个连接从 map 当中移除
	*/
	size_t n = this->connections_.erase(conn->name());
	(void)n;
	assert(n == 1);
	EventLoop* ioloop = conn->getLoop();
	ioloop->queueInLoop(
		std::bind(&TcpConnection::connectDestroyed, conn)
	);
}
