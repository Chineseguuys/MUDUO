#include "net/InetAddress.h"

#include "net/Endian.h"
#include "base/Logging.h"
#include "net/SocketsOps.h"

#include <netdb.h>
#include <netinet/in.h>

// INADDR_ANY use (type)value casting.
#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInaddrAny = INADDR_ANY; /***0x00000000    0.0.0.0*/
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK; /**0x7f000001    127.0.0.1 */
#pragma GCC diagnostic error "-Wold-style-cast"


//     /* Structure describing an Internet socket address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

//     struct sockaddr_in6 {
//         sa_family_t     sin6_family;   /* address family: AF_INET6 */
//         uint16_t        sin6_port;     /* port in network byte order */
//         uint32_t        sin6_flowinfo; /* IPv6 flow information */
//         struct in6_addr sin6_addr;     /* IPv6 address */
//         uint32_t        sin6_scope_id; /* IPv6 scope-id */
//     };

using namespace muduo;
using namespace muduo::net;


static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in6),
              "InetAddress is same size as sockaddr_in6");
/**
 * 这个是可以得到保证的，从上面的 sockaddr_in 和 sockaddr_in6 的结构我们就可以看出来
*/
static_assert(offsetof(sockaddr_in, sin_family) == 0, "sin_family offset 0");
static_assert(offsetof(sockaddr_in6, sin6_family) == 0, "sin6_family offset 0");
static_assert(offsetof(sockaddr_in, sin_port) == 2, "sin_port offset 2");
static_assert(offsetof(sockaddr_in6, sin6_port) == 2, "sin6_port offset 2");


InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6)
{
    static_assert(offsetof(InetAddress, addr6_) == 0, "addr6_ offset 0");
    static_assert(offsetof(InetAddress, addr_) == 0, "addr_ offset 0");

    if (ipv6)
    {
        muduo::memZero(&addr6_, sizeof addr6_);
        addr6_.sin6_family = AF_INET6;
        ::in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = sockets::hostToNetwork16(port);
    }
    else 
    {
        muduo::memZero(&addr_, sizeof addr_);
        addr_.sin_family = AF_INET;
        ::in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
        addr_.sin_addr.s_addr = sockets::hostToNetwork32(ip);
        addr_.sin_port = sockets::hostToNetwork16(port);
    }
}

/**
 * 给定 ip 和端口号，ip 使用的是点10进制 (127.0.0.1)
*/
InetAddress::InetAddress(StringArg ip, uint16_t port, bool ipv6)
{
    if (ipv6)
    {
        memZero(&addr6_, sizeof addr6_);
        sockets::fromIpPort(ip.c_str(), port, &addr6_);
    }
    else
    {
        memZero(&addr_, sizeof addr_);
        sockets::fromIpPort(ip.c_str(), port, &addr_);
    }
}

/**
 * 返回字符串的格式 : 192.168.1.1:8080
*/
string InetAddress::toIpPort() const
{
    char buf[64] = "";
    sockets::toIpPort(buf, sizeof buf, getSockAddr());
    return buf;
}

/**
 * 返回字符串的格式 ： 192.168.1.1
*/
string InetAddress::toIp() const
{
    char buf[64] = "";
    sockets::toIp(buf, sizeof buf, getSockAddr());
    return buf;
}

/**
 * 返回网络字节顺序的二进制 ip 地址
 * 只在 ipv4 协议当中使用
*/
uint32_t InetAddress::ipNetEndian() const
{
    assert(family() == AF_INET);
    return addr_.sin_addr.s_addr;
}

/**
 * 返回主机顺序的 port 值
*/
uint16_t InetAddress::toPort() const
{
    return sockets::networkToHost16(portNetEndian());
}

static __thread char t_resolveBuffer[64 * 1024];

/**
 * 根据域名来获取 ip
*/
bool InetAddress::resolve(StringArg hostname, InetAddress* out)
{
    assert(out != NULL);
    struct hostent hent;
    struct hostent* he = NULL;
/**
 *struct hostent
	{
		char *h_name;         //正式主机名
		char **h_aliases;     //主机别名
		int h_addrtype;       //主机IP地址类型：IPV4-AF_INET
		int h_length;		  //主机IP地址字节长度，对于IPv4是四字节，即32位
		char **h_addr_list;	  //主机的IP地址列表 以网络字节顺序存储的二进制值
	};	
	#define h_addr h_addr_list[0]   //保存
*/
    int herrno = 0;
    muduo::memZero(&hent, sizeof(hent));

    int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuffer, sizeof t_resolveBuffer, &he, &herrno);
    /**
     * t_resolveBuffer 是一个临时的缓冲区，用来存储这个函数执行的过程中的各种中间的数据
     * he ： 如果这个函数执行成功，那么 he 应该指向 hent,否则的话， he 应该指向 NULL
     * herrno : 错误返回码
     * 返回值 ： 成功返回 0，失败返回非 0 的值
    */
    if (ret == 0 && he != NULL)
    {
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t)); /**返回的是 32 字节的 ipv4 地址*/
        out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else
    {
        if (ret)
        {
            LOG_SYSERR << "InetAddress::resolve";
        }
        return false;
    }
}

void InetAddress::setScopeId(uint32_t scope_id)
{
    if (family() == AF_INET6)
    {
        addr6_.sin6_scope_id = scope_id;
    }
}