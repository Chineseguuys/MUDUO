#include "net/Channel.h"
#include "base/Logging.h"
#include "net/EventLoop.h"



#include <sstream>

#include <poll.h>

using namespace muduo;
using namespace muduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;


Channel::Channel(EventLoop* loop, int fd_)
    :  loop_(loop),
        fd_(fd_),
        events_(0),
        revents_(0),
        index_(-1),
        logHup_(true),
        tied_(false),
        eventHandling_(false),
        addedToLoop_(false)
    {}


Channel::~Channel()
{
    assert(!this->eventHandling_);
    assert(!this->addedToLoop_);
    if (loop_->isInLoopThread())    /*这些函数还没有定义*/
    {
        assert(!loop_->hasChannel(this));   /*这个函数还没有定义*/
    }
}


void Channel::tie(const std::shared_ptr<void>& obj)
{
    LOG_TRACE << "Channel::tie-" << "channel:" << this->fd();
    this->tie_ = obj;
    this->tied_ = true;
}

void Channel::update()
{
    addedToLoop_ = true;
    loop_->updateChannel(this);
}

void Channel::remove()
{
    assert(isNoneEvent());
    addedToLoop_ = false;
    loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
    std::shared_ptr<void> guard;
    if (tied_)
    {
        guard = tie_.lock();  /*返回 shared_ptr 对象*/
        /**
         * tie_ 持有的的对象有可能已经失效
        */
        if (guard)
        {
        handleEventWithGuard(receiveTime);
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    eventHandling_ = true;  /**事件正在处理*/
    LOG_TRACE << reventsToString();
    if ((revents_ & POLLHUP) && !(revents_ & POLLIN))   /**指定的文件描述符挂起状态，并且没有数据可读*/
    {
        if (logHup_)
        {
            LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
        }
        if (closeCallback_) closeCallback_();
    }

    if (revents_ & POLLNVAL)    /**指定的文件描述符非法*/
    {
        LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
    }

    if (revents_ & (POLLERR | POLLNVAL))    /*指定的文件描述符发生错误或者非法*/
    {
        if (errorCallback_) errorCallback_();
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    /*有数据可读 / 有紧急的数据可读 / 流套接字的远程端断开连接，或者关闭了写端*/
    {
        if (readCallback_) readCallback_(receiveTime);
    }
    if (revents_ & POLLOUT) /*现在写将不会阻塞*/
    {
        if (writeCallback_) writeCallback_();
    }
    eventHandling_ = false; /**没有事件在进行处理*/
}

string Channel::reventsToString() const
{
    return eventsToString(fd_, revents_);
}

string Channel::eventsToString() const
{
    return eventsToString(fd_, events_);
}

string Channel::eventsToString(int fd, int ev)
{
    std::ostringstream oss;
    oss << fd << ": ";
    if (ev & POLLIN)
        oss << "IN ";
    if (ev & POLLPRI)
        oss << "PRI ";
    if (ev & POLLOUT)
        oss << "OUT ";
    if (ev & POLLHUP)
        oss << "HUP ";
    if (ev & POLLRDHUP)
        oss << "RDHUP ";
    if (ev & POLLERR)
        oss << "ERR ";
    if (ev & POLLNVAL)
        oss << "NVAL ";

    return oss.str();   /**将字符流转化为 string*/
}