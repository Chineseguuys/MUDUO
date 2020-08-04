# Sockct

## socket 的绑定规则

* 规则1：socket可以指定绑定到一个特定的ip和port，例如绑定到192.168.0.11:9000上；

* 规则2：同时也支持通配绑定方式，即绑定到本地"any address"（例如一个socket绑定为 0.0.0.0:21，**那么它同时绑定了所有的本地地址**）；

* 规则3：**默认情况下**，任意两个socket都无法绑定到相同的源IP地址和源端口。

## SO_REUSEADDR

SO_REUSEADDR 的作用主要有下面的两点：

1. 改变了通配绑定时处理源地址冲突的处理方式：

| SO_REUSEADDR |	先绑定socketA |	后绑定socketB 	| 绑定socketB的结果
| ------| ------- | ------ | ------
| 无关 |	192.168.0.1:21 |	192.168.0.1:21 	| Error (EADDRINUSE)
无关 	|192.168.0.1:21 |	10.0.0.1:21 |	OK
无关 |	10.0.0.1:21 |	192.168.0.1:21 |	OK
disable |	0.0.0.0:21 	| 192.168.1.0:21 |	Error (EADDRINUSE)
disable |	192.168.1.0:21 	| 0.0.0.0:21 	| Error (EADDRINUSE)
enable | 	0.0.0.0:21 | 	192.168.1.0:21 |	OK
enable 	| 192.168.1.0:21 |	0.0.0.0:21 |	OK
无关 |	0.0.0.0:21 |	0.0.0.0:21 |	Error (EADDRINUSE)

2. 改变了系统对处于 TIME_WAIT 状态的 socket 的看待方式

> socket通常都有发送缓冲区，当调用send()函数成功后，只是将数据放到了缓冲区，并不意味着所有数据真正被发送出去。对于TCP socket，在加入缓冲区和真正被发送之间的时延会相当长。这就导致当close一个TCP socket的时候，可能在发送缓冲区中保存着等待发送的数据。为了确保TCP的可靠传输，TCP的实现是close一个TCP socket时，如果它仍然有数据等待发送，那么该socket会进入TIME_WAIT状态。这种状态将持续到数据被全部发送或者发生超时（这个超时时间通常被称为Linger Time，大多数系统默认为2分钟）。

* 在未设置SO_REUSEADDR时，内核将一个处于TIME_WAIT状态的socketA仍然看成是一个绑定了指定ip和port的有效socket，因此此时如果另外一个socketB试图绑定相同的ip和port都将失败（不满足规则3），直到socketA被真正释放后，才能够绑定成功。如果socketB设置SO_REUSEADDR（仅仅只需要socketB进行设置），这种情况下socketB的绑定调用将成功返回，但真正生效需要在socketA被真正释放后。（这个地方的理解可能有点问题，待后续验证一下）。总结一下：**内核在处理一个设置了SO_REUSEADDR的socket绑定时，如果其绑定的ip和port和一个处于TIME_WAIT状态的socket冲突时，内核将忽略这种冲突，即改变了系统对处于TIME_WAIT状态的socket的看待方式。**

## SO_REUSEPORT

1. **允许将多个socket绑定到相同的地址和端口，前提每个socket绑定前都需设置SO_REUSEPORT**。如果第一个绑定的socket未设置SO_REUSEPORT，那么其他的socket无论有没有设置SO_REUSEPORT都无法绑定到该地址和端口直到第一个socket释放了绑定。

2. attention：SO_REUSEPORT并不表示SO_REUSEADDR，即不具备上述SO_REUSEADDR的第二点作用（对TIME_WAIT状态的socket处理方式）。因此当有个socketA未设置SO_REUSEPORT绑定后处在TIME_WAIT状态时，如果socketB仅设置了SO_REUSEPORT在绑定和socketA相同的ip和端口时将会失败。解决方案
    1. socketB设置 SO_REUSEADDR 或者socketB即设置 SO_REUSEADDR 也设置 SO_REUSEPORT
    2. 两个socket上都设置 SO_REUSEPORT

## SO_KEEPALIVE

对于面向连接的TCP socket,在实际应用中通常都要检测对端是否处于连接中,连接端口分两种情况:
1. 连接正常关闭,调用close() shutdown()连接优雅关闭,send与recv立马返回错误,select返回SOCK_ERR;
2. 连接的对端异常关闭,比如网络断掉,突然断电.

对于第二种情况,判断连接是否断开的方法有一下几种:

1. 自己编写心跳包程序,简单的说就是自己的程序加入一条线程,定时向对端发送数据包,查看是否有ACK,根据ACK的返回情况来管理连接。此方法比较通用,一般使用业务层心跳处理,灵活可控,但改变了现有的协议;

2. 使用TCP的keepalive机制,UNIX网络编程不推荐使用 ```SO_KEEPALIVE``` 来做心跳检测(为什么??)。

keepalive原理:

* TCP内嵌有心跳包,以服务端为例,当server检测到超过一定时间(/proc/sys/net/ipv4/tcp_keepalive_time 7200 即2小时)没有数据传输,那么会向client端发送一个keepalive packet,此时client端有三种反应:

    1. client端连接正常,返回一个ACK.server端收到ACK后重置计时器,在2小时后在发送探测.如果2小时内连接上有数据传输,那么在该时间的基础上向后推延2小时发送探测包;

    2. 客户端异常关闭,或网络断开。client无响应,server收不到ACK,在一定时间(/proc/sys/net/ipv4/tcp_keepalive_intvl 75 即75秒)后重发keepalive packet, 并且重发一定次数(/proc/sys/net/ipv4/tcp_keepalive_probes 9 即9次);

    3. 客户端曾经崩溃,但已经重启.server收到的探测响应是一个复位,server端终止连接。



## 程序的实现细节

### 建立 Socket 

```c++
int sockfd = ::socket(sa_family_t, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOSEXEC, IPPROTO_TCP);
```

### 绑定指定的 IP 和 端口号

```c
int ret = ::bind(int sockfd, const struct sockaddr* addr, /*地址的实际的长度*/);
/**
	地址的实际的长度是用户决定的，使用 Ipv4 协议，addr 的实际类型为 sockaddr_in, 使用 ipv6 协议的话，addr 的实际类型为 sockaddr_in6
*/
```

### 开始在端口进行监听

```c
int ret = ::listen(int sockfd, int backlog);
```

```backlog``` 参数表明了服务端的 ```socket``` 允许的最长的未连接的等待队列的长度，如果一个新的客户端的连接请求到达，但是队列达到了最大的长度，那么这个连接将会被丢弃，客户端可能会收到 ```ECONNREFUSED``` 错误提示。客户端程序可以选择在接收到这个错误之后选择重新进行连接或者直接结束连接

```ret```  如果成功的开始监听的过程，那么这个系统调用返回 0 ，否则的话，返回  -1

#### 可能会发生的错误

1. ```EADDRINUSE``` 这个Ip + 端口已经被另一个 socket 所占用或者这个 socket 在进行监听之前没有进行 ```bind``` 的过程，操作系统尝试给其分配一个随机的 端口，但是发现无法完成分配（可用端口使用完了）

2.  ```EBADF``` 提供的 sockfd 是一个无效的值
3. ```ENOTSOCK```  提供的 sockfd 根本就不是一个 socket 套接字
4. ```EOPNOTSUPP``` 提供的 sockfd 不是一个可以用来监听端口的套接字

### 接受一个客户端的连接

```c++
int connfd = ::accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
int connfd = ::accpet4(int sockfd, struct sockaddr* addr, socklen_t* addrlen, int flags);
/** flags 一般设置为 SOCK_NONBLOCK | SOCK_CLOSEXEC 并且也只有这两个参数可以选择
```

```connfd``` 如果连接成功的话，connfd 将会返回一个连接套接子的文件描述符，如果错误的话，那么返回的 -1，并且 errono 将会被设置为相应的错误的编号



#### 可能发生的错误

```EAGAIN``` : 对于非阻塞形式的连接，在使用这个系统调用的时候，可能还没有任何的连接建立起来，会返回这个错误，这个错误可以直接忽略掉，因为它并不代表出错了，用户可以过一段时间重新进行系统调用

```EINTR``` 在执行这个系统调用的过程中发生了中断，操作系统要去执行中断服务程序，当前的连接短时间无法返回结果了，用户需要自行处理（过一段时间在来查询）

```EPROTO``` 协议错误

```EPERM``` 防火墙组阻止连接的进行

```EMFILE``` 当前的进程可以打开的文件的数量达到了上限，或者操作系统可以打开的文件的数量达到了上限，没有办法为当前的连接建立一个 socket 套接字

<font color=red>```EBADF``` </font>上面已经介绍了[^1]

<font color=red>```ENOTSORK```</font> 上面进行了介绍

<font color=red>```EFAULT``` </font>系统调用的参数 ```addr``` 不是一个用户空间可以写入的内存地址

<font color=red>```ENOBUFS``` </font> 这个错误的原因还没有了解（man 文档的解释是 Not enough free memory. This often means that the memory allocation is limited by the socket buffer limuts, not by the system memory.)

<font color=red>```EOPNOTSUPP``` </font>提供的 socket 不是一个流类型的 （```SOCK_STREAM```）

### 客户端的连接

```c++
int ret = ::connect(int sockfd, struct sockaddr* addr, socklen_t addrlen);
```

ret : 如果连接握手的过程成功，返回 0， 否则的话返回 -1， 并且将全局的变量 errno 设置为相应的错误值

### 错误的类型

```c++
	switch(savedErrno){
		case 0 :
		case EINPROGRESS :	/*正在进行连接的处理*/
		case EINTR :	/*当前的连接的过程被系统调用打断*/
		case EISCONN :	/*参数sockfd的socket已是连线状态*/
			this->connecting(sockfd);
			break;

		case EAGAIN: /*重新进行尝试*/
		case EADDRINUSE: /*地址已经被使用*/
		case EADDRNOTAVAIL: /*如果没有端口可用，返回这个错误*/
		case ECONNREFUSED: /*连接被拒绝*/
		case ENETUNREACH:	/*网络不可达*/
			retry(sockfd);
			break;

		case EACCES: /*没有权限*/
		case EPERM:	/*操作没有被允许*/
		case EAFNOSUPPORT:	/*地址家族没有被协议支持*/
		case EALREADY:	/*操作已经在进行中*/
		case EBADF:	/*错误的文件描述符*/
		case EFAULT:	/*错误的地址*/
		case ENOTSOCK: /*在非 socket 上面执行 socket 操作*/
			LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
			sockets::close(sockfd);
			break;
		
		default :
			LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
			sockets::close(sockfd);
			// connectErrorCallback_();
			break;
	}
```

## 4.1 如何处理连接的关闭过程

1. 正常的关闭的过程。由客户 client 来进行关闭
2. 异常的关闭过程，由于 socket 的错误不得不关闭

> <font color=red>在 Tcp 的连接当中，关闭读操作是对对方不可见的。虽然关闭了读，但是操作系统还是会接收对方的所有的  Tcp 报文，只是这些报文会全部丢弃，不提交给应用层。发送方也不知道对方怎么处理自己的数据的</font>

> <font color=red>对方关闭写操作，对自己没有任何的影响，第一，自己的读操作是被动的，对方关闭了，自己不读就完事了。对方关闭了读操作自己又无法发现。那么对于自己来说，只要处理好对方的完全关闭（读写全部都被关闭）的过程就可以了</font>

```c
int shutdown(int fd, int how)
/**
 * fd 是你要关闭的 socket 的文件描述符
 * 有三种关闭的方式
 * SHUT_RD : 关闭读端（另一端不会发现对方关闭了读端，因为关闭读端只是操作系统不会将 socket 的内容提交给应用层
 * 但是操作系统依然会正常接收来自对方的数据，只不过，全部丢弃）
 * SHUT_WR ： 关闭写端 （关闭写端的话，会向对方发送 FIN 报文，告知对方自己不再发送数据了）
 * SHUT_RDWR ： 同时关闭了读端和写端（给对方发送 FIN 报文，告诉对方自己不再发送数据了）
*/
```

### 4.1.1 如何处理对方发送的 FIN 报文？

1. 对方发送了 FIN(可以认为对方是处在半关闭的状态) 。我们不需要考虑对方是调用了 ```close()``` 函数还是使用了 ```shutdown()``` 函数。
   1. 对方只是关闭了写端，我们依然可以给对方发送数据
   2. 对方同时关闭了写端和读端，我们给对方发送数据的时候，会产生超时的错误。






### 4.1.2 注意异常退出和正常退出的区别


## 注释

[^1]: 红色的部分表示的是致命的错误，在 muduo 中会造成进程的直接结束