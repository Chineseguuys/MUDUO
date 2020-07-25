#include "base/CountDownLatch.h"

using namespace muduo;

CountDownLatch::CountDownLatch(int count):mutex_(), condition_(mutex_), count_(count)
{
}

/**
 * 在线程 Thread 当中使用 CountDownLatch 我们就可以实现阻塞式的等待在 Thread 当中，直到
 * 线程开始运行，将 count_ 设置为 0，阻塞结束
*/
void CountDownLatch::wait()
{
	MutexLockGuard lock(mutex_);	// 对互斥量进行上锁
	// 构造的过程中自动上锁，退出作用域的时候自动析构时解除锁
	while(count_ > 0)
		condition_.wait();
}

void CountDownLatch::countDown()
{
	MutexLockGuard lock(mutex_);
	--count_;
	if (count_ == 0)
		condition_.notify_all();
}


int CountDownLatch::getCount() const
{
	MutexLockGuard lock(mutex_);
	return count_;
}