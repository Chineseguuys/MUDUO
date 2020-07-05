#ifndef MUDUO_NET_INETADDRESS_H
#define MUDUO_NET_INETADDRESS_H

#include "base/copyable.h"
#include "base/StringPiece.h"


#include <netinet/in.h>

namespace muduo
{
namespace net
{
namespace sockets
{
/**
 * 	sizeof sockaddr 	 	16
	sizeof sockaddr_in 	 	16
	sizeof sockaddr_in6 	28

	虽然这三个数据结构的大小不是完全的相等，但是并不影响 sockaddr_in* 和 sockaddr_in6* 转化
	为 sockaddr* 类型，因为这三个数据结构的前两个字节都代表了 协议类型，所以根据前面两个字节，可
	以轻松的判断传入的对象究竟是哪一种数据类型
*/
const struct ::sockaddr* sockaddr_cast(const struct ::sockaddr_in6* addr);
} // namespace sockets

/**
 * ipv4 和 ipv6 的一个统一的接口。实现创建和查询一个 sockaddr 的相关的算法
*/
class InetAddress : public muduo::copyable
{
private:
	union
	{
		struct ::sockaddr_in	addr_;
		struct ::sockaddr_in6	addr6_; 
		/**
		 * sockaddr sockaddr_in sockaddr_in6 的前 32bits (四个字节) 的内容是相同的
		 * 前两个字节都表示通信协议的版本 ipv4 or ipv6
		 * 后两个字节都表示端口号
		*/
	};
public:
 	/// Constructs an endpoint with given port number.
 	/// Mostly used in TcpServer listening.
  	explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

  	/// Constructs an endpoint with given ip and port.
  	/// @c ip should be "1.2.3.4"
  	InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);

  	/// Constructs an endpoint with given struct @c sockaddr_in
  	/// Mostly used when accepting new connections
  	explicit InetAddress(const struct sockaddr_in& addr)
    	: addr_(addr)
  	{ }

  	explicit InetAddress(const struct sockaddr_in6& addr)
    	: addr6_(addr)
  	{ }

  	sa_family_t family() const { return addr_.sin_family; }
  	string toIp() const;
  	string toIpPort() const;
  	uint16_t toPort() const;

  	// default copy/assignment are Okay

  	const struct sockaddr* getSockAddr() const { return sockets::sockaddr_cast(&addr6_); }
  	void setSockAddrInet6(const struct sockaddr_in6& addr6) { addr6_ = addr6; }

  	uint32_t ipNetEndian() const;
  	uint16_t portNetEndian() const { return addr_.sin_port; }

  	// resolve hostname to IP address, not changing port or sin_family
  	// return true on success.
  	// thread safe
  	static bool resolve(StringArg hostname, InetAddress* result);
  	// static std::vector<InetAddress> resolveAll(const char* hostname, uint16_t port = 0);

  	// set IPv6 ScopeID
  	void setScopeId(uint32_t scope_id);
};

} // namespace net

} // namespace muduo



#endif