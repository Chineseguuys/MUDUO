#ifndef MUDUO_NET_CONNECTOR_H
#define MUDUO_NET_CONNECTOR_H

#include "base/noncopyable.h"
#include "net/InetAddress.h"

#include <functional>
#include <memory>

namespace muduo
{
namespace net
{
class Channel;
class EventLoop;

class Connector : public noncopyable,
		public std::enable_shared_from_this<Connector>
{
public:
	typedef std::function<void (int sockfd)> NewConnectionCallback;

	Connector(EventLoop* loop, const InetAddress& serverAddr);
	~Connector();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{ newConnectionCallback_ = cb; }

	void start();  // can be called in any thread
	void restart();  // must be called in loop thread
	void stop();  // can be called in any thread

	const InetAddress& serverAddress() const { return serverAddr_; }

private:
	enum States { KDisconnected, KConnecting, KConnected };
	static const int kMaxRetryDelayMs = 30*1000;	/**最大的重试延时*/
	static const int kInitRetryDelayMs = 500;	/*默认的重试延时*/

	EventLoop*			loop_;
	InetAddress			serverAddr_;
	bool				connect_;
	States				state_;
	std::unique_ptr<Channel>	channel_;
	NewConnectionCallback		newConnectionCallback_; /**连接成功之后所调用的回调函数*/
	int 						retryDelayMs_;	/*连接失败之后的重试计时*/

private:
	void setState(States s) { this->state_ = s; }
	void startInLoop();
	void stopInLoop();
	void connect();
	void connecting(int sockfd);
	void handleWrite();
	void handleError();
	void retry(int sockfd);
	int removeAndResetChannel();
	void resetChannel();
};

} // namespace net

} // namespace muduo



#endif