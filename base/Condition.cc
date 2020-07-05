#include "base/Condition.h"

#include <errno.h>


bool muduo::Condition::waitForSeconds(double seconds)
{
	/**
	 * 时间是从 1970-1-1 开始计算到现在的秒数。精度为 s + ns
	 * 先获取当前的时间，格式为 s + ns
	 * 之后根据 seconds 计算出需要等待的时间 s + ns
	 * 将当前的时间和需要等待的时间相加，就是我们等待结束时的绝对时间
	*/
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);

	const int64_t kNanoSecondsPerSecond = 1000000000;
	int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

	abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec += static_cast<long>((abstime.tv_nsec + nanoseconds)  % kNanoSecondsPerSecond);

	MutexLock::UnassignGuard ug(mutex_);
	return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}