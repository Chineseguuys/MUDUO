## 1.如何保证主动关闭的时候，对方已经收到了全部的数据？ 如何保证缓冲区的数据发送完毕才关闭写端？

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

<font color=red>一旦连接的状态进入了 ```KDisconnecting``` 状态，那么在每一次写数据的过程中，在写入数据完毕之后都会去检测连接的状态，一旦连接的状态是 ```KDisconnecting``` ，那么就可以断开连接</font>


## 2.连接的状态发生了变化，如何通知上层？

我们知道 ```TcpConnection``` 是负责数据的传输功能的函数。他和上层 ```TcpServer``` 和 ```TcpClient``` 之间使用回调函数进行处理。

1. 完成连接的过程，状态从 ```KDisConnecting``` 状态到 ```KConnected``` ,这个时候需要调用 ```connectionCallback_``` 回调通知上层状态发生了变化

2. ```TcpConnection``` 要执行关闭的时候，如果连接的状态是 ```Kconnected``` 或者 ```KDisconnecting``` 状态，会将其设置为 ```KDisconnected``` 然后调用  ```connectionCallback_``` 通知上层

3. 在销毁连接的时候， channel_ 从监听的队列中移除。如果移除之前的状态是 ```KConnected``` ,那么设置为 ```DisConnected``` 。这个时候调用 ```connectionCallback_``` 通知上层

## 3.读取到数据如何通知上层?

```c
this->messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
```

## 4.完成一次数据的传递的过程？
一次发送的数据可能非常的多，可以经过多次发送的过程，才可以完成数据的发送的过程。这个时候回调函数  
```c
this->loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
```

用于通知上层函数本次的数据全部已经发送完毕。可以发送后面的数据，或者可以关闭连接的过程


## 5.如果主动的发起连接，但是对方一直进行拒绝，如何定期进行重试？

这一部分的解决方案在文件 ```Connector.h``` 和 ```Connector.cc``` 文件当中.

1. 发起重试的最短的时间延时为 500 ms。最大的超时重试的延时为 30s

2. 在使用 ```socket()``` 函数建立套接字接口之后，使用 ```connect() ``` 函数进行连接请求。由于是非阻塞形式调用上述函数，所以我们将  socketfd 添加到 epoll 监测队列当中去，使用 channel 注册读写的回调函数对 socketfd 上面的连接事件进行监测。此时的连接的状态为 ```kConnecting```

3. 如果读的过程中出现了错误，那么我们需要设置超时重连，首先要关闭上面已经打开的文件描述符，计算新的重试延时。一般每一次重试的延时是上一次延时的 2 倍。直到延时设定的最大值 30s 。调用 EventLoop::runAfter() 设置定时任务，将 ```startInLoop()``` 加入到 timerQueue 队列中，在延时达到之后重新进行连接

4. 目前代码中的实现会一直进行重传，不会自动结束连接的过程（也就是说会一直到连接成功为止）。在超时重试的延时达到了 30s 之后，会不断的每 30s 进行一次重试


## 6.非阻塞网络编程应该用边沿触发还是电平触发？如果使用电平触发，那么什么时候关注 EPOLLOUT 事件？ 会不会造成 busy-loop? 如果采用的是边沿触发，如何防治漏读造成的饥饿？

* 漏读--- 在使用边沿触发的时候，如果 socket 的缓冲区的数据没有一次性的读取完毕，在这一次读取数据之后，没有新的事件到达的话，那么缓冲区内剩余的数据可能一直得不到读取，就会造成漏读的问题。

为了防止漏读，那么我们需要在每一次读取的过程中，把所有的数据全部读取出来，但是应用的缓冲区 ```Buffer``` 可能没有容纳所有的数据的大小，所以使用了散布 I/O 的方式使用 ```readv()``` 来进行读取。Buffer 需要进行动态的扩容

* 扩容包括直接扩大 buffer 的大小，即重新分配内存 + 数据的拷贝。 或者如果已经读取的部分的空间 + 还可以写入的空间 大小大于要存储的空间，我们也可以通过空间的挪动来扩大我们的可写空间



