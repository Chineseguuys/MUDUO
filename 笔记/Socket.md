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

