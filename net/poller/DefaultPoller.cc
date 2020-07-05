#include "net/Poller.h"
#include "net/poller/PollPoller.h"
#include "net/poller/EPollPoller.h"



#include <stdlib.h>

using namespace muduo::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
	if (::getenv("MODUO_USE_POLL"))
		return new PollPoller(loop);
	else 
		return new EPollPoller(loop);
}