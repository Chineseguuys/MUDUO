#include "net/EventLoop.h"
#include "base/Mutex.h"
#include "base/Logging.h"
#include "net/Channel.h"
#include "net/Poller.h"
#include "net/SocketsOps.h"
#include "net/TimerQueue.h"

#include <algorithm>

#include <signal.h>

#include <sys/eventfd.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;


namespace{

__thread EventLoop* t_loopInThisThread = 0;
/**
 * 一个线程一个 eventloop ?
*/

const int kPollTimeMs = 10000;

/**
 * 目前越来越多的应用程序采用事件驱动的方式实现功能，如何高效地利用系统资源实现通知的管理和送达就愈发变得重要起来。
 * 在Linux系统中，eventfd是一个用来通知事件的文件描述符，timerfd是的定时器事件的文件描述符。
 * 二者都是内核向用户空间的应用发送通知的机制，可以有效地被用来实现用户空间的事件/通知驱动的应用程序
 * 
 * 如果eventfd设置了EFD_SEMAPHORE，那么每次read就会返回1，并且让eventfd对应的计数器减一；
 * 如果eventfd没有设置EFD_SEMAPHORE，那么每次read就会直接返回计数器中的数值，read之后计数器就会置0。
 * 不管是哪一种，当计数器为0时，如果继续read，
 * 那么read就会阻塞（如果eventfd没有设置EFD_NONBLOCK）
 * 或者返回EAGAIN错误（如果eventfd设置了EFD_NONBLOCK）
*/
int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    /**
     * EFD_NONBLOCK ： 非阻塞的形式
     * EFD_CLOEXEC  ： 在使用 fork() 生成的子进程不继承这个文件描述符
    */
    if (evtfd < 0)
    {
        LOG_SYSERR << "Failed to Create eventfd";
        abort();
    }
    return evtfd;
}

#pragma GCC diagnostic ignored "-Wold-style-cast"
struct IgnoreSigPipe
{
public:
    IgnoreSigPipe()
    {
        ::signal(SIGPIPE, SIG_IGN);
        /**
         * SIGHUP:在对会话的概念有所了解之后，我们现在开始正式介绍一下SIGHUP信号，
         *  SIGHUP 信号在用户终端连接(正常或非正常)结束时发出, 通常是在终端的控制进程结束时, 
         *  通知同一session内的各个作业, 这时它们与控制终端不再关联. 系统对SIGHUP信号的默认处理是终止收到该信号的进程。
         *  所以若程序中没有捕捉该信号，当收到该信号时，进程就会退出
         * SIGPIPE: 在网络编程中，SIGPIPE这个信号是很常见的。
         *  当往一个写端关闭的管道或socket连接中连续写入数据时会引发SIGPIPE信号,引发SIGPIPE信号的写操作将设置errno为EPIPE。
         *  在TCP通信中，当通信的双方中的一方close一个连接时，若另一方接着发数据，
         *  根据TCP协议的规定，会收到一个RST响应报文，若再往这个服务器发送数据时，
         *  系统会发出一个SIGPIPE信号给进程，告诉进程这个连接已经断开了，不能再写入数据。
         * 
         * 在处理方式上，SIG_IGN 表示忽略这个信号
        */
    }
};
#pragma GCC diagnostic error "-Wold-style-cast"

IgnoreSigPipe initObj;
}  // namespace 

EventLoop* EventLoop::getEventLoopOfCurrentThread(){
    return t_loopInThisThread;
}

EventLoop::EventLoop()
    :   looping_(false),
        quit_(false),
        eventHandling_(false),
        callingPendingFunctors_(false),
        iteration_(0),
        threadId_(CurrentThread::tid()),
        poller_(Poller::newDefaultPoller(this)),
        timerQueue_(new TimerQueue(this)),
        wakeupFd_(createEventfd()),
        wakeupChannel_(new Channel(this, wakeupFd_)), /**/
        currentActiveChannel_(NULL)
{
    LOG_DEBUG << "Eventloop Create " << this << " in thread " << threadId_;
    /**
     * 我们在一个线程当中，只允许存在一个 eventloop 实例
    */
    if (t_loopInThisThread)
    {
        LOG_FATAL << "Another EventLoop " << t_loopInThisThread << " exit in this thread " << threadId_;
    }
    else
    {
        t_loopInThisThread = this;
        LOG_INFO << "set the t_loopInThisThread " << this << " : " << threadId_;
    }
    this->wakeupChannel_->setReadCallback(
        std::bind(&EventLoop::handleRead, this) /*Channel 的读回调函数设置为了 eventloop 的 handleRead*/
    );
    this->wakeupChannel_->enableReading();  /*我们总是读唤醒的 fd*/
}

EventLoop::~EventLoop()
{
    LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
            << " destructs in thread " << CurrentThread::tid();
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = NULL;
}

/**
 * 这个函数是整个 EventLoop 类中的关键所在
 * 这个函数应该会在线程开始之后一直执行
 * loop 必须要在创建 EventLoop 的线程下面进行执行
*/
void EventLoop::loop()
{
    assert(!looping_); /*如果已经在 loop 的状态报错*/
    assertInLoopThread(); /*不允许从其他的线程，调用这个函数*/
    this->looping_ = true;
    this->quit_ = false;    /* 结束标志设置为 false, 保证 while() 循环*/
    LOG_TRACE << "EventLoop " << this << " starting loop ";

    /**
     * 所有的事件都会在 EventLoop::loop() 中通过 Channel 来进行分发
    */
    while (!quit_)
    {
        this->activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        /**
         * 在每一次的循环当中检测是否有事件发生，如果有事件发生的话，就进行相应的处理
        */
        ++iteration_;
        if (Logger::logLevel() <= Logger::TRACE)
        {
            printActiveChannels();
        }
        eventHandling_ = true;
        for (Channel* channel : this->activeChannels_)
        {
            this->currentActiveChannel_ = channel;
            /**
             * handleEvent() 是 channel 类的成员函数，它会根据事件的类型去调用不同的 Callback
            */
            this->currentActiveChannel_->handleEvent(pollReturnTime_);
        }
        currentActiveChannel_ = NULL;
        eventHandling_ = false;
        this->doPendingFunctors();
        /**
         * 除了上面的由 poll 触发的事件以外，还有各个组件直接添加到 loop 中的事件需要进行处理
        */
    }
    LOG_TRACE << "EventLoop " << this << " stop looping ";
    this->looping_ = false;
}

void EventLoop::quit()
{
    this->quit_ = true;
    /**打破 loop 中的循环*/
    if (!isInLoopThread())
    {
        this->wakeup();
        /**
         * 在这里执行 wakeup() 函数之后。poll 将会检测到相应的文件描述符的变化，将与 wakeup_ 文件描述符
         * 相对应的 channel 装载到 channel 的队列当中去，在 loop 的循环当中可以进行处理
        */
    }
}

void EventLoop::runInLoop(Functor cb)
{
    /**
     * 如果是从当前的线程中访问这个函数，则直接执行
     * 如果是从其他的线程中访问这个函数，则将其放入到队列当中
    */
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(std::move(cb));
    }
}

/**
 * 线程安全的
*/
void EventLoop::queueInLoop(Functor cb)
{
    LOG_INFO << "push Functor into the eventloop :" << this;
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(std::move(cb));  /**把句柄 cb 放入到待执行队列当中*/
    }

    if (!isInLoopThread() || callingPendingFunctors_)
    {
        wakeup();
    }
}


size_t EventLoop::queueSize() const
{
    MutexLockGuard lock(this->mutex_);
    return this->pendingFunctors_.size();
}

/*和 timer timerqueue 相关的函数*/

TimerId EventLoop::runAfter(double delay, TimerCallback cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, std::move(cb));
}

TimerId EventLoop::runAt(Timestamp time, TimerCallback cb)
{
    return timerQueue_->addTimer(std::move(cb), time, 0.0);
}

TimerId EventLoop::runEvery(double interval, TimerCallback cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(std::move(cb), time, interval);
}

void EventLoop::cancel(TimerId timerid)
{
    timerQueue_->cancel(timerid);
}

void EventLoop::updateChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}


void EventLoop::removeChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    if (eventHandling_)
    {
        assert(currentActiveChannel_ == channel || 
            std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
        /**
         * 当前活跃的 channel 不是要删除的 channel, 并且在当前的活跃 channel 队列中有要删除的 channel
         * 这里就会发出警告
        */
    }

    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return this->poller_->hasChannel(channel);
}

/**
 * 如果尝试从其他的线程来调用这个 eveltloop 的某一些函数将会发出致命错误
*/
void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  CurrentThread::tid();
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = sockets::write(wakeupFd_, &one, sizeof one); /*weakupFd_ 是一个 eventfd*/
    if (n != sizeof one)
    {
        LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;   /**读取 8 字节的数据*/
    /**
     * 这里对 wakeup_ 事件的处理，只是对这个事件进行了一下重置，没有做任何详细的处理
     * 因为确实也不需要进行任何处理，因为所有的事件都放在了pendingFunctors_ 当中，每一次 loop 的
     * 循环都会执行这里面的所有的函数，所以不需要在 handleRead() 当中进行处理
    */
    ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
    if (n != sizeof one)
    {
        LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    this->callingPendingFunctors_ = true;

    {
        MutexLockGuard lock(this->mutex_);
        functors.swap(this->pendingFunctors_);
    }

    /**
     * 一次性执行完这里面的所有的句柄
    */
    for (const Functor& functor : functors)
    {
        functor();
    }

    this->callingPendingFunctors_ = false;
}

/**
 * 打印所有活跃的 channels
*/
void EventLoop::printActiveChannels() const
{
    for (const Channel* channel : activeChannels_)
    {
        LOG_TRACE << "{" << channel->reventsToString() << "} ";
    }
}