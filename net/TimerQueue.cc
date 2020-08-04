#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif


#include "base/Logging.h"
#include "net/TimerQueue.h"
#include "net/EventLoop.h"
#include "net/TimerId.h"
#include "net/Timer.h"

#include <sys/timerfd.h>
#include <unistd.h>

namespace muduo
{
namespace net
{
namespace detail
{

int createTimerfd()
{
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
									TFD_NONBLOCK | TFD_CLOEXEC);
	/**
	 * CLOCK_REALTIME:系统时间的实时时间
	 * CLOCK_MONOTONIC: 以固定的速率运行，从不进行调整和复位 ,它不受任何系统time-of-day时钟修改的影响
	 * 如果使用的是 CLOCK_MONOTONIC 的话，那么在设置定时时间的时候，设置的值是从 timerfd_settime() 
	 * 开始的时间间隔。
	 * 
	 * 如果你想要睡眠到某一个绝对的时间值，那么 CLOCK_REALTIME 将是一个很好的选择，如果你要睡眠一个相对的值，
	 * CLOCK_MONOTONIC 将是一个理想的时间源
	 * 第二个参数和 eventfd() 函数中的相同
	*/
	if (timerfd < 0)
	{
		LOG_SYSFATAL << "Failed in timerfd_create";
	}
	return timerfd;	/*一个文件描述符*/
}

/**
 * 指定的时间和该函数调用时的差值
*/
struct timespec howMuchTimeFromNow(Timestamp when)
{
  	int64_t microseconds = when.microSecondsSinceEpoch()
						 - Timestamp::now().microSecondsSinceEpoch();
  	if (microseconds < 100)
  	{
		microseconds = 100;
  	}

	struct timespec ts; /**这是一个由纳秒和秒构成的纳秒精度的时间值*/
	ts.tv_sec = static_cast<time_t>(
		microseconds / Timestamp::kMicorSecondsPerSecond);
	ts.tv_nsec = static_cast<long>(
		(microseconds % Timestamp::kMicorSecondsPerSecond) * 1000);
	return ts;
}

void readTimerfd(int timerfd, Timestamp now)
{
	uint64_t howmany;
	/**
	 * 非阻塞的读取， 不管是否读取成功，都直接返回，不会被阻塞
	*/
	ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
	LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
	if (n != sizeof howmany)
	{
		LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
	}
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
	// wake up loop by timerfd_settime()
	struct itimerspec newValue;
	struct itimerspec oldValue;
	memZero(&newValue, sizeof newValue);
	memZero(&oldValue, sizeof oldValue);
	newValue.it_value = howMuchTimeFromNow(expiration);	/*it_value 表示定时器第一次超时时间*/
	/**newValue.it_interval 表示之后的超时时间，即每隔多长时间进行一次超时*/
	int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
	/**
	 * 
	 * newValue 中存储的时将要设置的超时时间
	 * oldValue 中存储之前存储的超时时间
	 * 超时时钟在 timerfd_settime() 设定完成之后开始计时
	 * 
	 * 这个函数的第二个参数要么是 TIMER_ABSTIME 或者是 0. 如果使用的是 0 的话，我们定时的过程往往是这样实现的：
	 * 如果我们要定时到 T1, 我们先获取当前的时间 T0, 得到 T1 - T0 ,将这个差值传递给 timerfd_settime()
	 * 
	 * 如果设置的是 TIMER_ABSTIME, 我们可以直接指定 T1，不需要自己去计算差值了
	 * 
	 * notes : 需要对应的关系 CLOCK_REALTIME <-> TIMER_ABSTIME
	 * 						CLOCK_MONITONIC <-> 0
	*/
	if (ret)
	{
		LOG_SYSERR << "timerfd_settime()";
	}
}

}  // namespace detail
}  // namespace net
}  // namespace muduo

using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),	/*事件循环: EventLoop 拥有一个 TimerQueue 对象，而 TimerQueue 也要知道自己属于哪一个 loop*/
    timerfd_(createTimerfd()),	/*一个时间事件文件描述符*/
    timerfdChannel_(loop, timerfd_),	/*属于一个 eventloop , 注册一个 timer 事件*/
    timers_(),
    callingExpiredTimers_(false)
{
	timerfdChannel_.setReadCallback(
		std::bind(&TimerQueue::handleRead, this));
	// we are always reading the timerfd, we disarm it with timerfd_settime.
	timerfdChannel_.enableReading();
	LOG_INFO << "TimerQueue::TimerQueue[" << this;
}


TimerQueue::~TimerQueue()
{
	timerfdChannel_.disableAll();
	timerfdChannel_.remove();
	::close(timerfd_);
	// do not remove channel, since we're in EventLoop::dtor();
	for (const Entry& timer : timers_)
	{
		delete timer.second;
	}
}

TimerId TimerQueue::addTimer(TimerCallback cb,
                             Timestamp when,
                             double interval)
	/**
	 * timer 触发之后的回调函数
	 * timer 触发的时间
	 * interval timer 重复触发的话，时间间隔
	*/
{
	Timer* timer = new Timer(std::move(cb), when, interval);
	/**
	 * 一个新的 timer 是通过 new 产生的，注意释放
	*/
	loop_->runInLoop(
		std::bind(&TimerQueue::addTimerInLoop, this, timer));
		/*这个 addTimerInLoop 未必在第一时间就被执行， 要看 loop_ 何时去调用它*/

	return TimerId(timer, timer->sequence());	/*timerid 唯一的识别和查找一个 timer 对象*/
	/**
	 * 每一个 Timer 的 sequence_ 是唯一的
	*/
}


void TimerQueue::cancel(TimerId timerId)
{
  	loop_->runInLoop(
      std::bind(&TimerQueue::cancelInLoop, this, timerId));
	  /*这个 cancelInLoop 未必在第一时间就被执行， 要看 loop_ 何时去调用它*/
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
	loop_->assertInLoopThread();
	bool earliestChanged = insert(timer);
	/**
	 * 如果 timer 插在了整个任务队列的最前面，那么
	 * 立刻设置这个 timer 的任务时间为 timerfd_ 的触发时间
	*/
	if (earliestChanged)
	{
		resetTimerfd(timerfd_, timer->expiration());
	}
}

/**
 * 将指定的 timer 任务从队列中移除
*/
void TimerQueue::cancelInLoop(TimerId timerid)
{
	loop_->assertInLoopThread();
	assert(timers_.size() == activeTimers_.size());
	ActiveTimer timer(timerid.timer_, timerid.sequence_);
	ActiveTimerSet::iterator it = activeTimers_.find(timer);
	if (it != activeTimers_.end())
	{
		size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
		assert(n == 1); (void) n;
		delete it->first;
		activeTimers_.erase(it);
	}
	else if (callingExpiredTimers_)
	{
		cancelingTimers_.insert(timer);
	}
	assert(timers_.size() == activeTimers_.size());
}

void TimerQueue::handleRead()
{
	loop_->assertInLoopThread();
	Timestamp now(Timestamp::now());
	readTimerfd(timerfd_, now);		/**在这里 read 的作用是什么，由于是非阻塞的，无论这个函数执行结果如何，下面的内容都同样的执行*/
	/**
	 * 由于事件是由 epoll 所监控，有事件发生的时候触发的，所以这里的 readTimerfd 一定可以读到相应的内容
	 * read() 的作用就是重置 timerfd_ 的
	*/

	std::vector<Entry> expired = getExpired(now);

	callingExpiredTimers_ = true;
	cancelingTimers_.clear();

	for (const Entry& it : expired){
		it.second->run();
	}

	this->reset(expired, now);
}

/**
 * 获取已经超时的所有的 timer 的信息
*/
std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
	assert(timers_.size() == activeTimers_.size());
	LOG_TRACE << "have " << timers_.size() << " timers in queue before getExpired";
	std::vector<Entry> expired;
	Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator end = timers_.lower_bound(sentry);
	/**
	 * upper_bound 的比较是 std::pair 的比较，所以先比较 timestamp 的大小，大小相同的时候 比较 Timer* 的大小
	 * sentry 中的 Timer* 设置为了内存中内存地址允许的最大值，所以实际的 Entry 对象中，不存在 Timer* 的地址会比
	 * sentry 中的大
	 * 找到 set 当中，第一个比 setry 大的数据的位置
	*/
	assert(end == timers_.end() || now < end->first);
	// 把这些已经过期的 timer 全部拷贝出来，并从 timers_ 和 activetimers_ 中进行移除
	std::copy(timers_.begin(), end, std::back_inserter(expired));
  	/**拷贝从 begin 到 end 的所有的数据，包括 begin ,但是不包括 end*/
	timers_.erase(timers_.begin(), end);
  	/**删除从 begin 开始到 end 结束的所有的数据，包括 begin,但是不包括 end*/

	LOG_TRACE << "have " << expired.size() << " timer expired";

	for (const Entry& it : expired){
		ActiveTimer timer(it.second, it.second->sequence());
		size_t n = activeTimers_.erase(timer);
		assert(n == 1); (void)n;
	}
	LOG_TRACE << "timer_ size = " << timers_.size() << ", activeTimers_ size = " << activeTimers_.size();
	assert(timers_.size() == activeTimers_.size());
	return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
	Timestamp nextExpire;
	
	for (const Entry& it : expired){
		ActiveTimer timer(it.second, it.second->sequence());
		/**
		 * 如果这个 timer 被设置为周期性的，并且没有被取消，那么我们在它当前过期之后，重新将其放入 set 当中
		*/
		if (it.second->repeat() && cancelingTimers_.find(timer) == cancelingTimers_.end())
		{
			it.second->restart(now);
			insert(it.second);
		}
		else 
		{
			delete it.second;
			/**
			 * 已经没有用的定时器需要及时的释放内存
			*/
		}
	}

	if (!timers_.empty())
	{
		nextExpire = timers_.begin()->second->expiration();
	}
	if (nextExpire.valid())
	{
		/**
		 * timerfd 被重新设定为一个新的值，这个值就是当前的 set 中最早的那个 timer 定时值
		*/
		resetTimerfd(timerfd_, nextExpire);
	}
}


bool TimerQueue::insert(Timer* timer)
{
	loop_->assertInLoopThread();
	assert(timers_.size() == activeTimers_.size());	/*这两个集合要保持相同的大小*/
	bool earliestChanged = false;
	Timestamp when = timer->expiration();
	TimerList::iterator it = timers_.begin();
	/**
	 * 如果要插入的 timer 任务处在所有任务的最前面
	 * 两种情况，当前的 timers_ 是空的，或者 timer 执行的时间，比所有集合中的执行的时间都要早
	*/
	if (it == timers_.end() || when < it->first)
	{
		earliestChanged = true;
	}
	{
		std::pair<TimerList::iterator, bool> result
		= timers_.insert(Entry(when, timer));
		LOG_TRACE << "TimerQueue::insert into timers_ - timer : " << timer->sequence(); 
		assert(result.second); (void)result;
	}
	{
		std::pair<ActiveTimerSet::iterator, bool> result
		= activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
		LOG_TRACE << "TimerQueue::insert into activeTimers_ - timer : " << timer->sequence();
		assert(result.second); (void)result;
	}

	assert(timers_.size() == activeTimers_.size());
	return earliestChanged;
} 