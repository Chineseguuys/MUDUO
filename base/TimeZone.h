#ifndef MUDUO_BASE_TIMEZONE_H
#define MUDUO_BASE_TIMEZONE_H

#include "base/copyable.h"
#include <memory>
#include <time.h>


namespace muduo
{

/**
 * 时区和夏令时
 * 时间的跨度的范围是 1970 - 2030
*/
class TimeZone : public muduo::copyable
{
public:
	explicit TimeZone(const char* zonefile);
	TimeZone(int eastOfUtc, const char* tzname);
	TimeZone() = default;

	bool valid() const
	{
		return static_cast<bool>(data_);
	}

	/**
	 * 将用秒代表的时间转化为用 tm 代表的时间
	 * 
	 * tm 结构用于表示 year-month-day-hour-minutes-seconds
	 * 即，tm 的最高的精度为 1 second
	*/
	struct tm toLocalTime(time_t secondsSinceEpoch) const;
	/**
	 * 用 tm 代表的时间转换为秒代表的时间
	*/
	time_t fromLocalTime(const struct tm&) const;

	// gmtime(3)
	static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);
	// timegm(3)
	static time_t fromUtcTime(const struct tm&);
	// year in [1900..2500], month in [1..12], day in [1..31]
	static time_t fromUtcTime(int year, int month, int day,
								int hour, int minute, int seconds);

	struct Data;

private:
	std::shared_ptr<Data> data_;
};

} // namespace muduo




#endif