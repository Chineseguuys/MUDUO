#include "net/Poller.h"
#include "net/Channel.h"



using namespace muduo;
using namespace muduo::net;


Poller::Poller(EventLoop* event)
	: ownerloop_(event)
{
}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const 
{
	this->assertInLoopThread();
	ChannelMap::const_iterator iter  = this->channels_.find(channel->fd());
	return iter != this->channels_.end() && iter->second == channel;
}