#ifndef MUDUO_NET_BUFFER_H
#define MUDUO_NET_BUFFER_H

#include "base/copyable.h"
#include "base/StringPiece.h"
#include "base/Types.h"

#include "net/Endian.h"


#include <algorithm>
#include <vector>

#include <assert.h>
#include <string.h>

namespace muduo
{

namespace net
{
/**
 +-------------------+------------------+------------------+
 | discardable bytes |  readable bytes  |  writable bytes  |
 | 已经读取的内容缓冲区 | 可以读取的内容      |可以写入的空间      |
 +-------------------+------------------+------------------+
 |                   |                  |                  |
 0      <=      readerIndex   <=   writerIndex    <=    capacity
————————————————
Buffer 仿 Netty ChannelBuffer 的 buffer class，数据的读写通过 buffer 进行，用户的代码不需要调用 read() 和 write() ，只需要处理收到的数据和
准备好要发送的数据
*/
class Buffer : public muduo::copyable{
private:
	std::vector<char>   buffer_;
	size_t              reader_index_;
	size_t              writer_index_;

	static const char kCRLF[];

public:
	static const size_t kCheapPrepend = 8;
	static const size_t kInitialSize = 1024;

	explicit Buffer(size_t initialSize = kInitialSize)
		:   buffer_(kCheapPrepend + initialSize),
			reader_index_(kCheapPrepend),
			writer_index_(kCheapPrepend)
	{
		/**初始化之后的检查*/
		assert(this->readableBytes() == 0);
		assert(this->writableBytes() == initialSize);
		assert(this->prependableBytes() == kCheapPrepend);
	}

	// 编译器自动生成的拷贝构造函数，移动构造函数和 拷贝赋值 operator= 可以很好的工作

	void swap(Buffer& rhs)
	{
		/**移动语义*/
		this->buffer_.swap(rhs.buffer_);
		std::swap(this->reader_index_, rhs.reader_index_);
		std::swap(this->writer_index_, rhs.writer_index_);
	}

	size_t readableBytes() const
	{ return writer_index_ - reader_index_; }

	size_t writableBytes() const
	{ return buffer_.size() - reader_index_; }

	size_t prependableBytes() const
	{ return reader_index_; }

	const char* peek() const 
	/*读区间开始的位置*/
	{
		return this->begin() + this->reader_index_;
	}

	const char* findCRLF() const 
	{
		const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF+2);
		return crlf == this->beginWrite() ? NULL : crlf; 
	}

	const char* findCRLF(const char* start) const
	{
		assert(peek() <= start);
		assert(start <= beginWrite());
		// FIXME: replace with memmem()?
		const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF+2);
		return crlf == beginWrite() ? NULL : crlf;
	}

	const char* findEOL() const {
		const void* eol = memchr(this->peek(), '\n', this->readableBytes());
		return static_cast<const char*>(eol);
	}

	const char* findEOL(const char* start) const
	{
		assert(peek() <= start);
		assert(start <= beginWrite());
		const void* eol = memchr(start, '\n', beginWrite() - start);
		return static_cast<const char*>(eol);
	}

	void retrieve(size_t len)
	{
		assert(len <= readableBytes());
		if (len < readableBytes())
		{
			this->reader_index_ += len;
		}
		else
		{
			this->retrieveAll();
		}
	}

	void retrieveUntil(const char* end)
	{
		assert(peek() <= end);	/*终点不能小于当前可读区域的起点*/
		assert(end <= beginWrite());	/*终点也不可以超过写区域的起点*/
		retrieve(end - peek());
	}

	void retrieveInt64()
	{
		retrieve(sizeof(int64_t));
	}

	void retrieveInt32()
	{
		retrieve(sizeof(int32_t));
	}

	void retrieveInt16()
	{
		retrieve(sizeof(int16_t));
	}

	void retrieveInt8()
	{
		retrieve(sizeof(int8_t));
	}

	void retrieveAll()
	{
		this->reader_index_ = kCheapPrepend;
		this->writer_index_ = kCheapPrepend;
	}

	std::string retrieveAllAsString()
	{
		return retrieveAsString(readableBytes());
	}

	std::string retrieveAsString(size_t len)
	{
		assert(len <= readableBytes());
		std::string result(peek(), len);
		retrieve(len);
		return result;
	}

	muduo::StringPiece toStringPiece() const 
	{
		return muduo::StringPiece(peek(), this->readableBytes());	/*浅拷贝*/
	}

	void append(const muduo::StringPiece& str)
	{
		append(str.data(), str.size());
	}

	void append(const char* /*restrict*/ data, size_t len)
	{
		ensureWritableBytes(len);
		std::copy(data, data+len, beginWrite());
		hasWritten(len);
	}

	void append(const void* /*restrict*/ data, size_t len)
	{
		append(static_cast<const char*>(data), len);
	}

	/**保证有大于 len 的空间可写*/
	void ensureWritableBytes(size_t len)
	{
		if (writableBytes() < len)
		{
			makeSpace(len);
		}
		assert(writableBytes() >= len);
	}

	char* beginWrite()
	/*写区间开始的位置*/
	{ return begin() + writer_index_; }

	const char* beginWrite() const
	{ return begin() + writer_index_; }

	void hasWritten(size_t len)
	{
		assert(len < this->writableBytes());
		this->writer_index_ += len;
	}

	void unwrite(size_t len)
	{
		assert(len < this->readableBytes());
		this->writer_index_ -= len;
	}

	// append
	///
	/// Append int64_t using network endian
	///
	void appendInt64(int64_t x)
	{
		int64_t be64 = muduo::net::sockets::hostToNetwork64(x);
		append(&be64, sizeof be64);
	}

	///
	/// Append int32_t using network endian
	///
	void appendInt32(int32_t x)
	{
		int32_t be32 = muduo::net::sockets::hostToNetwork32(x);
		append(&be32, sizeof be32);
	}

	void appendInt16(int16_t x)
	{
		int16_t be16 = muduo::net::sockets::hostToNetwork16(x);
		append(&be16, sizeof be16);
	}

	void appendInt8(int8_t x)
	{
		append(&x, sizeof x);
	}

	// read
	///
	/// Read int64_t from network endian
	///
	/// Require: buf->readableBytes() >= sizeof(int32_t)
	int64_t readInt64()
	{
		int64_t result = peekInt64();
		retrieveInt64();
		return result;
	}

	/// Read int32_t from network endian
	///
	/// Require: buf->readableBytes() >= sizeof(int32_t)
	int32_t readInt32()
	{
		int32_t result = peekInt32();
		retrieveInt32();
		return result;
	}

	int16_t readInt16()
	{
		int16_t result = peekInt16();
		retrieveInt16();
		return result;
	}

	int8_t readInt8()
	{
		int8_t result = peekInt8();
		retrieveInt8();
		return result;
	}


	// peek
	///
	/// Peek int64_t from network endian
	///
	/// Require: buf->readableBytes() >= sizeof(int64_t)
	int64_t peekInt64() const
	{
		assert(readableBytes() >= sizeof(int64_t));
		int64_t be64 = 0;
		::memcpy(&be64, peek(), sizeof be64);
		return muduo::net::sockets::networkToHost64(be64);
	}

	///
	/// Peek int32_t from network endian
	///
	/// Require: buf->readableBytes() >= sizeof(int32_t)
	int32_t peekInt32() const
	{
		assert(readableBytes() >= sizeof(int32_t));
		int32_t be32 = 0;
		::memcpy(&be32, peek(), sizeof be32);
		return muduo::net::sockets::networkToHost32(be32);
	}

	int16_t peekInt16() const
	{
		assert(readableBytes() >= sizeof(int16_t));
		int16_t be16 = 0;
		::memcpy(&be16, peek(), sizeof be16);
		return muduo::net::sockets::networkToHost16(be16);
	}

	int8_t peekInt8() const
	{
		assert(readableBytes() >= sizeof(int8_t));
		int8_t x = *peek();
		return x;
	}

	// prepend
	///
	/// Prepend int64_t using network endian
	///
	void prependInt64(int64_t x)
	{
		int64_t be64 = muduo::net::sockets::hostToNetwork64(x);
		prepend(&be64, sizeof be64);
	}

	///
	/// Prepend int32_t using network endian
	///
	void prependInt32(int32_t x)
	{
		int32_t be32 = muduo::net::sockets::hostToNetwork32(x);
		prepend(&be32, sizeof be32);
	}

	void prependInt16(int16_t x)
	{
		int16_t be16 = muduo::net::sockets::hostToNetwork16(x);
		prepend(&be16, sizeof be16);
	}

	void prependInt8(int8_t x)
	{
		prepend(&x, sizeof x);
	}

	/**
	 * 在可读区域的前面插入一段数据（覆盖原来位置的数据）
	 * read_index_ 同时向前移动
	*/
	void prepend(const void* data, size_t len)
	{
		assert(len <= prependableBytes());
		reader_index_ -= len;
		const char* d = static_cast<const char*>(data);
		std::copy(d, d + len, begin() + reader_index_);
	}

	/**将 vector 收缩到合适的大小 reserve 表示预留的空间大小*/
	void shrink(size_t reserve)
	{
		// FIXME: use vector::shrink_to_fit() in C++ 11 if possible.
		Buffer other;
		other.ensureWritableBytes(readableBytes()+reserve);
		other.append(toStringPiece());
		swap(other);
	}

	size_t internalCapacity() const
	{
		return buffer_.capacity();
	}

	ssize_t readfd(int fd, int* savedErrno);

private:
	char* begin() { return &*buffer_.begin(); }
	const char* begin() const { return &*buffer_.begin(); }

	void makeSpace(size_t len)
	{
		if (writableBytes() + prependableBytes() < len + kCheapPrepend)
		/**
		 * 如果写区域剩余的空间 + 已读区域的空间 - kCheapPrepend 保留空间 < 当前需要的空间大小(len)
		 * 那么我们就需要重新进行内存的分配
		*/
		{
			buffer_.resize(writer_index_ + len);
		}
		else 
		{
			/**
			 * 反之，我们可以利用前面的已读区域的空间，扩展我们写空间的大小
			*/
			assert(kCheapPrepend < reader_index_);
			size_t readable = readableBytes();
			std::copy(	begin() + reader_index_ , 
						begin() + writer_index_,
						begin() + kCheapPrepend);
			reader_index_ = kCheapPrepend;
			writer_index_ = reader_index_ + readable;
			assert(readable == this->readableBytes());
		}
	}
};

} // namespace net


} // namespace mudup


#endif