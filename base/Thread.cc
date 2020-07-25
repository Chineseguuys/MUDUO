#include "base/Thread.h"
#include "base/CurrentThread.h"
#include "base/Exception.h"
#include "base/Logging.h"

#include <type_traits>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>


namespace muduo
{

namespace detail
{

pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork()
{
	muduo::CurrentThread::t_cachedTid = 0;
	muduo::CurrentThread::t_threadName = "main";
	CurrentThread::tid();
}

class ThreadNameInitializer
{
public:
	ThreadNameInitializer()
	{
		muduo::CurrentThread::t_threadName = "main";
		CurrentThread::tid();
		pthread_atfork(NULL, NULL, &afterFork);
		/**
		 * 子进程会复制父进程的数据，所以子进程和父进程之间是独立的，父进程可能有多个线程，这些线程相关的信息也会复制给
		 * 子进程
		 * int pthread_atfork(void (*__prepare)(), void (*__parent)(), void (*__child)()) throw()
		 * __prepare 表示在执行 fork() 之前，父进程需要进行的工作；__child 表示 fork() 之后，子进程的准备工作
		 * __parent 表示 fork() 之后父进程执行的收尾工作
		*/
	}
};

ThreadNameInitializer init;


/**
 * 为线程函数 startData(void* ) 提供参数 
 * 这个类封装了线程执行所需要的所有的数据
*/
struct ThreadData
{
	typedef muduo::Thread::ThreadFunc ThreadFunc;
	ThreadFunc func_;
	string name_;
	pid_t* tid_;
	CountDownLatch* latch_;

	ThreadData(ThreadFunc func,
				const string& name,
				pid_t* tid,
				CountDownLatch* latch)
		: func_(std::move(func)),
		name_(name),
		tid_(tid),
		latch_(latch)
	{ }

	void runInThread()
	{
		*tid_ = muduo::CurrentThread::tid();	/*外部的值也被修改*/
		tid_ = NULL;	/*这里的 tid_ 变为 NULL, 不影响外部的值*/
		latch_->countDown();
		latch_ = NULL;

		muduo::CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();
		::prctl(PR_SET_NAME, muduo::CurrentThread::t_threadName);
		/*改变进程的名字*/
		try
		{
			func_();	/*线程中要运行的函数 这是 C++ 中的仿函数*/
			muduo::CurrentThread::t_threadName = "finished";
			/**函数运行结束，设置 t_threadName 为 finshed*/
		}
		catch (const Exception& ex)
		{
			muduo::CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
			abort();
		}
		catch (const std::exception& ex)
		{
			muduo::CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			abort();
		}
		catch (...)
		{
			muduo::CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
			throw; // rethrow
		}
	}
};

/**
 * 这个函数是真正在子线程中被包装的函数，它被传递到  pthread_create() 中的任务函数
*/
void* startThread(void* obj)
{
	ThreadData* data = static_cast<ThreadData*>(obj);
	data->runInThread();
	delete data;
	return NULL;
}

} // namespace detail


void CurrentThread::cacheTid()
{
	if (t_cachedTid == 0)
	{
		t_cachedTid = detail::gettid();
		t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d", t_cachedTid);
	}
}

bool CurrentThread::isMainThread()
{
	return tid() == ::getpid(); /*主线程的 线程 id 和进程的进程 id 是相同的*/
}

void CurrentThread::sleepUsec(int64_t usec)
{
	struct timespec ts = { 0, 0 };
	ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicorSecondsPerSecond);
	ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicorSecondsPerSecond * 1000);
	LOG_INFO << "CurrentThread::sleepUsec";
	::nanosleep(&ts, NULL);
}

AtomicInt32 Thread::numCreated_;	/*这是一个全局的变量，在整个程序的周期内可用*/

Thread::Thread(ThreadFunc func, const string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(std::move(func)),
    name_(n),	/*线程的名称，应该是可以自己设置的*/
    latch_(1)	/*这个值被设置为 1*/
{
  	setDefaultName();	/*为线程设置一个名称*/
}

Thread::~Thread()
{
	if (started_ && !joined_)
	{
		pthread_detach(pthreadId_);
		/*线程结束之后就会自动的销毁*/
	}
}

void Thread::setDefaultName()
{
	int num = numCreated_.incrementAndGet();	/*可以从 numCreated_ 拿到一个唯一的值*/
	if (name_.empty())
	/*如果线程没有名字，就分配一个名称*/
	{
		char buf[32];
		snprintf(buf, sizeof buf, "Thread%d", num);
		name_ = buf;
	}
}

void Thread::start()
{
	assert(!started_);/*如果在线程开始执行之前，started_ 的值就已经是 true, 那么就会报错*/
	started_ = true;
	// FIXME: move(func_)
	detail::ThreadData* data = new detail::ThreadData(func_, name_, &tid_, &latch_);
	if (pthread_create(&pthreadId_, NULL, &detail::startThread, data))
	/**
	 * 创建一个线程，线程的入口函数是 startThread
	 * 线程函数的参数是 ThreadData* 对象
	*/
	{
		/*线程创建失败*/
		started_ = false;
		delete data; // or no delete?
		LOG_SYSFATAL << "Failed in pthread_create";
	}
	else
	{
		/*线程创建成功，如果函数正常执行的话，那么 latch_ 的值会减到 0 -- runInThread()*/
		latch_.wait();
		assert(tid_ > 0);	/*runInThread()*/
	}
}

int Thread::join()
{
	assert(started_);
	assert(!joined_);
	joined_ = true;
	return pthread_join(pthreadId_, NULL);
	/**
	 * 如果 thread 是以 joinable 形式进行运行的时候，子线程在退出的时候不会删除自己的
	 * 堆栈的数据，在主线程中调用 pthread_join() 可以获取这些数据（子线程返回的信息）
	 * 之后再删除这些数据，如果主线程调用这个函数的时候，子线程还没有执行完毕，那么会进行阻塞
	*/
}

} // namespace muduo
