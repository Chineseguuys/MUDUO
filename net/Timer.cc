#include "net/Timer.h"


using namespace muduo;
using namespace muduo::net;

AtomicInt64 muduo::net::Timer::s_numCreated_;


void Timer::restart(Timestamp now)
{
	if (this->repeat_)
	{
		this->expiration_ = addTime(now, interval_);
	}
	else 
	{
		this->expiration_ = Timestamp::invalid();	/* = TimeStamp(0)*/
	}
}