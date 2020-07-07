#ifndef MUDUO_BASE_THREADPOOL_H
#define MUDUO_BASE_THREADPOOL_H

#include "base/Mutex.h"
#include "base/Condition.h"
#include "base/Thread.h"
#include "base/Types.h"

#include <deque>
#include <vector>

namespace muduo
{

class ThreadPool : public  noncopyable 
{
public:
	typedef std::function<void ()> Task;
private:
	mutable MutexLock  mutex_;
	Condition notEmpty_  GUARDED_BY(mutex_);
	Condition notFull_	GUARDED_BY(mutex_);
	std::string name_;
	Task 	threadInitCallback_;
	std::vector<std::unique_ptr<muduo::Thread> > threads_;
	std::deque<Task> queue_ GUARDED_BY(mutex_);
	size_t maxQueueSize_;
	bool running_;
private:
	bool isFull() const REQUIRES(mutex_);
	void runInThread();
	Task take();
public:
	explicit ThreadPool(const std::string nameArg = std::string("ThreadPool"));
	~ThreadPool();

	/**
	 * 下面两个设置属性的函数必须在线程池执行之间进行调用
	*/
	void setMaxQueueSize(int maxSize) { this->maxQueueSize_ = maxSize; }
	void setThreadInitCallback(const Task& cb) 
	{
		this->threadInitCallback_ = cb;
	}

	void start(int numThreads);
	void stop();

	const std::string& name() const { return this->name_; }
	size_t queueSize() const;

	void run(Task f);	/**添加任务*/
};

} // namespace muduo





#endif