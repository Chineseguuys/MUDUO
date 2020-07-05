#include "base/CountDownLatch.h"

using namespace muduo;

CountDownLatch::CountDownLatch(int count):mutex_(), condition_(mutex_), count_(count)
{
}

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