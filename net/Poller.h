#ifndef MUDUO_NET_POLLER_H
#define MUDUO_NET_POLLER_H

#include "net/EventLoop.h"
#include "base/Timestamp.h"

#include <map>
#include <vector>

namespace muduo
{
namespace net
{

class Channel;

// 纯虚
// 为 IO 复用设计
/**
 * Poller 是 PollPoller 和 EpollPoller 的基类，采用 电平触发的方式，它是 EventLoop 的成员
 * 生命周期由 EventLoop 控制
 * 一个 EventLoop 有一个 Poller,它的生命周期和 EventLoop 的一样长
*/
class Poller : public noncopyable
{
public:
	typedef std::vector<Channel*>	ChannelList;

	Poller(EventLoop*);
	virtual ~Poller();

	/// Polls the I/O events.
	/// Must be called in the loop thread.
	virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

	/// Changes the interested I/O events.
	/// Must be called in the loop thread.
	virtual void updateChannel(Channel* channel) = 0;

	/// Remove the channel, when it destructs.
	/// Must be called in the loop thread.
	virtual void removeChannel(Channel* channel) = 0;

	virtual bool hasChannel(Channel* channel) const;

	/**
	 * 工厂模式，抽象工厂
	*/
	static Poller* newDefaultPoller(EventLoop* loop);

	void assertInLoopThread() const
	{
		this->ownerloop_->assertInLoopThread();
	}

protected:
	typedef std::map<int, Channel*>     ChannelMap;
	/**
	 * channels_ 保存了每一个 fd 所关心的事件
	 * int : index: 表示 channel 在具体的 poller 类 channel 所关联的 pollfd 的下标值.
	 * int 存储的是 channel 对应的文件描述符
	*/
	ChannelMap	channels_;

private:
	EventLoop*	ownerloop_;
};

} // namespace net

} // namespace muduo


#endif