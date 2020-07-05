#ifndef MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H


#include "base/Types.h"
#include "base/copyable.h"

#include <boost/operators.hpp>

namespace muduo
{

/**
 * 世界标准时间的时间戳，单位是微秒
 * 
 * 这个类不可更改
 * 
 * 这个类推荐用传值的方式进行传递
*/
class Timestamp :   public muduo::copyable,
                    public boost::equality_comparable<Timestamp>,
                    public boost::less_than_comparable<Timestamp>
{
public:
    Timestamp() : microSecondsSinceEpoch_(0) {}

    explicit Timestamp(int64_t microSecondsSinceEpochArg)
        : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
    {}

    void swap(Timestamp& that)
    {
        std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }

    // 以一定的格式输出时间
    string toString() const;

    string toFormattedString (bool showMicroseconds = true) const;

    bool valid() const  { return microSecondsSinceEpoch_ > 0; }
    /**
     * 获取时间的数值
    */
    int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
    time_t secondsSineEpoch() const
    {
        return static_cast<time_t>(microSecondsSinceEpoch_ / kMicorSecondsPerSecond);
    }

    // 获取当前的时间
    static Timestamp now();
    static Timestamp invalid()
    {
        return Timestamp();
    }

    /**
     * 根据秒数时间返回 微秒 时间
    */
    static Timestamp fromUnixTime(time_t t)
    {
        return fromUnixTime(t, 0);
    }

    static Timestamp fromUnixTime(time_t t, int microseconds)
    {
        return Timestamp(static_cast<int64_t>(t) * kMicorSecondsPerSecond + microseconds);
    }

    static const int kMicorSecondsPerSecond =  1000 * 1000; // 每一秒包含的微秒数

private:
    int64_t microSecondsSinceEpoch_;    // 从公元纪年以来的微秒数
    // 完全够用
};

// 时间戳的比较，算术运算

inline bool operator < (Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator == (Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicorSecondsPerSecond;
}

inline Timestamp addTime(Timestamp times, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicorSecondsPerSecond);
    return Timestamp(times.microSecondsSinceEpoch() + delta);
}


} // namespace muduo




#endif