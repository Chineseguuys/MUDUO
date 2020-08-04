#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include "base/noncopyable.h"

#include "stdint.h"

// 支持原子操作的整数，赋值，加，减（减的过程每一次减1）

namespace muduo
{

namespace detail
{

template <typename T>
class AtomicIntegerT : noncopyable
{
public:
	AtomicIntegerT() : value_(0){}

	T get()
	{
		/**
		 * 这是由编译器提供的内建函数，用于原子操作
		*/
		return __sync_val_compare_and_swap(&value_, 0, 0);
		/**
		 * __sync_val_compare_and_swap(type* ptr, type oldval, type newval)
		 * 	如果 *ptr == oldval, 那么将 newval  写入到 *ptr 当中，并且返回 *ptr 的旧的值
		*/
	}

	T getAndAdd(T x)
	{
		/**
		 * 将输入的数值加到指定的指针内容中，这是一个原子的操作
		 * 返回的值是相加之前的值
		*/
		return __sync_fetch_and_add(&value_, x);
		/**
		 * __sync_fetch_and_add(&value_, x) : 返回 value 的旧值，将 value 设置为 x
		 * __sync_add_and_fetch(&value_, x) : value 的值设置为 x, 返回 value 的旧值
		*/
	}

	T addAndGet(T x)
	{
		return getAndAdd(x) + x;
	}

	T incrementAndGet()
	{
		return addAndGet(1);
	}

	T decrementAndGet()
	{
		return addAndGet(-1);
	}

	void add(T x)
	{
		getAndAdd(x);
	}

	void increment()
	{
		incrementAndGet();
	}

	void decrement()
	{
		decrementAndGet();
	}

	T getAndSet(T newValue)
	{
		return __sync_lock_test_and_set(&value_, newValue);
		/**
		 * 如果 value 的值是 0 ，那么返回 value， 并将数值设置为 newValue
		 * 这个函数常用于自旋锁。比方说，value_ 的值为 0 表示没有线程进入临界区
		*/
	}

private:
	volatile T value_;
};

} // namespace detail

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

} // namespace muduo


#endif