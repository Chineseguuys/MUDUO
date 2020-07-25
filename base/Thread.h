#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H


#include "base/Atomic.h"
#include "base/CountDownLatch.h"
#include "base/Types.h"

#include <functional>
#include <memory>
#include <pthread.h>

namespace muduo
{

class Thread : noncopyable 
{
public:
	typedef std::function<void ()> ThreadFunc;

	explicit Thread(ThreadFunc, const string& name  = string());

	~Thread();

	void start();
	int join();

	bool started() const {return this->started_;}

	pid_t tid() const { return tid_; }

	const string& name() const { return name_; }

	static int numCreated() { return numCreated_.get(); }

private:
	void setDefaultName();

	bool				started_;
	bool				joined_;
	pthread_t			pthreadId_;
	pid_t				tid_;
	ThreadFunc			func_;
	string				name_;
	CountDownLatch		latch_; /**线程安全的，阻塞式的访问*/

	static AtomicInt32 numCreated_; /**原子访问，不需要进行上锁*/
	/**
	 * static 类型，所有的 Thread 实例只需要保留一个 numCreated_; 
	 * 它可以用来指示创建的 Thread 的数量
	*/
};

}



#endif