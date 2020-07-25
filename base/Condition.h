#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include "base/Mutex.h"

#include <pthread.h>

namespace muduo
{

/**
 * 当一个线程获取了某些资源,同时还需要其他的一些资源,但是这些资源的条件还没有满足,我们可以使用条件变量
 * 条件变量和操作系统中的信号量非常的相似
*/

/**
 * 为什么要使用 condition
 * 假设我们有两个线程，一个共享区域的变量 a = 0;
 * A : if (a > 0) {执行某些操作}
 * B ： a += 1
 * 假设A 线程先获取到了 a 的控制，但是变量的条件不满足，如果 A 在这个位置等待 a 的值发生变化，但是
 * 不释放自己的锁，那么 B 线程也无法获取 a 的控制权，那么就会导致 A 永久的等待的情况
 * 
 * 为了防止这种情况的出现，引入了 Condition， A 在发现条件不满足的时候，释放当前控制的量。A 转向等待条件变量，
 * 在其他的线程完成了变量的修改的过程之后，可以通知 A，A 可以尝试再次获取锁来继续自己的运行过程
*/
class Condition : noncopyable
{
public:
	explicit Condition(MutexLock& mutex)
		: mutex_(mutex)
	{
		MCHECK(pthread_cond_init(&pcond_, NULL));
	}

	~Condition()
	{
		MCHECK(pthread_cond_destroy(&pcond_));
	}

	void wait()
	{
		MutexLock::UnassignGuard ug(mutex_);
		/**
		 * 将 mutex_ 的 holder_ 设置为 0，在 pthread_cond_wait() 当中，当前的线程将会释放锁，并进入到
		 * 休眠当中
		*/
		MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
	}

	bool waitForSeconds(double seconds);	// 实现在 Condition.cc 里面

	void notify()
	{
		MCHECK(pthread_cond_signal(&pcond_));
	}

	void notify_all()
	{
		MCHECK(pthread_cond_broadcast(&pcond_));
	}

private:
	MutexLock&      mutex_;
	pthread_cond_t  pcond_;
};
} // namespace muduo




#endif