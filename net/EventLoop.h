#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <atomic>
#include <functional>
#include <vector>

#include <boost/any.hpp>


#include "base/Mutex.h"
#include "base/CurrentThread.h"
#include "base/Timestamp.h"
#include "net/Callbacks.h"
#include "net/TimerId.h"

namespace muduo
{
namespace net
{

class Channel;
class Poller;
class TimerQueue;

/**
 * Reactor : 一个线程最多只有一个
 * 每一个TcpConnection 必须归某一个 EventLoop 来进行管理,它负责 IO 和定时器事件的分派。它用 eventfd() 来异步唤醒，这有别于传统的
 * 用一对 pipe 的方法。它用 TimerQueue 作为定时器管理， 用 Poller 作为 IO multiplexing
 * 
 * Reactor 模式使用非阻塞的 IO + poll（epoll)函数来处理并发，程序的基本的结构是一个事件循环，
 * 以事件驱动和事件回调来实现业务逻辑
*/

class EventLoop : noncopyable
{
public:
	typedef std::function<void ()> Functor;

	EventLoop();
	~EventLoop();

	// 必须和创建对象的线程位于同一个线程
	void loop();

	// 离开循环  如果你通过一个原始指针进行访问，那么它不是 100% 线程安全的
	// 通过 shared_ptr 进行访问可以保证 100% 线程安全
	void quit();

	// poll 返回的时间，往往意味着数据到达
	Timestamp pollReturnTime() const { return this->pollReturnTime_; }

	int64_t iteration() const { return this->iteration_; }

	/**
	 * 立即在循环线程中调用回调
	 * 它唤醒循环，并执行 cb
	 * 如果在同一个循环线程当中，那么 cb 在这个函数中运行
	 * 从其他的线程进行调用时安全的
	*/
	void runInLoop(Functor cb);

	void queueInLoop(Functor cb);

	size_t queueSize() const;

	/**
	 * 在 时间 time 调用 cb 回调函数
	 * 这个函数是线程安全的
	*/
	TimerId runAt(Timestamp time, TimerCallback cb);

	/**
	 * 在延时 delay 秒之后再执行回调
	 * 线程安全
	*/
	TimerId runAfter(double delay, TimerCallback cb);

	/**
	 * 每 interval 秒执行一次回调
	 * 线程安全
	*/
	TimerId runEvery(double interval, TimerCallback cb);

	///
	/// 取消 timer.
	/// 线程安全
	///
  	void cancel(TimerId timerId);

	// 内部使用
	void wakeup();
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);
	bool hasChannel(Channel* channel);

	// pid_t threadId() const { return threadId_; }
	void assertInLoopThread()
	{
		/**如果函数的调用不是在创建 EventLoop 的线程中进行的，那么就会报错*/
		if (!isInLoopThread())
		{
			abortNotInLoopThread();
		}
	}
	bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
	/**
	 * threadId_ 是创建 EventLoop 的线程的 id, CurrentThread::tid() 是当前调用这个函数的线程的 id
	*/
	// bool callingPendingFunctors() const { return callingPendingFunctors_; }
	bool eventHandling() const { return eventHandling_; }

	void setContext(const boost::any& context)
	{ context_ = context; }

	const boost::any& getContext() const
	{ return context_; }

	boost::any* getMutableContext()
	{ return &context_; }

	static EventLoop* getEventLoopOfCurrentThread();

private:
	void abortNotInLoopThread();
	void handleRead();  // waked up
	void doPendingFunctors();

	void printActiveChannels() const; // DEBUG

	typedef std::vector<Channel*> ChannelList;
	bool looping_; /*原子性*/
	std::atomic<bool> quit_;
	bool eventHandling_;	/*原子性*/
	bool callingPendingFunctors_; /*原子性*/
	int64_t 					iteration_;
	const pid_t 				threadId_;
	Timestamp 					pollReturnTime_;
	std::unique_ptr<Poller> 	poller_;	/*多态的性质*/
	std::unique_ptr<TimerQueue> timerQueue_;
	int wakeupFd_;
	std::unique_ptr<Channel> 	wakeupChannel_;
  	boost::any context_;
	// scratch variables /*临时变量*/
  	ChannelList activeChannels_;
  	Channel* currentActiveChannel_;

	mutable MutexLock mutex_;
  	std::vector<Functor> pendingFunctors_ GUARDED_BY(mutex_);
};


} // namespace net

} // namespace muduo



#endif