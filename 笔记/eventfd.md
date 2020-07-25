# 事件触发

eventfd 是一个用来通知事件的文件描述符，它是一个内核空间下应用发送通知的机制

```c++
int eventfd(unsigned int initval, int flags);
```

flags 可以使用下面的三种标志：

* ```EFD_CLOEXEC``` : 简单的来说就是 fork 子进程的时候不继承这个文件描述符

* ```EFD_NONBLOCK``` : 文件的操作将会是非阻塞的

* ```EFD_SEMAPHORE``` : 支持 semophore 语义的读


## 两个操作

```c++
int ret = ::read(int sockfd, void* buf, sizeof buf);
```

如果使用 ```write()``` 将 counter 设置的值为1 那么调用 read() 将会将 counter 的值设置为 0；如果没有先使用 ```write()``` 对 counter 进行置位，直接调用 ```read()``` 那么将会阻塞（sockfd 为阻塞的），或者产生错误 ```EAGAIN``` ,表示稍后进行重试


# 定时触发的任务

定时触发任务所使用的 timerfd 依然是交给 epoll 进行管理的，所以在定时器超时的时候，可能没有及时的读取相应触发的定时器，所以存在一定的延时误差。具体的超时的时间由调用 ```read()``` 函数的时候的时间为准

```c++
int timerfd_create(int clockid, int flags);

int timerfd_settime(int fd, int flags,
                           const struct itimerspec *new_value,
                           struct itimerspec *old_value);

int timerfd_gettime(int fd, struct itimerspec *curr_value);
```

