#include "base/ThreadPool.h"
#include "base/Exception.h"


#include <assert.h>
#include <errno.h>


using namespace muduo;

ThreadPool::ThreadPool(const std::string nameArg)
    : mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_),
      name_(nameArg),
      maxQueueSize_(0),
      running_(false)
{}

ThreadPool::~ThreadPool()
{
    if (this->running_)
        stop();
}

size_t ThreadPool::queueSize() const
{
    MutexLockGuard lock(mutex_);
    return queue_.size();
}

/**
 * 这个函数需要在获得了锁之后再进行调用
*/
bool ThreadPool::isFull() const
{
    mutex_.assertLocked();
    return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}


void ThreadPool::start(int numThreads)
{
    /**
     * 在开始执行线程之前，当前的线程池一定是空的
    */
    assert(this->threads_.empty());
    this->running_ = true;
    this->threads_.reserve(numThreads);

    for (int i = 0; i < numThreads; ++i)
    {
        char id[32];
        snprintf(id, sizeof id, "%d", i + 1);
        this->threads_.emplace_back(
            new muduo::Thread(std::bind(&ThreadPool::runInThread, this), this->name_ + id)
        );
        this->threads_[i]->start();
    }

    if (numThreads == 0 && threadInitCallback_)
     this->threadInitCallback_();
}

void ThreadPool::stop()
{
    {
        MutexLockGuard lock(this->mutex_);
        this->running_ = false;
        this->notEmpty_.notify_all();
        this->notFull_.notify_all();
    }

    for (auto& thr : this->threads_)
        thr->join();
}

/**
 * 一个线程池中的线程将会循环执行这里面的内容，不断的从 queue 当中读出待执行的函数，进行执行
*/
void ThreadPool::runInThread()
{
    try 
    {
        if (this->threadInitCallback_)
            this->threadInitCallback_();
        while(this->running_)
        {
            Task task(this->take());
            if (task)
                task();
        }
    }
    catch(const Exception& ex)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
        abort();
    }
    catch (const std::exception& ex)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch(...)
    {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
        throw; // rethrow
    }
}


ThreadPool::Task ThreadPool::take()
{
    MutexLockGuard lock(this->mutex_);
    while(this->queue_.empty() && this->running_)
        this->notEmpty_.wait();
    Task task;
    if (!this->queue_.empty())
    {
        task = queue_.front();
        queue_.pop_front();

        if (maxQueueSize_ > 0)
            notFull_.notify_all();
    }

    return task;
}


/**
 * 向我们的线程池当中添加任务
 * 执行添加任务的线程和线程池当中的线程之间是独立的
*/

void ThreadPool::run(Task task)
{
    /**
     * 线程池里面没有线程，那么就无法使用线程池来执行我们的任务，那么就直接执行这个任务
    */
    if (this->threads_.empty())
        task();
    else 
    {
        MutexLockGuard lock(this->mutex_);
        while(this->isFull() && this->running_)
        {
            /**
             * 线程池当中的任务队列满了，没有办法将新的任务添加进去，那么我们就阻塞等待可以插入的时候
            */
            notFull_.wait();
        }
        if (this->running_ == false) return;    /**线程池被终止了，那么任务就不添加进去了*/

        assert(!this->isFull());
        
        this->queue_.push_back(std::move(task));
        notEmpty_.notify_all();
    }
}