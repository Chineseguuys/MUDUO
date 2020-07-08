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

void receiveFile(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* inputbuffer, muduo::Timestamp receivedTime)
{
    std::string temp = inputbuffer->retrieveAllAsString();
    contents.append(temp);
}

void quit(muduo::net::EventLoop* loop)
{
    loop->quit();
}

int main(int argc, char* argv[], char* env[])
{
    muduo::net::EventLoop loop;
    muduo::net::InetAddress serverAddr("127.0.0.1", 2021);
    muduo::net::TcpClient client (&loop, serverAddr, "TcpClient");
    client.setMessageCallback(receiveFile);
    client.connect();
    loop.loop();

    printf("%s\n", contents.c_str());
    return 0;
}