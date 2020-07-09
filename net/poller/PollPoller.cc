#include "net/poller/PollPoller.h"
#include "base/Logging.h"
#include "net/Channel.h"


#include <assert.h>
#include <errno.h>
#include <poll.h>


using namespace muduo;
using namespace muduo::net;


/**
 * 具体的 poller 类属于一个 eventloop 对象
*/
PollPoller::PollPoller(EventLoop* loop)
	: Poller(loop)
{}

PollPoller::~PollPoller() = default;

Timestamp PollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
	int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
	/**
	 * 和 select 相同，每一次查询都需要重新进行 pollfd 的设置
	 * 
	 * timeoutMs 的设定，表示 poll 阻塞的方式
	 *  -1 ： 一直阻塞，直到所监视的 fd 当中有一个到达就绪或者捕获了一个信号
	 *  0 ： 不会阻塞，立刻返回
	 *  >0 : 阻塞一定的时间，该时间类没有事件到达，则超时返回
	*/
	int savedErrno = errno;

	Timestamp now(Timestamp::now());
	if (numEvents > 0)
	{
		LOG_TRACE << numEvents << " events happend";
		this->fillActiveChannles(numEvents, activeChannels);
	}
	else if (numEvents == 0)
	{
		LOG_TRACE << " nothing happended";
	}
	else 
	{
		if (savedErrno != EINTR)
		{
			errno = savedErrno;
			LOG_SYSERR << "PollPoller::poll()";
		}
	}
	return now;
}

void PollPoller::fillActiveChannles(int numEvents, ChannelList* activeChannels) const 
{
	/**
	 * @activeChannels 来自于 eventloop
	 * 即使有事件发生了，poll 也不知道是谁发生了，所以要对自己管理的所有的事件进行一次遍历循环
	 * 
	 * 一个 poller 处理一组 IO 的事件
	 * 一个 channel 负责处理一个 IO 的数据
	 * 
	 * 一个 eventloop 包含一个 poller 和若干个 channel
	*/
	for (PollFdList::const_iterator pfd = pollfds_.begin(); 
			pfd != pollfds_.end() && numEvents > 0; ++pfd)
	{
		if (pfd->revents > 0)
		{
			--numEvents;
			ChannelMap::const_iterator ch = channels_.find(pfd->fd);
			/**
			 * epoll 中还包含指针项，可以指向 channel* 对象，这使得 epoll 比 poll 又多了一个方便的地方，就是
			 * 不需要去集合中查询 channel 了，直接通过指针就可以找到 channel
			*/
			assert(ch != channels_.end());
			Channel* channel = ch->second;
			assert(channel->fd() == pfd->fd);
			channel->set_revents(pfd->revents); /*channel 如何处理这个事件，由 revents 的类型决定*/
			activeChannels->push_back(channel);
		}
	}
}

/**
 * 添加 监测的事件，或者修改所监测的事件的类型
*/
void PollPoller::updateChannel(Channel* channel)
{
	Poller::assertInLoopThread();
	LOG_TRACE << "fd=" << channel->fd() << " events = " << channel->events();
	if (channel->index() < 0)
	/**
	 * 新加入的 channel 没有分配 index, 初始的 index 是小于 0  的
	*/
	{
		assert(channels_.find(channel->fd()) == channels_.end());
		struct pollfd pfd;
		pfd.fd = channel->fd();
		pfd.events = static_cast<short>(channel->events());
		/**
		 * 设置这个 channel 感兴趣的事件
		*/
		pfd.revents = 0;
		pollfds_.push_back(pfd);
		int idx = static_cast<int>(pollfds_.size() - 1);
		channel->set_index(idx);
		channels_[pfd.fd] = channel;
	}
	else 
	{
		assert(channels_.find(channel->fd()) != channels_.end());
		assert(channels_[channel->fd()] == channel);
		int idx = channel->index();
   	 	assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
		struct pollfd& pfd  =pollfds_[idx];
		assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() - 1);
		pfd.fd = channel->fd();
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = 0;
		if (channel->isNoneEvent())
		{
			pfd.fd = -channel->fd() - 1;
			/**
			 * 文件描述符不存在负数，所以如果一个 pollfd 的 fd 设置为负数，那么意味着 channel 想要忽略这个文件描述符的事件
			*/
		}
	}
}

void PollPoller::removeChannel(Channel* channel)
{
	Poller::assertInLoopThread();
	LOG_TRACE << "fd = " << channel->fd();
	assert(channels_.find(channel->fd()) != channels_.end()); /**channel 要在当前的 poll 的 channel 列表当中*/
  	assert(channels_[channel->fd()] == channel);
  	assert(channel->isNoneEvent()); /*要被删除的 channel 必须是没有 event 的 channel*/
	
	int idx = channel->index();
	assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
	const struct pollfd& pfd = pollfds_[idx]; (void)pfd;
	
	assert(pfd.fd == -channel->fd() - 1  && pfd.events == channel->events());
	size_t n = channels_.erase(channel->fd());
	assert(n == 1); (void)n;
	if (implicit_cast<size_t>(idx) == pollfds_.size() -1)
	{
		/**
		 * 如果待删除的 channel 在整个 channel 列表的末尾的位置，就可以直接删除
		*/
		pollfds_.pop_back();
	}
	else 
	{
		/**
		 * 待删除的 channel 不在末尾的话，就和末尾的交换位置，然后再从末尾的位置删除
		*/
		int channelAtEnd = pollfds_.back().fd;
		std::iter_swap(pollfds_.begin() + idx, pollfds_.end() - 1);
		if (channelAtEnd < 0)
			channelAtEnd = - channelAtEnd - 1;
		channels_[channelAtEnd]->set_index(idx);
		pollfds_.pop_back();
	}
}