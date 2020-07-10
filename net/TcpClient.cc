#include "net/TcpClient.h"

#include "base/Logging.h"
#include "net/Connector.h"
#include "net/EventLoop.h"
#include "net/SocketsOps.h"


#include <stdio.h>  // snprintf

using namespace muduo;
using namespace muduo::net;


namespace muduo
{
namespace net
{
namespace detail
{

void removeConnection(EventLoop* loop, const TcpConnectionPtr& conn)
{
	loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

void removeConnector(const ConnectorPtr& connector)
{
  //connector->
}

}  // namespace detail
}  // namespace net
}  // namespace muduo


TcpClient::TcpClient(EventLoop* loop,
					 const InetAddress& serverAddr,
					 const string& nameArg)
  : loop_(CHECK_NOTNULL(loop)),
	connector_(new Connector(loop, serverAddr)),/**使用 shared_ptr 进行管理，退出的时候自动进行析构*/
	name_(nameArg),
	connectionCallback_(defaultConnectionCallback),
	messageCallback_(defaultMessageCallback),
	retry_(false),
	connect_(true),
	nextConnId_(1)
{
	connector_->setNewConnectionCallback(
		std::bind(&TcpClient::newConnection, this, _1));
	// FIXME setConnectFailedCallback
	LOG_INFO << "TcpClient::TcpClient[" << name_
			<< "] - connector " << get_pointer(connector_);
}

TcpClient::~TcpClient()
{
	LOG_INFO << "TcpClient::~TcpClient[" << name_
			<< "] - connector " << get_pointer(connector_);
	TcpConnectionPtr conn;
	bool unique = false;
	{
		MutexLockGuard lock(mutex_);
		unique = connection_.unique(); /**查看 shared_ptr 管理的对象是否只有当前的对象拥有*/
		conn = connection_;
	}
	if (conn)
	{
		assert(loop_ == conn->getLoop());
		// FIXME: not 100% safe, if we are in different thread
		CloseCallback cb = std::bind(&detail::removeConnection, loop_, _1);
		loop_->runInLoop(
			std::bind(&TcpConnection::setCloseCallback, conn, cb));
		if (unique)
		{
			conn->forceClose();
		}
	}
	else
	/**
	 * 如果客户端析构的时候，连接还没有建立起来,或者连接已经被销毁了，执行下面的操作
	*/
	{
		connector_->stop();
		// FIXME: HACK
		loop_->runAfter(1, std::bind(&detail::removeConnector, connector_));
	}
}



/**
 * 建立连接
*/
void TcpClient::connect()
{
	LOG_INFO << "TcpClient::connect[" << name_ << "] - connecting to "
           << connector_->serverAddress().toIpPort();
	this->connect_ = true;
	this->connector_->start();
}

/**
 * connector 负责将一个建立一个连接
 * TcpConnecion 负责再已经建立的连接上面进行数据的传输，和最终断开连接
*/
void TcpClient::disconnect()
{
	this->connect_ = false;
	{
		MutexLockGuard lock(this->mutex_);
		if (this->connection_)
			this->connection_->shutdown();
	}
}

/**
 * 停止当前的连接过程
*/
void TcpClient::stop()
{
	this->connect_ = false;
	this->connector_->stop();
}

/**
 * Connector 在完成了连接之后，返回 sockfd, 并调用上层 TcpClient 的回调函数通知 TcpClient
 * TcpClient 在知道了连接建立成功之后，再建立 TcpConnection 对象用于数据的传输
*/
void TcpClient::newConnection(int sockfd)
{
	this->loop_->assertInLoopThread();
	InetAddress peerAddr(sockets::getPeerAddr(sockfd));
	char buf[32];
	snprintf(buf, sizeof buf, ":%s#%d", peerAddr.toIpPort().c_str(), nextConnId_);
	++nextConnId_;

	string connName = name_ + buf;
	InetAddress localAddr(sockets::getLocalAddr(sockfd));

	TcpConnectionPtr conn(
		new TcpConnection(loop_,
			connName,
			sockfd,
			localAddr,
			peerAddr)
	);
	conn->setConnectionCallback(this->connectionCallback_);
	conn->setMessageCallback(this->messageCallback_);
	conn->setWriteCompleteCallback(this->writeCompleteCallback_);
	conn->setCloseCallback(
		std::bind(&TcpClient::removeConnection, this, _1)
	);
	{
		MutexLockGuard lock(this->mutex_);
		this->connection_ = conn;
	}
	conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
	this->loop_->assertInLoopThread();
	assert(this->loop_ = conn->getLoop());

	{
		MutexLockGuard lock(this->mutex_);
		assert(this->connection_ == conn);
		this->connection_.reset();	/*注意，shared_ptr 的性质，this->connection_ 被删除，但是 conn 依然保留*/
	}

	loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
	if (this->retry_ && this->connect_)
	{
		LOG_INFO << "TcpClient::connect[" << name_ << "] - Reconnecting to "
             << connector_->serverAddress().toIpPort();
    	connector_->restart();
	}
}

