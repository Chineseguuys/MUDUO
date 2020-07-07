#ifndef MUDUO_NET_EVENTLOOPTHREAD_H
#define MUDUO_NET_EVENTLOOPTHREAD_H

#include "base/Condition.h"
#include "base/Mutex.h"
#include "base/Thread.h"

/**
 * EventLoopThread 让 eventloop 在子线程中运行，并将 eventloop 的指针返回给我们主线程
 * 这样我们在主线程当中也可以对 eventloop 进行一系列的操作
 * 由于在多个线程之间进行操作，所以涉及到了多线程的技术
 */
namespace muduo
{
namespace net
{
class EventLoop;

class EventLoopThread : noncopyable 
{
public:
	typedef std::function<void (EventLoop*)> ThreadInitCallback;

	EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
					const string& name = string());
	~EventLoopThread();
	EventLoop* startLoop();

private:
	EventLoop*			loop_  GUARDED_BY(mutex_);
	bool				exiting_;
	Thread				thread_;
	MutexLock			mutex_;
	Condition 			cond_ GUARDED_BY(mutex_);
	ThreadInitCallback	callback_;

private:
	void threadFunc();
};

} // namespace net

} // namespace muduo


#endif