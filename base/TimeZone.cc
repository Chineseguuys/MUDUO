#include "base/TimeZone.h"
#include "base/noncopyable.h"
#include "base/Date.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include <assert.h>
//#define _BSD_SOURCE
#include <endian.h>

#include <stdint.h>
#include <stdio.h>


namespace muduo
{
namespace detail
{

struct Transition
{
	time_t		gmtime;	// 世界标准时
	time_t		localtime;	// 本地时间
	int			localtimeIdx;	// 时区的编号，比如说，中国为东8区， +8

	Transition(time_t t, time_t l, int localIdx)
		: gmtime(t), localtime(l), localtimeIdx(localIdx)
	{ }
};

/**
 * 比较 UTC 世界标准时 或者 本地时间
 * 
 * 世界一共分为 24 个时区，每个时区拥有一个自己的本地时间
 * 每一个时区之间相差 1 小时
 * 
 * 仿函数
*/
struct Comp
{
	bool compareGmt;	// 是比较世界标准时，还是比较本地时间

	Comp(bool gmt)
		: compareGmt(gmt)
	{}

	bool operator() (const Transition& lhs, const Transition& rhs) const
	{
		if (compareGmt)
			return lhs.gmtime < rhs.gmtime;
		else 
			return lhs.localtime < lhs.localtime;
	}

	bool equal(const Transition& lhs, const Transition& rhs) const
	{
		if (compareGmt)
			return lhs.gmtime == rhs.gmtime;
		else 
			return lhs.localtime == rhs.localtime;
	}
};

struct Localtime
{
	time_t	gmtOffset;		// 世界日期变更线的东部地区的正秒数，变更线的西部地区的负秒数
	// 
	bool 	isDst;	//是否时夏令时
	int 	arrbIdx;	// 时区的索引

	Localtime(time_t offset, bool dst, int arrb)
		: gmtOffset(offset), isDst(dst), arrbIdx(arrb)
	{}
};

inline void fillHMS(unsigned seconds, struct tm* utc)
{
	utc->tm_sec = seconds % 60;
	unsigned minutes = seconds / 60;
	utc->tm_min = minutes % 60;
	utc->tm_hour = minutes / 60;
}


} // namespace detail

const int kSecondsPerDay = 24*60*60;

} // namespace muduo

using namespace muduo;
using namespace std;

struct TimeZone::Data
{
	vector<detail::Transition> 	transitions;
	vector<detail::Localtime>	localtimes;
	vector<string> 				names;
	string 						abbreviation;	// 缩写
};

namespace muduo
{

namespace detail
{

/**
 * 包装了一个文件读取过程，三种读取文件的方式
 * 1 二进制读取
 * 2 以 int 类型读取
 * 3 以无符号 char 类型读取
*/
class File : noncopyable
{
public:
	File (const char* file)
		:fp_(::fopen(file, "rb"))
	{}

	~File()
	{
		if (this->fp_)
			::fclose(fp_);
	}

	bool valid() const { return this->fp_; }	// 是否获得了有效的文件描述符

	string readBytes(int n)
	{
		char buf[n];
		ssize_t nr = ::fread(buf, 1, n, this->fp_);
		if (nr != n)
			throw logic_error("No Enough Data");
		return string(buf, n);
	}

	int32_t readInt32()
	{
		int32_t x;
		ssize_t nr = ::fread(&x, 1, sizeof(int32_t), this->fp_);
		if (nr != sizeof(int32_t))
			throw logic_error("Bad int32 Data");
		return be32toh(x);
	}


	uint8_t readUInt8()
	{
		uint8_t x;
		ssize_t nr = ::fread(&x, 1, sizeof(uint8_t), this->fp_);
		if (nr != sizeof(uint8_t))
			throw logic_error("Bad Uint8 Data");
		return x;
	}

private:
	FILE* fp_;
};

/**
 * 从文件内容创建一个 TimeZone 的内容
*/
bool readTimeZoneFile(const char* file, TimeZone::Data* data)
{
	File f(file);
	if (f.valid())
	{
		/**
		 * TimeZoneFile 是一个格式化的文件
		*/
		try
		{
			string head = f.readBytes(4);
			if (head != "TZif")
				throw logic_error("bad Head");
			string version = f.readBytes(1);
			f.readBytes(15);

			int32_t isgmtcnt = f.readInt32();
			int32_t isstdcnt = f.readInt32();
			int32_t leapcnt = f.readInt32();
			int32_t timecnt = f.readInt32();
			int32_t typecnt = f.readInt32();
			int32_t charcnt = f.readInt32();

			vector<int32_t> trans;
			vector<int>		localtimes;

			trans.reserve(timecnt);
			for (int i = 0; i < timecnt; ++i)
				trans.push_back(f.readInt32());	// 世界标准时间

			for (int i = 0; i < timecnt; ++i)
			{
				uint8_t local = f.readUInt8();
				localtimes.push_back(local);	// 时区区号
			}

			for (int i = 0; i < typecnt; ++i)
			{
				uint32_t gmtoff = f.readInt32();
				uint8_t isdst = f.readInt32();
				uint8_t abbrind = f.readUInt8();

				data->localtimes.push_back(Localtime(gmtoff, isdst, abbrind));
			}

			for (int i =0; i < timecnt; ++i)
			{
				int localIdx = localtimes[i];
				time_t localtime = trans[i] + data->localtimes[localIdx].gmtOffset;	// 世界标准时 + 时区差值
				data->transitions.push_back(Transition(trans[i], localtime, localIdx));
				// 世界标准时  / 本地时间  / 地区区号
			}

			data->abbreviation = f.readBytes(charcnt);
			
			for (int i = 0; i < leapcnt; ++i)
			{
				//int32_t leaptime = f.readInt32();
				//int32_t cumleap = f.readInt32();
			}
			(void) isstdcnt;
			(void) isgmtcnt;
		}
		catch(logic_error& e)
		{
			fprintf(stderr, "%s\n", e.what());
		}
			
	}
	return true;
}

const Localtime* findLocaltime(const TimeZone::Data& data, Transition trans, Comp comp)
{
	const Localtime* local = NULL;

	if (data.transitions.empty() || comp(trans, data.transitions.front()))
	{
		local = &data.localtimes.front();
	}
	else 
	{
		vector<Transition>::const_iterator transI = lower_bound(
			data.transitions.begin(),
			data.transitions.end(),
			trans,
			comp
		);

		if (transI != data.transitions.end())
		{
			if (!comp.equal(trans, *transI))
			{
				assert(transI != data.transitions.begin());
				--transI;
			}
			local = &data.localtimes[transI->localtimeIdx];
		}
		else 
		{
			local = &data.localtimes[data.transitions.back().localtimeIdx];
		}
	}
	return local;	// 返回的是一个类的对象的指针
}



}	// namespace details
}	// namespace muduo

TimeZone::TimeZone(const char* zonefile)
	: data_(new TimeZone::Data)	// 这个数据成员保存在 shared_ptr<>() 当中
{
	if (!detail::readTimeZoneFile(zonefile, this->data_.get()))
	{
		this->data_.reset();
	}
}

TimeZone::TimeZone(int eastOfUtc, const char* tzname)
	: data_(new TimeZone::Data)
{
	data_->localtimes.push_back(detail::Localtime(eastOfUtc, false, 0));
	data_->abbreviation = tzname;
}

struct tm TimeZone::toLocalTime(time_t seconds) const
{
	struct tm localTime;
	muduo::memZero(&localTime, sizeof(localTime));

	assert(this->data_ != NULL);
	const Data& data(*data_);

	detail::Transition sentry(seconds, 0, 0);
	const detail::Localtime* local = detail::findLocaltime(data, sentry, detail::Comp(true));

	if (local)
	{
		time_t localSeconds = seconds + local->gmtOffset;
		::gmtime_r(&localSeconds, &localTime);
		localTime.tm_isdst = local->isDst;	// 是否使用夏令时
		localTime.tm_gmtoff = local->gmtOffset;	// 比世界标准时提前的秒数
		localTime.tm_zone = &data.abbreviation[local->arrbIdx];
	}

	return localTime;
}

time_t TimeZone::fromLocalTime(const struct tm& localTm) const
{
	assert(data_ != NULL);
	const Data& data(*data_);

	struct tm tmp = localTm;
	time_t seconds = ::timegm(&tmp); // FIXME: toUtcTime 本地时间（和 Epoc 相隔的秒数）
	detail::Transition sentry(0, seconds, 0);
	const detail::Localtime* local = findLocaltime(data, sentry, detail::Comp(false));
	if (localTm.tm_isdst)	// 先不管夏令时，夏令时的规则十分的复杂，不同的国家和地区的规则不一样
	{
		struct tm tryTm = toLocalTime(seconds - local->gmtOffset);
		if (!tryTm.tm_isdst
			&& tryTm.tm_hour == localTm.tm_hour
			&& tryTm.tm_min == localTm.tm_min)
		{
		// FIXME: HACK
		seconds -= 3600;
		}
	}
	return seconds - local->gmtOffset;	// 本地时间 - 和世界时间变更线时间的差值
}


struct tm TimeZone::toUtcTime(time_t secondsSinceEpoch, bool yday)
{
	struct tm utc;
	memZero(&utc, sizeof(utc));
	utc.tm_zone = "GMT";
	int seconds = static_cast<int>(secondsSinceEpoch % kSecondsPerDay);
	int days = static_cast<int>(secondsSinceEpoch / kSecondsPerDay);
	if (seconds < 0)
	{
		seconds += kSecondsPerDay;
		--days;
	}
	detail::fillHMS(seconds, &utc);	//将时分秒的信息进行填写
	Date date(days + Date::kJulianDayOf1970_01_01);	// 只包含年月日
	Date::YearMonthDay ymd = date.yearMonthDay();
	utc.tm_year = ymd.year - 1900;
	utc.tm_mon = ymd.month - 1;
	utc.tm_mday = ymd.day;
	utc.tm_wday = date.weekDay();

	if (yday)
	{
		Date startOfYear(ymd.year, 1, 1);
		utc.tm_yday = date.julianDayNumber() - startOfYear.julianDayNumber();
		/*这个日期时当年的第多少天*/
	}
	return utc;
}

time_t TimeZone::fromUtcTime(const struct tm& utc)
{
	return fromUtcTime(utc.tm_year + 1900, utc.tm_mon + 1, utc.tm_mday,
						utc.tm_hour, utc.tm_min, utc.tm_sec);
}

time_t TimeZone::fromUtcTime(int year, int month, int day,
                             int hour, int minute, int seconds)
{
	Date date(year, month, day);
	int secondsInDay = hour * 3600 + minute * 60 + seconds;
	time_t days = date.julianDayNumber() - Date::kJulianDayOf1970_01_01;
	return days * kSecondsPerDay + secondsInDay;
}