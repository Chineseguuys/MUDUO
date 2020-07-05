#include "base/Logging.h"

#include "base/CurrentThread.h"
#include "base/Timestamp.h"
#include "base/TimeZone.h"


#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

namespace muduo
{

__thread char t_errnobuf[512];
__thread char t_time[64];
__thread time_t t_lastSecond;

const char* strerror_tl(int savedErrno)
{
	return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

Logger::LogLevel initLogLevel()
{
	/**
	 * getenv(str) 搜索由 str 给定的环境字符串，并返回相应的值，没有找到返回 null
	 */
	::setenv("MUDUO_LOG_TRACE", "true", 1);
	if (::getenv("MUDUO_LOG_TRACE"))
		{
			printf("MUDUO_LONG_TRACE\n");
			return Logger::TRACE;
		}
	else if (::getenv("MUDUO_LOG_DEBUG"))
		{
			printf("MUDUO_LOG_DEBUG\n");
			return Logger::DEBUG;
		}
	else
	{
		printf("MODUO_LOG_INFO\n");
		return Logger::INFO;
	}
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
	"TRACE ",
	"DEBUG ",
	"INFO  ",
	"WARN  ",
	"ERROR ",
	"FATAL ",
};

// 帮助类在编译期间就得知字符串的长度信息
class T{
public:
	T (const char* str, unsigned len)
		: str_(str),
		len_(len)
	{
		assert(strlen(str) == len_);
	}

	const char*		str_;
	const unsigned	len_;
};

inline LogStream& operator<<(LogStream& s, T v)
{
	s.append(v.str_, v.len_);
	return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
	s.append(v.data_, v.size_);
	return s;
}

/**
 * 默认从标准控制台进行输出
*/
void defaultOutput(const char* msg, int len)
{
	size_t n = fwrite(msg, 1, len, stdout);
	//FIXME check n
	(void)n;
}

/**
 * 默认刷新流，刷新控制台数据流
*/
void defaultFlush()
{
  fflush(stdout);
}

Logger::OutputFunc	g_output = defaultOutput;
Logger::FlushFunc	g_flush = defaultFlush;
TimeZone			g_logTimeZone;
} // namespace muduo

using namespace muduo;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
	:	time_(Timestamp::now()),	// 微秒表示的当前时间
		stream_(),
		level_(level),
		line_(line),
		basename_(file)
{
	formatTime();
	CurrentThread::tid();
	stream_ << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
	stream_ << T(LogLevelName[level], 6);
	if (savedErrno != 0)
	{
		stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
	}
}


void Logger::Impl::formatTime(){
	int64_t microSecondsSinceEpoch = time_.microSecondsSinceEpoch();
	/**
	 * 微秒数转化为 秒 + 微秒
	 * 1000320 us = 1s + 320 us
	*/
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicorSecondsPerSecond);
	int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicorSecondsPerSecond);
	if (seconds != t_lastSecond)
	{
		t_lastSecond = seconds;
		struct tm tm_time;
		if (g_logTimeZone.valid())
		{
		tm_time = g_logTimeZone.toLocalTime(seconds);
		}
		else
		{
		::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime
		}

		int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
			tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		assert(len == 17); (void)len;
	}

	if (g_logTimeZone.valid())
	{
		Fmt us(".%06d ", microseconds);
		assert(us.length() == 8);
		stream_ << T(t_time, 17) << T(us.data(), 8);
	}
	else
	{
		Fmt us(".%06dZ ", microseconds);
		assert(us.length() == 9);
		stream_ << T(t_time, 17) << T(us.data(), 9);
	}
}

void Logger::Impl::finish()
{
  	stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line)	/*SourceFile 浅拷贝*/
	:	impl_(LogLevel::INFO, 0, file, line)	/*0 表示没有错误*/
{
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
  : 	impl_(level, 0, file, line)
{
  	impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level)
  : 	impl_(level, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, bool toAbort)
  : impl_(toAbort ? FATAL : ERROR, errno, file, line)	/*errno : C 中的一个全局的变量，用于记录错误号*/
{
}

Logger::~Logger()
{
	impl_.finish();
	const LogStream::Buffer& buf(stream().buffer());
	g_output(buf.data(), buf.length());		/*默认从控制台输出 buf 中的信息*/
	if (impl_.level_ == FATAL)	// 如果等级为致命错误，终止程序
	{
		g_flush();
		abort();	// 如果 log 的级别是 FATAL，那么会终止程序
	}
}

void Logger::setLogLevel(Logger::LogLevel level)
{
  	g_logLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
  	g_output = out;
}

void Logger::setFlush(FlushFunc flush)
{
  	g_flush = flush;
}

void Logger::setTimeZone(const TimeZone& tz)
{
  	g_logTimeZone = tz;
}