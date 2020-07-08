#include "examples/cdns/Resolver.h"

#include "base/Logging.h"
#include "net/Channel.h"
#include "net/EventLoop.h"

#include <ares.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

using namespace muduo;
using namespace muduo::net;
using namespace cdns;


namespace
{
double getSeconds(struct timeval* tv)
{
    if (tv)
        return double(tv->tv_sec) + double(tv->tv_usec)/1000000.0;
    else
        return -1.0;
}

const char* getSocketType(int type)
{
    if (type == SOCK_DGRAM)
        return "UDP";
    else if (type == SOCK_STREAM)
        return "TCP";
    else
        return "Unknown";
}

const bool kDebug = false;
}  // namespace

