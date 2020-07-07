#include "net/Acceptor.h"


#include "base/Logging.h"
#include "net/EventLoop.h"
#include "net/InetAddress.h"
#include "net/SocketsOps.h"


#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


using namespace muduo;
using namespace muduo::net;


Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport)
	:   loop_(loop),
		acceptSocket_(sockets::createNonblockingOrDie(listenAddr.family())),
		accpetChannel_(loop, acceptSocket_.fd()),
		listenning_(false),
		idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
	assert(idleFd_ >= 0);
	this->acceptSocket_.setReuseAddr(true);
	this->acceptSocket_.setReusePort(reuseport);
	this->acceptSocket_.bindAddress(listenAddr);
	this->accpetChannel_.setReadCallback(
		std::bind(&Acceptor::handleRead, this)
	);
}

Acceptor::~Acceptor()
{
	this->accpetChannel_.disableAll();
	this->accpetChannel_.remove();
	::close(idleFd_);
}

void Acceptor::listen()
{
	this->loop_->assertInLoopThread();
	this->listenning_  = true;
	this->acceptSocket_.listen();
	this->accpetChannel_.enableReading();	/**这个时候， socketfd 被注册到 poller 当中，接受 poller 的检测*/
}

void Acceptor::handleRead()
{
	this->loop_->assertInLoopThread();
	InetAddress  peerAddr;	/*远端也就是客户端的 IP 地址*/

	int connfd = this->acceptSocket_.accept(&peerAddr);
	if (connfd >= 0)
	{
		if (this->newConnectionCallback_)
			newConnectionCallback_(connfd, peerAddr);
		else 
			sockets::close(connfd);
	}
	else 
	{
		LOG_SYSERR << "in Acceptor::handleRead";
    // Read the section named "The special problem of
    // accept()ing when you can't" in libev's doc.
    // By Marc Lehmann, author of libev.
		if (errno == EMFILE)
		{
			/***
			 * 每一个进程可以使用的 fd 的数量是有限的，当打开的 fd 的数量超出了限制，会发生这个错误
			*/
			::close(idleFd_);
			idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
			::close(idleFd_);
			idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
		}
	}
}
