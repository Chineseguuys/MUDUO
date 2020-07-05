#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include "base/Mutex.h"
#include "base/Timestamp.h"
#include "net/Channel.h"
#include "net/Callbacks.h"


namespace muduo
{
namespace net
{

class TimerId;
class EventLoop;
class Timer;

class TimerQueue : public noncopyable
{
public:
	explicit TimerQueue(EventLoop* loop);
  	~TimerQueue();

  ///
  /// Schedules the callback to be run at given time,
  /// repeats if @c interval > 0.0.
  ///
  /// Must be thread safe. Usually be called from other threads.
  	TimerId addTimer(TimerCallback cb,
                   Timestamp when,
                   double interval);

  	void cancel(TimerId timerId);
private:
	typedef std::pair<Timestamp, Timer*> Entry;
	typedef std::set<Entry> TimerList;
	typedef std::pair<Timer*, int64_t> ActiveTimer;
	typedef std::set<ActiveTimer> ActiveTimerSet;

  	void addTimerInLoop(Timer* timer);
  	void cancelInLoop(TimerId timerId);
  	// called when timerfd alarms
  	void handleRead();
  	// move out all expired timers
  	std::vector<Entry> getExpired(Timestamp now);
  	void reset(const std::vector<Entry>& expired, Timestamp now);

  	bool insert(Timer* timer);

  	EventLoop* loop_;
  	const int timerfd_;
  	Channel timerfdChannel_;
  	// Timer list sorted by expiration
  	TimerList timers_;

  	// for cancel()
  	ActiveTimerSet activeTimers_;
	/**
	 * activeTimers_ 和 timers_ 应当保持一致， activeTimers_ 是用来根据 Timer* 进行快速查找使用的
	*/
  	bool callingExpiredTimers_; /* atomic */
  	ActiveTimerSet cancelingTimers_;	
};
} // namespace net

} // namespace muduo



#endif