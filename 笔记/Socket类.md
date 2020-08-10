# Socket 类

这个类的主要的作用就是封装一下 socket 文件描述符 ```sockfd```，以及在这个文件描述符上面的一些操作

1. 绑定 ip 和端口号
2. 在端口上进行监听
3. 调用 accpet() 开始进行连接的处理
4. 以及设置一些 Tcp 的相关的属性
4. 单向关闭读端， ```shutdownWrite()```


有关 Tcp 的相关的属性的设置：
```c
int setsockopt(int __fd, int __level, int __optname, const void *__optval, socklen_t __optlen) throw()
```

比方说设置 Tcp 的延迟发送的属性：
```c
void Socket::setTcpNoDelay(bool on)
{
    /**
     * 为了防止出现 TCP 的粘包问题，我们不希望 Tcp 等待多个数据报，合并为一个大的数据报一起进行发送
    */
    int optval = on ? 1 : 0;
    ::setsockopt(this->sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<size_t>(sizeof optval));
}
```

设置 Tcp 的 ip 复用和 端口 复用

```C
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
```

