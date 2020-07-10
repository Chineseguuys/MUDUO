#include "base/Logging.h"
#include "net/EventLoop.h"
#include "net/TcpClient.h"
#include "base/Timestamp.h"
#include "net/TcpConnection.h"

#include <stdlib.h>
#include <memory>
#include <string>
#include <functional>

std::string contents;

void receiveFile(const muduo::net::TcpConnectionPtr& conn, 
        muduo::net::Buffer* inputbuffer, 
        muduo::Timestamp receivedTime)
{
    LOG_INFO << "receiveFile";
    std::string temp = inputbuffer->retrieveAllAsString();
    contents.append(temp);
}

void connectionCallback(const muduo::net::TcpConnectionPtr& conn)
{
    if (conn->disconnected())
    {
        LOG_INFO << "Client disconnected";
        muduo::net::EventLoop* loop = conn->getLoop();
        loop->runAfter(5.0, std::bind(&muduo::net::EventLoop::quit, loop));
    }
}

int main(int argc, char* argv[], char* env[])
{
    muduo::net::EventLoop loop;
    muduo::net::InetAddress serverAddr("127.0.0.1", 2021);
    muduo::net::TcpClient client (&loop, serverAddr, "TcpClient");
    client.setMessageCallback(receiveFile);
    client.setConnectionCallback(connectionCallback);
    client.connect();
    loop.loop();

    printf("%s\n", contents.c_str());
    return 0;
}