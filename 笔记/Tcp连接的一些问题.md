## 如何保证主动关闭的时候，对方已经收到了全部的数据？ 如何保证缓冲区的数据发送完毕才关闭写端？

TcpConnection 连接具有三种状态：
```c
enum StateE { 
    KDisconnected, /*已经完全断开的连接*/
    KConnecting,   /*正在连接的过程中*/
    KConnected,    /*已经完成了连接，正在传输数据*/
    KDisconnecting /*准备关闭，但是还有数据要发送*/
};
```

当想主动去关闭一个连接的时候（关闭连接的写端，读端不进行主动的关闭）。首先,将连接的状态设置为 ```KDisconnecting```
```c
if (state == KConnected)
    state = KDisconnecting;
```

紧接着查看应用层的缓冲区内部是否还有数据

```c
if (!channel_.isWritting()) {
    socket_->shutdownWrite();
}
```

如果缓冲区内有数据，那么 ```channel_isWritting() == true```，那么这个时候我们无法执行关闭写的过程。

这个时候的链接器的状态已经处在 ```KDisconnecting``` 的状态，在发送数据的时候，如果缓冲区为空，那么需要查看这个状态，如果处在 ```KDisconnecting``` ，再次执行关闭写的操作


## 如果主动的发起连接，但是对方一直进行拒绝，如何定期进行重试？

这一部分的解决方案在文件 ```Connector.h``` 和 ```Connector.cc``` 文件当中.

1. 发起重试的最短的时间延时为 500 ms。最大的超时重试的延时为 30s

2. 



