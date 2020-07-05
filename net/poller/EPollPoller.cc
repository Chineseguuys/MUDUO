#include "net/poller/EPollPoller.h"

#include "net/Channel.h"
#include "base/Logging.h"


#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>


using namespace muduo;
using namespace muduo::net;

// On Linux, the constants of poll(2) and epoll(4)
// are expected to be the same.
static_assert(EPOLLIN == POLLIN,        "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI,      "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT,      "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP,  "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR,      "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP,      "epoll uses same flag values as poll");

/**
 * epoll 不需要像 poll 那样对 channel 进行标号，因为 epoll 会将发生事件的 epoll_struct 自动排列在数据结构的最前面。查询中没有
 * 发生事件的排在后面
 * 
 * 下面的 const 用来标识 channel 的状态，表示 channel 是否已经加入了某一个 epoll 当中
*/
namespace 
{
const int kNew      = -1; /*新的，待加入的 channel*/
const int kAdded    = 1;  /*已经加入的节点*/
const int kDeleted  = 2;   /*需要被删除的节点*/ 
}

EPollPoller::EPollPoller(EventLoop* loop)
	: Poller(loop),
	  epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
	  events_(KInitEventListSize)	/*16 个*/
{
	if (epollfd_ < 0)
	/**
	 * 如果创建 epollfd_ 文件失败，将会产生一个致命的错误
	*/
	{
		LOG_SYSFATAL << "EPollPoller::EPollPoller";
	}
}

EPollPoller::~EPollPoller()
{
  	::close(epollfd_);
}

/*轮询*/
muduo::Timestamp muduo::net::EPollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
	LOG_TRACE << "fd total count " << channels_.size();
	int numEvents = ::epoll_wait(
		epollfd_,
		&*events_.begin(),	/*&events_[0]*/
		static_cast<int>(events_.size()),
		timeoutMs
	);
	/**
	 * 这个函数在第一次进行调用的时候，events_ 和 epollfd_ 完成了绑定。之后在此执行将不会再进行绑定。
	 * 如果使用 epoll_ctl() 函数进行修改，添加，删除的操作都会在 events_ 上面完成
	*/
	int savedErrno = errno;
	Timestamp now(Timestamp::now());

	if (numEvents > 0)
	{
		LOG_TRACE << numEvents  << " events happened";


		this->fillActiveChannels(numEvents, activeChannels);
		if (implicit_cast<size_t>(numEvents) == events_.size())
		{
			this->events_.resize(this->events_.size() * 2);
			/**
			 * 不理解？ 
			*/
		}
	}
	else if (numEvents == 0)
		LOG_TRACE << "nothing happened";
	else 
	{
		if (savedErrno != EINTR)
		{
			errno = savedErrno;
			LOG_SYSERR << "EPollPoller::poll()";
		}
	}

	return now;
}

void EPollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const 
{
	assert(implicit_cast<size_t>(numEvents) <= events_.size());
	for (int i = 0; i < numEvents; ++i)
	{
		Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
	#ifndef NDEBUG
		int fd = channel->fd();
		ChannelMap::const_iterator it  = channels_.find(fd);
		assert(it != channels_.end());
		assert(it->second == channel);
	#endif
		channel->set_revents(events_[i].events);
		activeChannels->push_back(channel);
	}
}

void EPollPoller::updateChannel(Channel* channel)
{
	Poller::assertInLoopThread();
	const int index = channel->index();
	LOG_TRACE << "fd = " << channel->fd()
		<< " events = " << channel->events() << " index = " << index;
	if (index == kNew || index == kDeleted)
	{
		// a new one, add with EPOLL_CTL_ADD
		int fd = channel->fd();
		if (index == kNew)
		{
			assert(channels_.find(fd) == channels_.end());
			channels_[fd] = channel;	/*在 channelmap 中添加或者更新的操作*/
		}
		else // index == kDeleted
		{
			assert(channels_.find(fd) != channels_.end());
			assert(channels_[fd] == channel);
		}

		channel->set_index(kAdded);
		update(EPOLL_CTL_ADD, channel);
	}
	else
	{
		// update existing one with EPOLL_CTL_MOD/DEL
		int fd = channel->fd();
		(void)fd;
		assert(channels_.find(fd) != channels_.end());
		assert(channels_[fd] == channel);
		assert(index == kAdded);
		if (channel->isNoneEvent()) /*如果一个 channel 不再处理任何的 socket 事件的时候，它应该被移除*/
		{
			update(EPOLL_CTL_DEL, channel);
			channel->set_index(kDeleted);
		}
		else
		{
			update(EPOLL_CTL_MOD, channel);
		}
	}
}

void EPollPoller::removeChannel(Channel* channel)
{
	/**
	 * removeChannel 必须要是对应的 eventloop 发起的
	*/
	Poller::assertInLoopThread();
	int fd = channel->fd();
	LOG_TRACE << "fd = " << fd;
	assert(channels_.find(fd) != channels_.end());
  	assert(channels_[fd] == channel);
  	assert(channel->isNoneEvent());
	/**
	 * 1。 要保证 channel 在 poll 的集合中
	 * 2。 要保证集合中的 channel 和 channel 要对应上
	 * 3。 要保证这个待删除的 channel 是没有事情干的
	*/
	int index = channel->index();
	assert(index == kAdded || index == kDeleted);
	size_t n = channels_.erase(fd);	/*从 channelmap 中移除一个 channel*/
	(void)n;
	assert(n == 1);

	if (index == kAdded)
		update(EPOLL_CTL_DEL, channel);
	channel->set_index(kNew);	/*channel 被移除，但是可能后面还有机会再加回来*/
}

void EPollPoller::update(int operation, Channel* channel)
{
	struct epoll_event event;
	memZero(&event, sizeof event);
	event.events = channel->events();
	event.data.ptr = channel;
	int fd = channel->fd();
	LOG_TRACE << "epoll_ctl op = " << EPollPoller::operationToString(operation)
    	<< " fd = " << fd << " event = { " << channel->eventsToString() << " }";
	if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
	{
		if (operation == EPOLL_CTL_DEL)
		{
			LOG_SYSERR << "epoll_ctl op =" << EPollPoller::operationToString(operation) << " fd =" << fd;
		}
		else
		{
			LOG_SYSFATAL << "epoll_ctl op =" << EPollPoller::operationToString(operation) << " fd =" << fd;
		}
	}
}

const char* EPollPoller::operationToString(int op)
{
	switch (op)
	{
		case EPOLL_CTL_ADD:
			return "ADD";
		case EPOLL_CTL_DEL:
			return "DEL";
		case EPOLL_CTL_MOD:
			return "MOD";
		default:
			assert(false && "ERROR op");
			return "Unknown Operation";
	}
}