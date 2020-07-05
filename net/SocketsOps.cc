#include "net/SocketsOps.h"

#include "base/Types.h"
#include "base/Logging.h"
#include "net/Endian.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/uio.h>    //readv
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

namespace
{

typedef struct sockaddr SA;

#if VALGRIND || defined (NO_ACCEPT4)
void setNonBlockAndCloseOnExec(int sockfd)
{
	// non-block
	int flags = ::fcntl(sockfd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	int ret = ::fcntl(sockfd, F_SETFL, flags);
	// FIXME check

	// close-on-exec
	flags = ::fcntl(sockfd, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	ret = ::fcntl(sockfd, F_SETFD, flags);
	// FIXME check

	(void)ret;
}
#endif
}   // namespace


/**
 * sockaddr* 和 sockaddr_in* 与 sockaddr_in6* 之间的转化
 * 我们知道的是，无论是 shckaddr 还是 sockaddr_in 还是 sockaddr_in6 的前面的四个字节的内容是一样的
 * 前两个字节 IP 类型，后面两个字节是 端口号
*/
const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in6* addr)
{
  return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in* sockets::sockaddr_in_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in6* sockets::sockaddr_in6_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
}


int sockets::createNonblockingOrDie(sa_family_t family)
{
#if VALGRIND
	int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
		LOG_SYSFATAL << "sockets::createNonblockingOrDie";
	}

	setNonBlockAndCloseOnExec(sockfd);
#else
	int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  /**
   * SOCK_CLOEXEC : 我们在父进程中使用 fork() 来生成子线程的时候，
   * 子进程采用写时复制的方式来获得父进程的数据空间，栈，堆上的数据，这当然包括了文件描述符。
   * 但是一般的情况下我们在子进程中会调用 exec 来加载新的程序，那么自然那些来自于父进程的 文件描述符就没有用了，但是我们需要
   * 关闭这些描述符。所以一般的情况下，在子进程中执行 exec 之前，我们会手动关闭这些描述符，但是如果父类的描述符太多了，子进程中处理起来
   * 过于复杂，这里就提供了一种机制 close-on-exec ,在执行 exec 的时候，会自动的释放所有的文件描述符
   * 
   * 非阻塞的 socket 在操作的时候比阻塞式的要复杂一些，但是性能可以得到很大的提升
  */
	if (sockfd < 0)
	{
		LOG_SYSFATAL << "socket: createNonblockingOrDie";
	}
#endif
	return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr* addr)
{
	/**
	 * 将文件描述符 sockfd 和指定的 IP 以及 端口进行绑定
	*/
	int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
	if (ret < 0)
	{
		LOG_SYSFATAL << "sockets::bindOrDie";   // 这个类在析构的过程中，会自动调用 abort()
	}
}

void sockets::listenOrDie(int sockfd)
{
	int ret = ::listen(sockfd, SOMAXCONN);
	if (ret < 0)
	{
		LOG_SYSFATAL << "sockets::listenOrDie";
	}
}

/**
 * 一个客户端的连接请求到达，使用 accept 进行处理
*/
int sockets::accept(int sockfd, struct sockaddr_in6* addr)
{
	socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
#if VALGRIND || defined (NO_ACCEPT4)
	int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
	setNonBlockAndCloseOnExec(connfd);
#else
	int connfd = ::accept4(sockfd, sockaddr_cast(addr),
						 &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
	/**
	 * 如果连接成功，返回的套接字 文件描述符
	 * 套接字文件描述符所指向的结构中包含了服务器端的 ip, 端口号，也包含了客户端的ip 和端口号
	 * 因此通过这个文件描述符就可以进行一对一的通信
	 * 服务器在端口上会建立数据队列和连接请求队列，连接请求队列的内容会送往 sockfd(connect()函数所指定的套接字)
	 * 数据队列的内容，操作系统根据 客户端的 ip 和端口号，决定其发往哪一个 connfd
	*/
#endif
	if (connfd < 0)
	{
		/**
		 * 失败情况下的 errno
		*/
		int savedErrno = errno;
		LOG_SYSERR << "Socket::accept"; // 这个错误是不会终止程序的
		switch (savedErrno)
		{
			case EAGAIN: /*当前没有新的连接，你可以在一段时间之后重新尝试  AGAIN*/
			case ECONNABORTED: /*两端已经完成了三次握手的过程。另一端发送了一个 RST 报文，
			 这个错误可以直接忽略， 因为操作系统会完成断开连接的后续工作*/
			case EINTR:
			case EPROTO: // ???
			case EPERM:
			case EMFILE: // per-process lmit of open file desctiptor ???
				// expected errors
				errno = savedErrno;
				break;
			case EBADF:
			case EFAULT:
			case EINVAL:
			case ENFILE:
			case ENOBUFS:
			case ENOMEM:
			case ENOTSOCK:
			case EOPNOTSUPP:
				// unexpected errors
				/**
				 * 上面的这个错误会导致线程的退出
				*/
				LOG_FATAL << "unexpected error of ::accept " << savedErrno;	
				break;
			default:
				LOG_FATAL << "unknown error of ::accept " << savedErrno;
				break;
		}
	}
	return connfd;
}

// 客户端使用 connect ()
int sockets::connect(int sockfd, const struct sockaddr* addr)
{
  	return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
	/**
	 * 非阻塞的情况下，connect 函数不会等到三次握手的过程结束才返回，而是直接返回，返回时，连接的过程可能没有完成
	*/
}


ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
  	return ::read(sockfd, buf, count);
	/**
	 * 如果 套接字是非阻塞的，那么当没有数据可以进行读取的时候，read 会马上返回，不会阻塞等待
	*/
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
  	return ::readv(sockfd, iov, iovcnt);
	/**
	 * read() 将所有的内容全部读取到一个缓冲区 buf 当中
	 * iov 的结构非常的简单，一个指向一段连续空间的指针 + 这个空间的大小
	 * readv() 函数先塞满第一个缓冲区iov[0], 之后再去塞第二个缓冲区，以此类推
	 * 直到所有的数据全部写入到缓冲区中
	 * 
	 * writev() 相反，依次从每一个缓冲区中读出数据
	 * 因此上面的两个函数又被称为 ： 分散读 / 聚集写
	*/
}

/**往套接字中写入数据*/
ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
  	return ::write(sockfd, buf, count); /**未必 count 的数据可以全部写入到 socket 当中*/
	/**
	 * 返回的值，是实际上写入的字节的数量
	*/
}

void sockets::close(int sockfd)
{
	if (::close(sockfd) < 0)
	{
		LOG_SYSERR << "sockets::close";	// 不终止程序
	}
}

void sockets::shutdownWrite(int sockfd)
{
	/**
	 * 我们知道 TCP 是全双工的，所以 shutdownWrite() 可以进行半关闭，关闭的过程是不可逆的
	 * 那么 shutdownWrite() 和 close() 有什么区别，close() 只关闭当前的线程/进程和 套接字之间的
	 * 连接，但是多个线程可以连接到同一个 套接字上，其中的一个关闭，并不影响其他的
	 * 但是 shutdownWrite() 的关闭是连接层面的，所有使用这个套接字的进程/线程都无法再使用相应的功能
	 * 0 ： 关闭读
	 * 1 : 关闭写
	 * 2 ： 关闭读写
	*/
	if (::shutdown(sockfd, SHUT_WR) < 0)
	{
		LOG_SYSERR << "sockets::shutdownWrite";
	}
}


void sockets::toIpPort(char* buf, size_t size,
                       const struct sockaddr* addr)
{
	toIp(buf,size, addr);
	size_t end = ::strlen(buf);
	const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
	uint16_t port = sockets::networkToHost16(addr4->sin_port);	/*网络字节序转化为主机字节序*/
	assert(size > end);
	snprintf(buf+end, size-end, ":%u", port); 	/**ip:port : 192.168.1.1:8080*/
}

/**将 2 进制的 ip 地址转化为 10 进制的 ip 地址*/
void sockets::toIp(char* buf, size_t size,
                   const struct sockaddr* addr)
{
	if (addr->sa_family == AF_INET)
	{
		assert(size >= INET_ADDRSTRLEN);
		const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
		::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
	}
	else if (addr->sa_family == AF_INET6)
	{
		assert(size >= INET6_ADDRSTRLEN);
		const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
		::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
	}
}

void sockets::fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in* addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = hostToNetwork16(port);
	if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
	{
		LOG_SYSERR << "sockets::fromIpPort";
	}
}

void sockets::fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in6* addr)
{
	addr->sin6_family = AF_INET6;
	addr->sin6_port = hostToNetwork16(port);
	if (::inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0)	/*将 ip 中的十进制转化为二进制存入 addr->sin6_addr*/
	{
		LOG_SYSERR << "sockets::fromIpPort";
	}
}

int sockets::getSocketError(int sockfd)
{
	int optval;
	socklen_t optlen = static_cast<socklen_t>(sizeof optval);

	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
	/**
	 * 获取或者设置与某个套接字相关联的选项
	 * int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
	 * sock : 想要查询的套接字
	 * level:选项所在的层
	 * 		SOL_SOCKET : 通用的套接字选项
	 * 		IPPROTO_TCP: TCP 选项
	 * 		IPPROTO_IP : IP 选项
	 * optname : 选项字段的内容非常的多，具体的上网查询结果
	 * optval : 接收查询的数据
	 * optlen : 接收查询的数据的缓冲区的大小
	 * 
	 * setsockopt 的功能和 getsockopt 的功能相反
	*/
	{
		/**
		 * 出错的话，返回错误号
		*/
		return errno;
	}
	else
	{
		return optval;
	}
}

struct sockaddr_in6 sockets::getLocalAddr(int sockfd)
{
	struct sockaddr_in6 localaddr;
	memZero(&localaddr, sizeof localaddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
	if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0)
	/**
	 * 根据 sockfd 描述符获取本地的 ip 地址
	 * getpeername() 则是获得远端的 ip 地址
	 * 在一个没有调用 bind() 的客户程序上，客户在完成连接之后，可以使用 gethostname() 获取系统自动分配的 IP 和端口号
	 * 在一个调用 bind() 但是端口号设置为 0 的客户程序上，在完成连接之后，可以使用 gethostname 获取系统自动分配的端口号
	*/
	{
		LOG_SYSERR << "sockets::getLocalAddr";
	}
	return localaddr;
}


struct sockaddr_in6 sockets::getPeerAddr(int sockfd)
{
	struct sockaddr_in6 peeraddr;
	memZero(&peeraddr, sizeof peeraddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
	if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0)
	/**
	 * 这里应该使用连接套接字，而不是端口监听套接字。端口监听的套接字只用于监听端口
	*/
	{
		LOG_SYSERR << "sockets::getPeerAddr";
	}
	return peeraddr;
}


bool sockets::isSelfConnect(int sockfd)
{
	struct sockaddr_in6 localaddr = getLocalAddr(sockfd);
	struct sockaddr_in6 peeraddr = getPeerAddr(sockfd);
	if (localaddr.sin6_family == AF_INET)
	{
		const struct sockaddr_in* laddr4 = reinterpret_cast<struct sockaddr_in*>(&localaddr);
		const struct sockaddr_in* raddr4 = reinterpret_cast<struct sockaddr_in*>(&peeraddr);
		return laddr4->sin_port == raddr4->sin_port
			&& laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;	/*端口相同，并且 ip 地址也相同*/
	}
	else if (localaddr.sin6_family == AF_INET6)
	{
		return localaddr.sin6_port == peeraddr.sin6_port
			&& memcmp(&localaddr.sin6_addr, &peeraddr.sin6_addr, sizeof localaddr.sin6_addr) == 0;
	}
	else
	{
		return false;
	}
}


