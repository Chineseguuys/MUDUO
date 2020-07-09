#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include "base/noncopyable.h"
#include "base/Timestamp.h"

#include <functional>
#include <memory>

namespace muduo
{
namespace net
{

class EventLoop;

/**
 * 用于 socket 事件的分发
 * 这个类并不实际拥有文件描述符
 * 文件描述符可以是一个 socketfd, 可以是一个 eventfd, 一个 timerfd 或者一个 signalfd
 * 
 * Channel 是一个 selectable IO Channel 负责注册与响应 IO 事件，注意它不拥有 file descriptor 
 * 它是 Acceptor,Connector, EventLoop, TimerQueue, TcpConnection 的成员，生命周期由这些类控制
*/
class Channel : public noncopyable 
{
public:
	typedef std::function<void()> EventCallback;    /*事件回调*/
	typedef std::function<void (Timestamp)> ReadEventCallback;

	Channel(EventLoop* event, int fd);
	~Channel();

	void handleEvent(Timestamp receiveTime);

	// 几个回调
	void setReadCallback(ReadEventCallback cb)
	{ readCallback_ = std::move(cb); }
	void setWriteCallback(EventCallback cb)
	{ writeCallback_ = std::move(cb); }
	void setCloseCallback(EventCallback cb)
	{ closeCallback_ = std::move(cb); }
	void setErrorCallback(EventCallback cb)
	{ errorCallback_ = std::move(cb); }

	void tie(const std::shared_ptr<void>&);

	int events() const { return events_; }
	void set_revents(int revt) { revents_ = revt; } // used by pollers
	// int revents() const { return revents_; }
	bool isNoneEvent() const { return events_ == kNoneEvent; }

	void enableReading() { events_ |= kReadEvent; update(); }
	void disableReading() { events_ &= ~kReadEvent; update(); }
	void enableWriting() { events_ |= kWriteEvent; update(); }
	void disableWriting() { events_ &= ~kWriteEvent; update(); }
	void disableAll() { events_ = kNoneEvent; update(); }
	bool isWriting() const { return events_ & kWriteEvent; }
	bool isReading() const { return events_ & kReadEvent; }


	// for Poller
	/**
	 * 在 poll 当中进行使用，方便查找 channel 对应的 pollfd_ 对象，
	 * 在 epoll 当中没有什么作用 (epoll_event.data.ptr 存储了 channel*，直接就可以查找得到)
	*/
	int index() { return index_; }
	void set_index(int idx) { index_ = idx; }

	// for debug
	string reventsToString() const;
	string eventsToString() const;

	void doNotLogHup() { logHup_ = false; }

	EventLoop* ownerLoop() { return loop_; }
	void remove();

	int fd() const  { return this->fd_; }
private:

	static string eventsToString(int fd, int ev);

  	void update();
  	void handleEventWithGuard(Timestamp receiveTime);

	static const int	kNoneEvent;
	static const int 	kReadEvent;
	static const int 	kWriteEvent;

	EventLoop* loop_;
	const int  fd_;
	int        events_;
	/**
	 * events_ 存储的是一个事件需要被监控的事件的类型，比方说一个 socket 连接，可能即需要
	 * 监控读事件又需要监控写事件 (poll, epoll)
	*/
	int        revents_; // it's the received event types of epoll or poll
	/**
	 * revents_ 是 epoll 或者 poll 返回的事件类型，可能是可读，可能是可写，还有可能是一些错误描述
	 * channel 需要根据相应的 revents_ 来调用相应的 回调来处理这些事件
	*/
	int        index_; // used by Poller.
	bool       logHup_;	/*是否对 hup 进行日志的记录*/

	std::weak_ptr<void> tie_;
	bool tied_;
	bool eventHandling_;
	bool addedToLoop_;
	ReadEventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_;
	EventCallback errorCallback_;
};
} // namespace net

} // namespace muduo




#endif