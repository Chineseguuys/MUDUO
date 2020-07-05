#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

#include "base/Types.h"

namespace muduo
{
namespace CurrentThread
{
	extern __thread int		t_cachedTid;
	extern __thread char	t_tidString[32];
	extern __thread int		t_tidStringLength;
	extern __thread const char*		t_threadName;


	void cacheTid();

	/**
	 * 获取当前的线程的 tid
	*/
	inline int tid()
	{
		/**
		 * 注意，__builtin_expect() 的值与函数无关
		 * 如果 t_cachedTid == 0 成立的话，那么 if 的内容执行
		 * __builtin_expect(t_cachedTid == 0, 0) 表示表达式 t_cachedTid == 0 成立的可能性比较的小
		 * __builtin_expect(t_cachedTid == 0, 1) 表示表达式 t_cachedTid == 0 成立的可能性比较的大
		 * 
		 * 这个函数的主要的目的是为了编译器可以根据可能性的大小来优化这个 if_else 语句。即优化可能性大
		 * 的分支
		*/
		if (__builtin_expect(t_cachedTid == 0, 0))
		{
			cacheTid();
		}
		return t_cachedTid;
	}

	inline const char* tidString()
	{
		return t_tidString;
	}

	inline int tidStringLength()
	{
		return t_tidStringLength;
	}

	inline const char* name()
	{
		return t_threadName;
	}

	bool isMainThread();

	void sleepUsec(int64_t usec);

	string stackTrace(bool demangle);	// 获取线程的堆栈信息

} // namespace CurrentThread

} // namespace muduo



#endif