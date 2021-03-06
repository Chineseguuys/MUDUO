#include "net/Socket.h"

#include "base/Logging.h"
#include "net/InetAddress.h"
#include "net/SocketsOps.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>  // snprintf

using namespace muduo;
using namespace muduo::net;

Socket::~Socket()
{
    sockets::close(sockfd_);
}

bool Socket::getTcpInfo(struct tcp_info* tcpi) const
{
    socklen_t len = sizeof(*tcpi);
    memZero(tcpi, len);
    return ::getsockopt(sockfd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
    /**
     * 获取和某个套接字相关联的选项信息
    */
}

bool Socket::getTcpInfoString(char* buf, int len) const
{
    struct tcp_info tcpi;
    bool ok = getTcpInfo(&tcpi);
    if (ok)
    {
        snprintf(buf, len, "unrecovered=%u "
                "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
                "lost=%u retrans=%u rtt=%u rttvar=%u "
                "sshthresh=%u cwnd=%u total_retrans=%u",
                tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
                tcpi.tcpi_rto,          // Retransmit timeout in usec
                tcpi.tcpi_ato,          // Predicted tick of soft clock in usec
                tcpi.tcpi_snd_mss,
                tcpi.tcpi_rcv_mss,
                tcpi.tcpi_lost,         // Lost packets
                tcpi.tcpi_retrans,      // Retransmitted packets out
                tcpi.tcpi_rtt,          // Smoothed round trip time in usec
                tcpi.tcpi_rttvar,       // Medium deviation
                tcpi.tcpi_snd_ssthresh,
                tcpi.tcpi_snd_cwnd,
                tcpi.tcpi_total_retrans);  // Total retransmits for entire connection
    }
    return ok;
}

/**
 * 要么绑定成功，要么打印log 退出整个线程
*/
void Socket::bindAddress(const InetAddress& addr)
{
    sockets::bindOrDie(sockfd_, addr.getSockAddr());
}

void Socket::listen()
{
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peeraddr)
{
    struct sockaddr_in6 addr;
    memZero(&addr, sizeof addr);
    int connfd = sockets::accept(sockfd_, &addr);
    if (connfd >= 0)
    {
        peeraddr->setSockAddrInet6(addr);
    }
    return connfd;
}

/**
 * TCP 是全双工的工作模式，如果使用 close 来关闭 TCP 连接，那么关闭的发起方会同时关闭读端和写端 
 * 但是如果对方还有数据需要发送的时候，数据的传输就直接中断了
 * 
 * shutdown(socket, SHUT_WR / SHUT_RD / SHUT_WRRD)
*/
void Socket::shutdownWrite()
{
    sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on)
{
    /**
     * 为了防止出现 TCP 的粘包问题，我们不希望 Tcp 等待多个数据报，合并为一个大的数据报一起进行发送
    */
    int optval = on ? 1 : 0;
    ::setsockopt(this->sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<size_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(this->sockfd_, SOL_SOCKET, SO_REUSEADDR,
        &optval, static_cast<size_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                            &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on)
    {
        LOG_SYSERR << "SO_REUSEPORT failed.";
        /**
         * reuse port 可能失败的地方是，如果端口已经被一个 socket  A 所使用，并且 A 没有设置 SO_REUSEPORT
         * ，如果 A 处在 TIME_WAIT 状态的话，当前的 socket 想要绑定就会失败
         * 当然如果设置 SO_REUSEADDR 不会出现这种问题
        */
    }
    #else
    if (on)
    {
        LOG_ERROR << "SO_REUSEPORT is not supported.";
    }
#endif
}

void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(this->sockfd_, SOL_SOCKET, SO_KEEPALIVE,
        &optval, static_cast<size_t>(sizeof optval));
}
