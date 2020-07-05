#ifndef MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H

#include "base/Mutex.h"
#include "base/Condition.h"

#include <boost/circular_buffer.hpp>
#include <assert.h>

/**
 * 环形缓冲区，如果环的一圈被填满了数据，那么新加入的数据会覆盖旧的数据
 * 环形缓冲区是固定大小的
*/


namespace muduo
{

template<typename T>
class BoundedBlockingQueue : noncopyable
{
public:
	explicit BoundedBlockingQueue(int maxSize)
		: mutex_(),
		notEmpty_(mutex_),
		notFull_(mutex_),
		queue_(maxSize)
	{
	}

	void put(const T& x)
	{
		MutexLockGuard lock(mutex_);
		while (queue_.full())
		{
			notFull_.wait();	/*当有别的线程读取元素的时候，当前线程会被通知*/
		}
		assert(!queue_.full());
		queue_.push_back(x);
		notEmpty_.notify();	/*通知其他的线程，当前的队列中有元素*/
	}

	void put(T&& x)
	{
		MutexLockGuard lock(mutex_);
		while (queue_.full())
		{
			notFull_.wait();
		}
		assert(!queue_.full());
		queue_.push_back(std::move(x));
		notEmpty_.notify();
	}

	T take()
	{
		MutexLockGuard lock(mutex_);
		while (queue_.empty())
		{
			notEmpty_.wait();
		}
		assert(!queue_.empty());
		T front(std::move(queue_.front()));
		queue_.pop_front();
		notFull_.notify();
		return front;
	}

	bool empty() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.empty();
	}

	bool full() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.full();
	}

	size_t size() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.size();
	}

	size_t capacity() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.capacity();
	}

private:
	mutable MutexLock          mutex_;
	Condition                  notEmpty_ GUARDED_BY(mutex_);
	Condition                  notFull_ GUARDED_BY(mutex_);
	boost::circular_buffer<T>  queue_ GUARDED_BY(mutex_);
};

} // namespace muduo


#endif