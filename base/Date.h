#ifndef MUDUO_BASE_DATE_H
#define MUDUO_BASE_DATE_H

#include "base/copyable.h"
#include "base/Types.h"

struct tm;

namespace muduo
{

/**
 * 格里高丽历
 * 这个类是一个不可修改类
 * 推荐使用传值的方式进行使用
 * 
 * 从 1970年1月1号开始计算的天数
 * 可以获取直接的天数，也可以获取相关的年月日的数据
*/
class Date : public muduo::copyable
{
public:
	struct YearMonthDay
	{
		int year;	// [1900  ... 2500]
		int month;	// [1 ... 12]
		int day;	// [1 ... 31]
	};

	static const int kDaysPerWeek = 7;
	static const int kJulianDayOf1970_01_01;

	Date():julianDayNumber_(0)
	{}

	Date(int year, int month, int day);

	explicit Date(int julianDayNum)
		: julianDayNumber_(julianDayNum)
	{}

	explicit Date(const struct tm&);

	void swap(Date& that)
	{
		std::swap(this->julianDayNumber_, that.julianDayNumber_);
	}

	bool valid() const 
	{
		return this->julianDayNumber_ > 0;
	}

	string toIsoString() const;

	struct YearMonthDay yearMonthDay() const;

	int year() const
	{
		return this->yearMonthDay().year;
	}

	int month() const
	{
		return this->yearMonthDay().month;
	}

	int day() const
	{
		return this->yearMonthDay().day;
	}

	int weekDay() const
	{
		return (this->julianDayNumber_ + 1) % Date::kDaysPerWeek;
	}

	int julianDayNumber() const
	{
		return this->julianDayNumber_;
	}

private:
	int julianDayNumber_;
};

/**
 * 时间的比较
 * 越早的时间越小
*/

inline bool operator < (Date x, Date y)
{
	return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator == (Date x, Date y)
{
	return x.julianDayNumber() == y.julianDayNumber();
}



} // namespace muduo



#endif