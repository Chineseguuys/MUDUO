#include "base/Logging.h"
#include "net/EventLoop.h"
#include "net/TcpClient.h"
#include <stdlib.h>


using namespace muduo;
using namespace muduo::net;

TcpClient* g_client;


int main(int argc, char* argv[])
{
    EventLoop loop;
    InetAddress serverAddr("127.0.0.1", 1080);
    TcpClient clinet(&loop, serverAddr, "TcpClient");
    g_client = &clinet;

    loop.runAfter(15.0, std::bind(&EventLoop::quit, &loop));
    clinet.connect();
    loop.runAfter(10.0, std::bind(&TcpClient::disconnect, g_client));
    loop.loop();

    return 0;
}