#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include "base/CurrentThread.h"
#include "base/noncopyable.h"
#include "assert.h"
#include <pthread.h>


#if defined(__clang__) && (!defined(SWIG))
/**
 * __clang__ 这个宏可以用于判断编译器是否为 clang 编译器
*/
#define THREAD_ANNOTATION_ATTRIBUTE__(x)	__attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)
#endif

/**
 * 定义下面的这些宏有什么作用
*/

#define CAPABILITY(x)	\
	THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

#define SCOPED_CAPABILITY \
	THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)

#define GUARDED_BY(x) \
	THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))

#define PT_GUARDED_BY(x) \
	THREAD_ANNOTATION_ATTRIBUTE__(pt_guarded_by(x))

#define ACQUIRED_BEFORE(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(acquired_before(__VA_ARGS__))

#define ACQUIRED_AFTER(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(acquired_after(__VA_ARGS__))

#define REQUIRES(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define REQUIRES_SHARED(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

#define ACQUIRE(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#define ACQUIRE_SHARED(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#define RELEASE(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define RELEASE_SHARED(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define TRY_ACQUIRE(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_capability(__VA_ARGS__))

#define TRY_ACQUIRE_SHARED(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_shared_capability(__VA_ARGS__))

#define EXCLUDES(...) \
	THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

#define ASSERT_CAPABILITY(x) \
	THREAD_ANNOTATION_ATTRIBUTE__(assert_capability(x))

#define ASSERT_SHARED_CAPABILITY(x) \
	THREAD_ANNOTATION_ATTRIBUTE__(assert_shared_capability(x))

#define RETURN_CAPABILITY(x) \
	THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(x))

#define NO_THREAD_SAFETY_ANALYSIS \
	THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)



#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail(int errnum,
								const char* file,
								unsigned int line,
								const char* function)
	noexcept __attribute__((__noreturn__));
/**
 * GNU C 的一个特色就是 __attribute__.它可以设置变量的属性，类型的属性，函数的属性。
 * 这些属性在编译器进行编译的时候可以帮助编译器进行优化
*/
__END_DECLS
#endif

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
											 if (__builtin_expect(errnum != 0, 0))    \
												 __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else  // CHECK_PTHREAD_RETURN_VALUE

/**
 * 用于检测一个函数的返回值，如果返回值不是 0 的话，那么就会报错
*/
#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
											 assert(errnum == 0); (void) errnum;})

#endif


namespace muduo
{

/**
 * 获取一个互斥量的🔓，写入当前获得🔓的线程的 id
 * 释放锁，以及获取原始的互斥量
*/

class CAPABILITY("mutex") MutexLock : noncopyable
{
public:
	MutexLock() : holder_(0)
	{
		/**
		 * 互斥量的初始化的过程有两种方式，第一种是静态的方式。
		 * 使用一个静态的结构常量来初始化互斥量，PTHREAD_MUTEX_INITIALIZER
		 * 
		 * 另一种就是动态的方式，使用函数 pthread_mutex_init()
		*/
		MCHECK(pthread_mutex_init(&mutex_, NULL));
	}

	/**
	 * 这个函数只有在当前的线程获取到了互斥量之后才能调用这个函数
	*/
	bool isLockedByThisThread() const
	{
		return holder_ == CurrentThread::tid();
	}

	/**
	 * 如果获取到互斥量的线程不是当前的线程，那么运行期间就会直接报错
	*/
	void assertLocked() const ASSERT_CAPABILITY(this)
	{
		assert(isLockedByThisThread());
	}

	/**
	 * 先获取互斥量的控制，之后再写入线程 id
	*/
	void lock() ACQUIRE()
	{
		/**
		 * 获取锁失败，报错退出
		*/
		MCHECK(pthread_mutex_lock(&mutex_));	/*阻塞方式*/
		assignHolder();
	}

	void unlock() RELEASE()
	{
		unassignHolder();
		MCHECK(pthread_mutex_unlock(&mutex_));
	}

	pthread_mutex_t* getPthreadMutex()
	{
		return &mutex_;
	}


private:
	/**
	 * Condition 可以直接访问 MutexLock 内包括私有函数在内的所有的函数和变量
	*/
	friend class Condition;

	/**
	 * UnassignGuard
	 * 在 condition 当中被使用
	*/
	class UnassignGuard : noncopyable
	{
	public:
		/**
		 * 解除控制当前的互斥器的线程
		*/
		explicit UnassignGuard(MutexLock& owner) : owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard()
		{
			owner_.assignHolder();
		}

	private:
		MutexLock& owner_;
	};

	void unassignHolder()
	{
		this->holder_ = 0;
	}

	void assignHolder()
	{
		this->holder_ = CurrentThread::tid();
	}

	/**
	 * pthread 的 mutex 自身也是一个数据结构，里面包含计数值，拥有者编号等信息
	 */
	pthread_mutex_t mutex_;
	pid_t holder_;	// 拥有者的线程 id
};


/*RAII*/
/**
 * 构造时上锁，析构时解锁
*/
class SCOPED_CAPABILITY MutexLockGuard : noncopyable
{
public:
	explicit MutexLockGuard(MutexLock& mutex) ACQUIRE(mutex)
		: mutex_(mutex)
	{
		mutex_.lock();
	}

	~MutexLockGuard() RELEASE()
	{
		mutex_.unlock();
	}

private:
	MutexLock& mutex_;
};




} // namespace muduo
/**
 * 防止滥用，比方说这样使用：
 * MutexLockGuard(mutex_);
 * 一个临时的对象不可以长时间的持有🔓
*/
#define MutexLockGuard(x) error "Missing guard object name"

#endif