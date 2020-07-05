#ifndef MUDUO_BASE_LOGSTREAM_H
#define MUDUO_BASE_LOGSTREAM_H

#include "base/noncopyable.h"
#include "base/StringPiece.h"
#include "base/Types.h"
#include <assert.h>
#include <string.h> // memcpy

namespace muduo
{

namespace detail
{

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

/**
 * 一个固定大小的 Buffer
*/
template <int SIZE>
class FixedBuffer : noncopyable
{
public:
	FixedBuffer()
		: cur_(data_)
	{
		this->setCookie(cookieStart);	// static 函数可以看作普通函数
	}

	~FixedBuffer()
	{
		this->setCookie(cookieEnd);
	}

	void append(const char* buf, size_t len)
	{
		if (implicit_cast<size_t>(avail() > len))
		{
			memcpy(this->cur_, buf, len);	// 深拷贝
			this->cur_ += len;
		}
	}

	// 查询
	const char* data() const { return this->data_; }
	int length() const { return this->cur_ - this->data_; }
	char* current() { return this->cur_; }
	int avail() { return static_cast<int>(this->end() - this->cur_); }

	// 操作
	void add(size_t len) { this->cur_ += len; }
	void reset() { this->cur_ = this->data_; }
	void bzero() {memZero(this->data_, sizeof this->data_); }

	// 
	const char* debugString();
	void setCookie(void (*cookie)() ) { cookie_ = cookie; }	// 给函数指针赋值

	string toString() const {return string(data_, length()); }	// 深拷贝
	StringPiece toStringPiece() const { return StringPiece(data_, length()); }	// 浅拷贝
private:
	void 	(*cookie_)(); // 一个无参，无返回的函数指针
	char 	data_[SIZE];
	char* 	cur_;

	const char* end() { return data_ + sizeof data_; }
	static void cookieStart();
	static void cookieEnd();
};

} // namespace detail

/**
 * 日志流
 * 主要的内容就是重载类函数 operator <<() 实现输入
*/
class LogStream : noncopyable
{
typedef LogStream self;
public:
	typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;

	self& operator << (bool b)
	{
		this->buffer_.append(b ? "1" : "0", 1);
		return *this;
	}

	self& operator<<(short);
	self& operator<<(unsigned short);
	self& operator<<(int);
	self& operator<<(unsigned int);
	self& operator<<(long);
	self& operator<<(unsigned long);
	self& operator<<(long long);
	self& operator<<(unsigned long long);

	self& operator<<(const void*);
  	self& operator<<(double);

	self& operator<< (float f)
	{
		(*this) << static_cast<double>(f);
		return *this;
	}

	self& operator<<(char v)
	{
		this->buffer_.append(&v, 1);
		return *this;
	}

	self& operator<<(const char* str)
	{
		if (str)
		{
			this->buffer_.append(str, strlen(str));
		}
		else 
		{
			this->buffer_.append("(null)", 6);
		}
		return *this;
	}

	self& operator<<(const unsigned char* str)
	{
		return operator<<(reinterpret_cast<const char*>(str));
	}

	self& operator<<(const string& v)
	{
		buffer_.append(v.c_str(), v.size());
		return *this;
	}

	self& operator<<(const StringPiece& v)
	{
		buffer_.append(v.data(), v.size());
		return *this;
	}

	self& operator<<(const Buffer& v)
	{
		*this << v.toStringPiece();
		return *this;
	}

	void append(const char* data, int len){ buffer_.append(data, len); }
	const Buffer& buffer() { return buffer_; }
	void resetBuffer() { buffer_.reset(); }

private:
	void staticCheck();

	template <typename T>
	void formatInteger(T);

	Buffer buffer_;

	static const int kMaxNumericSize = 32;
};

/**
 * 用于格式化存储
 * e.g:  Fmt fmt("%d days", 10);
*/
class Fmt : public noncopyable
{
public:
	template <typename T>
	Fmt(const char* fmt, T val);

	const char* data() const { return buf_; }
	int length() const { return length_; }

private:
	char	buf_[32];
	int		length_;
};

inline LogStream& operator << (LogStream& s, const Fmt& fmt){
	s.append(fmt.data(), fmt.length());
	return s;
}

string formatSI(int64_t n);

string formatIEC(int64_t n);

} // namespace muduo




#endif