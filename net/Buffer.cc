#include "net/Buffer.h"
#include "net/SocketsOps.h"

#include <errno.h>
#include <sys/uio.h>

using namespace muduo;
using namespace muduo::net;

const char muduo::net::Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;


/**
 * 一次性从 socket 中读取足够的数据，如果当前的 buffer 无法完成存储，则使用
 * 额外的 buf 进行存储，之后再对 buffer 进行扩容
*/
ssize_t Buffer::readfd(int fd, int* savedErrno)
{
	char extrabuf[65536];
	struct iovec vec[2];    /**分散存储*/
	const size_t writable = this->writableBytes();
	vec[0].iov_base = this->begin() + this->writer_index_;	/*可写区域开始的位置*/
	vec[0].iov_len = writable;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof extrabuf;

	const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
	/**
	 * 如果当前的 buffer 当中的空间是足够的，那么我们就不需要额外的空间来存储，
	 * 否则的话，启用 extrabuf
	*/
	const ssize_t n = sockets::readv(fd, vec, iovcnt);
	if (n < 0)
	{
		*savedErrno = errno;
	}
	else if (muduo::implicit_cast<size_t>(n) <= writable)
	{
		this->writer_index_ += n;
	}
	else 
	{
		this->writer_index_ = this->buffer_.size();
		this->append(extrabuf, n - writable);
	}

	return n;	/**返回读取的字节数*/
}