#include "net/EventLoopThread.h"

#include "net/EventLoop.h"

using namespace muduo;
using namespace muduo::net;


EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, 
				const string& name)
		: loop_(NULL),
			exiting_(false),
			thread_(std::bind(&EventLoopThread::threadFunc, this), name),
			mutex_(),
			cond_(mutex_),
			callback_(cb)
{
}

EventLoopThread::~EventLoopThread()
{
	this->exiting_ = true;
	if (this->loop_ != NULL)
	{
		this->loop_->quit();
		this->thread_.join();
	}
}

EventLoop* EventLoopThread::startLoop()
{
	assert(!this->thread_.started());
	this->thread_.start();
	EventLoop* loop = NULL;
	{
		MutexLockGuard lock(this->mutex_);
		 while(this->loop_ == NULL)
		 {
			 cond_.wait();
		 }
		 loop = this->loop_;
	}
	return loop;
}

void EventLoopThread::threadFunc()
{
	EventLoop loop;

	if (this->callback_)
	{
		this->callback_(&loop);
	}
	{
		MutexLockGuard lock(this->mutex_);
		this->loop_ = &loop;
		this->cond_.notify();
	}

	loop.loop();
	MutexLockGuard lock(this->mutex_);
	this->loop_ = NULL;
}