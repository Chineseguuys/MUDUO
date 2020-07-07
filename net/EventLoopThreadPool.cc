#include "net/EventLoopThreadPool.h"
#include "net/EventLoop.h"
#include "net/EventLoopThread.h"


#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg)
    :   baseloop_(baseLoop),
        name_(nameArg),
        started_(false),
        numThreads_(0),
        next_(0)
{}

EventLoopThreadPool::~EventLoopThreadPool()
{
    // Don't delete loop, it's stack variable
}


void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
    assert(!this->started_);
    this->baseloop_->assertInLoopThread();

    this->started_ = true;

    for (int i = 0; i < this->numThreads_; ++i)
    {
        char buf[name_.size() + 32];
        snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);
        EventLoopThread* t = new EventLoopThread(cb, buf);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->startLoop());
    }

    if (this->numThreads_ == 0 && cb)
        cb(this->baseloop_);
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    baseloop_->assertInLoopThread();
    assert(started_);
    EventLoop* loop = baseloop_;

    if (!loops_.empty())
    {
        // round-robin
        loop = loops_[next_];
        ++next_;
        if (implicit_cast<size_t>(next_) >= loops_.size())
        {
        next_ = 0;
        }
    }
    return loop;
}

EventLoop* EventLoopThreadPool::getLoopForHash(size_t hashCode)
{
    baseloop_->assertInLoopThread();
    EventLoop* loop = baseloop_;

    if (!loops_.empty())
    {
        loop = loops_[hashCode % loops_.size()];
    }
    return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops()
{
    baseloop_->assertInLoopThread();
    assert(started_);
    if (loops_.empty())
    {
        return std::vector<EventLoop*>(1, baseloop_);
    }
    else
    {
        return loops_;
    }
}
