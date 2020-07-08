#include "examples/cdns/Resolver.h"
#include "net/EventLoop.h"
#include <stdio.h>


using namespace muduo;
using namespace muduo::net;
using namespace cdns;


EventLoop*      g_loop;
int             count = 0;
int             total = 0;

void quit()
{
    g_loop->quit();
}

void resolveCallback(const string& host, const InetAddress& addr)
{
    printf("resolveCallback %s -> %s\n", host.c_str(), addr.toIpPort().c_str());
    if (++count == total)
        quit();
}


void resolve(Resolver* res, const string& host)
{
    res->resolve(host, std::bind(&resolveCallback, host, _1));
}


int main(int argc, char* argv[], char* env[])
{
    EventLoop loop;
    loop.runAfter(10, quit);
    g_loop = &loop;
    Resolver resolver(&loop, argc == 1 ? Resolver::KDNSonly : Resolver::KDNSandHostsFile);

    if (argc == 1)
    {
        total = 3;
        resolve(&resolver, "www.baidu.com");
        resolve(&resolver, "www.google.com");
        resolve(&resolver, "www.example.com");
    }
    else 
    {
        total = argc - 1;
        for (int i = 1; i < argc; ++i)
            resolve(&resolver, argv[i]);
    }

    loop.loop();
}