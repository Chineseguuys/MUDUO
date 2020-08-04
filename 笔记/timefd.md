# timefd

timerfd 涉及的API

```c++
#include <sys/timerfd.h>

int timerfd_create(int clockid, int flags);
/*
timerfd_create（）函数创建一个定时器对象，同时返回一个与之关联的文件描述符。
clockid：clockid标识指定的时钟计数器，可选值（CLOCK_REALTIME、CLOCK_MONOTONIC。。。）
CLOCK_REALTIME:系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,中间时刻如果系统时间被用户改成其他,则对应的时间相应改变
CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
flags：参数flags（TFD_NONBLOCK(非阻塞模式)/TFD_CLOEXEC（表示当程序执行exec函数时本fd将被系统自动关闭,表示不传递）
*/
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value,struct itimerspec *old_value);
/*
timerfd_settime()此函数用于设置新的超时时间，并开始计时,能够启动和停止定时器;
    fd: 参数 fd 是 timerfd_create 函数返回的文件句柄
    flags：参数 flags 为 1 代表设置的是绝对时间（TFD_TIMER_ABSTIME 表示绝对定时器）；为0代表相对时间。
    new_value: 参数 new_value 指定定时器的超时时间以及超时间隔时间
    old_value: 如果 old_value 不为 NULL,  old_vlaue 返回之前定时器设置的超时时间，具体参考 timerfd_gettime() 函数
*/
int timerfd_gettime(int fd, struct itimerspec *curr_value);
/*
    timerfd_gettime()函数获取距离下次超时剩余的时间
    curr_value.it_value 字段表示距离下次超时的时间，如果改值为0，表示计时器已经解除
    该字段表示的值永远是一个相对值，无论 TFD_TIMER_ABSTIME 是否被设置
    curr_value.it_interval 定时器间隔时间
*/
```

```timerfd_create```: 创建一个 timerfd；返回的 fd 可以进行如下操作： read, select(poll, epoll), close

```timerfd_setting``` : 设置 timer 的周期，以及初始的间隔

```timerfd_gettime``` : 获取过期的时间


函数参数中的数据结构如下：

```c++
struct timespec
{
time_t tv_sec; /* Seconds */
long tv_nsec; /* Nanoseconds */
};

struct itimerspec
{
struct timespec it_interval; /* Interval for periodic timer */
struct timespec it_value; /* Initial expiration */
};
```

* timerfd是 Linux 为用户程序提供的一个定时器接口。这个接口基于文件描述符，通过文件描述符的可读事件进行超时通知，所以能够被用于 ```select/poll``` 的应用场景

* ```timerfd```、```eventfd```、```signalfd``` 配合 ```epoll``` 使用，可以构造出一个零轮询的程序，但程序没有处理的事件时，程序是被阻塞的。

# epoll 的底层的原理

## 网络接收数据

* 网卡将接收到的数据写入到内存中的某个地址当中

## 如何知道接收到了数据

* 网卡接收到数据，写入到内存，网卡发出一个硬件中断，这个中断会立刻被 CPU 截获并开始去执行网卡中断程序

## 进程阻塞为什么不占用 CPU 

操作系统为了支持多任务，实现了进程调度的功能，会把进程分为 运行 和 等待 等几种状态。运行状态是进程获得cpu使用权，正在执行代码的状态；等待状态是阻塞状态，比如运行 recv函数，程序会从运行状态变为等待状态，接收到数据后又变回运行状态。操作系统会分时执行各个运行状态的进程，由于速度很快，看上去就像是同时执行多个任务

### 例子

假设队列中有 A B C 三个进程：

当进程A执行到创建socket的语句时，操作系统会创建一个由文件系统管理的socket对象。这个socket对象包含了<font color=red>发送缓冲区、接收缓冲区、等待队列等成员。等待队列是个非常重要的结构，它指向所有需要等待该socket事件的进程。</font>

当程序执行到recv时，<font color=red>操作系统会将进程A从工作队列移动到该socket的等待队列中</font>。由于工作队列只剩下了进程B和C，依据进程调度，cpu会轮流执行这两个进程的程序，不会执行进程A的程序。所以进程A被阻塞，不会往下执行代码，也不会占用cpu资源。

当socket接收到数据后，操作系统将该socket等待队列上的进程重新放回到工作队列，该进程变成运行状态（<font color=red>当socket事件触发时，也就是有数据到来，会取下一个进程结构调用其回调，将其挂到工作队列中</font>），继续执行代码。也由于socket的接收缓冲区已经有了数据，recv可以返回接收到的数据

* socket 是由操作系统管理的，其一直在工作队列当中并处在运行的状态，但是访问这个 socket 的程序是由我们书写的，并且处在阻塞的状态，当 socket 接收到数据的时候，它会调用我们用户提前在 socket 中安排好的回调函数，通过 socket 调用回调来实现重新启动我们用户的程序


## 串联上面的过程

1. 进程 recv() 被阻塞，丢进了 socket 的等待队列当中
2. 网卡接收到数据，数据写入内存，网卡发送硬件中断，CPU 去执行网卡的中断服务程序
3. 中断服务程序一方面把数据写入到对应的 socket 的数据接收缓冲区当中，之后再唤醒进程 A 
4. A 被重新放入到工作队列当中

## 问题

> <font color=red>数据从网卡到 socket 是由操作系统完成的。从 socket 到服务器程序或者客户端程序是由用户程序自己完成的</font>

1. 操作系统如何知道网卡送来的数据应该交给哪一个 socket?
2. 如何同时监视多个 socket 的数据？
    * 一个 recv 只能监视一个 socket，那么对于服务器端，需要对大量的连接进行监视，要采用什么样的方法

* 对于发送到网络的信息，socket 一定包含了对应的  ip 信息和 端口信息。由网卡接收的数据也一定包含了 ip 信息和 端口的信息，所以通过查找就可以知道数据是发给谁的

* 同时监视多个 socket 

    * select 使用的方法： 把进程 A 放入到所有的希望被监视的 socket 的等待队列的下面。只要其中有一个 socket 接收到了数据，就可以唤醒 A 。此时进程 A 还需要遍历一次所有的 socket，才可以知道是谁唤醒了自己，然会再去处理相应的数据
    * 其一，每次调用select都需要将进程加入到所有监视socket的等待队列，每次唤醒都需要从每个队列中移除。这里涉及了两次遍历（遍历进程A关心的所有socket，需要注意的是添加从等待队列头部添加，删除通过回调直接实现，所以每个socket的等待队列不用遍历），而且每次都要将整个fds列表传递给内核，有一定的开销。正是因为遍历操作开销大，出于效率的考量，才会规定select的最大监视数量，默认只能监视1024个socket。
    * 其二，进程被唤醒后，程序并不知道哪些socket收到数据，还需要遍历一次（这一次遍历是在应用层）。

# epoll

## ```epoll``` 和 ```select```, ```poll``` 的区别

* select 和 poll 在每一次进行查询之前，都要将用户程序关联到每一个文件描述符(socket) 的等待队列当中，每一次调用返回之后，都会解除这些关联。当然这具有非常高的灵活性，每一次我们的程序可以监控不同的 socket。但是实际的情况是，我们的用户程序往往监控几个 socket 是固定的，一旦确定下面就不太去更改它，这种操作非常的耗时

* 另一方面，select 和 poll 只知道有几个 socket 触发了，但是不知道是哪几个触发了，所以我们的用户程序还要自己去遍历一次所有的 socket 才可以确认（socket 越多，遍历的时间就消耗越多）

* epoll 一旦用户程序和某些 socket 关联之后，一般不会再改变。（不再进行重复的添加和删除）。epoll 会将触发的 socket 排在数组的最前面，所以就不需要全部遍历了。（<font color=red>epoll 同样提供了添加和删除监控的 socket 功能，并没有失去灵活性</font>）

## 相关的函数

```c++
int epoll_create(int size);
/**
    size 表示的是 epoll 希望监听的文件描述符的个数，这个数值不需要精确的数值，只要是一个大致的数值就可以了
*/
```

```c++
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
/**
    EPOLL_CTL_ADD
    EPOLL_CTL_MOD
    EPOLL_CTL_DEL
*/

struct epoll_event {
    _u32 event;
    union {
        void * ptr;
        int fd;
        _u32 u32;
        _u64 u64;
    } data;
};

epoll_event::event 中可以监测的事件非常的多
```

### 轮询

```c++
int epoll_wait(int epfd, struct epoll_event* event, int maxevents, int timeout);
/**
    epoll 监测的事件需要使用轮询进行查看，否则的话应用程序无法接收到这些文件描述符的事件
    timeout : epoll 查看的时候，不会一直阻塞在这里，等待一定的时间后返回（即使在这个时间内没有事件发生也会返回）

    int 返回发生了事件的文件描述符的个数，epoll_event[] 已经进行了排序，发生事件的 epoll_event 排列在前面
*/
```

# BLOCKING 的解释

* 在我们使用多线程 + 阻塞IO 读取的方式进行多连接的编程的时候，线程是被 socket IO  所阻塞，如果 socket 没有准备好数据的话，那么我们的线程就会 blocking 在这个读取或者写入的过程当中

* 使用 select poll epoll 并不意味着我们的线程没有了 blocking，而是不会在 socket 上被 blocking。实际上我们的线程会在 select poll epoll 上被 blocking（**当我们监视的多个 socket 有数据到来的时候，我们的线程才会被唤醒进行相应的处理**）。

* 所有 select poll epoll 的技术被称之为非阻塞IO， 而不能称之为非阻塞（<font color=red>当然，我们不能被 IO 阻塞，因为我们需要对多用户的数据进行处理</font>）


## 那么我们为什么使用 ```select``` , ```poll```, ```epoll``` ?

* 原因很简单，因为它们可以解决大量连接的情况。如果我们需要对多个连接进行控制，而采用了阻塞IO 的方式，那么一旦我们阻塞在某一个 IO 上，那么其他的连接我们就没办法处理了，效率会大大降低。

* 如果连接的数量不是很多的话，多线程 + IO 的模式可能比 ```select``` , ```poll```, ```epoll``` 的效果更好

* 在我们使用 ```select``` , ```poll```, ```epoll``` 的时候，将 socket 设置为非阻塞，然后通过 ```select``` , ```poll```, ```epoll``` 来管理这些 socket 



## 注意

epoll， poll, select 并不是自动触发的，需要在循环当中不断的进行查询，如果循环过程中处理的事务比较的多，噪声每一次循环用时比较的长，一定程度上事件就无法得到及时的处理