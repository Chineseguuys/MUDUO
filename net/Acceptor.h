#ifndef MUDUO_NET_ACCEPTOR_H
#define MUDUO_NET_ACCEPTOR_H

#include <functional>

#include "net/Channel.h"
#include "net/Socket.h"

namespace muduo
{
namespace net
{
class EventLoop;
class InetAddress;

/**
 * 到达的 TCP 连接使用这个类进行接收
*/
class Acceptor : public noncopyable 
{
public:
	typedef std::function<void (int sockfd_, const InetAddress&)> NewConnectionCallback;
private:
	EventLoop*		loop_;
	Socket			acceptSocket_;
	Channel			accpetChannel_;
	NewConnectionCallback	newConnectionCallback_;
	bool listenning_;
	int idleFd_;
private:
	void handleRead();
public:
	Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
	~Acceptor();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{ newConnectionCallback_ = cb; }

	bool listenning() const { return listenning_; }
	void listen();
};

} // namespace net

} // namespace muduo




#endif