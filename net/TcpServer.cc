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

void TcpServer::start()
{
	if (started_.getAndSet(1) == 0)
	{
		this->threadPool_->start(threadInitCallback_);

		assert(!acceptor_->listenning());
		loop_->runInLoop(
			std::bind(&Acceptor::listen, get_pointer(acceptor_)));
	}
}


/**
 * connector 将监听端口的 socket 添加到了 epoll 的队列当中，每当 socket 有事件到达的时候，就意味着
 * 有新的连接到达了，Connector::handleRead() 是相应的回调函数，回调函数中通过调用 newConnection() 
 * 将这个新的连接通报给 TcpServer
*/

/**
 * 每一个新的连接，不是在当前的 loop 中进行处理，而是用 eventloopthreadpool 当中每一个子线程中的 loop 
 * 来进行处理，TcpServer 的 loop 只负责 Acceptor 的处理工作
 * 但是这和一个连接一个线程的处理方式不同，每一个子线程中的 loop 可以同时处理多个连接
*/
void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
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
	connections_[connName] = conn;
	/**
	 * 一个 Tcp 服务端需要管理多个 Tcp数据连接。使用 map 管理这些连接可以快速进行查找
	*/
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
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
