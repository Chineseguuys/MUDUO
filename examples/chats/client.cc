#include "examples/chats/codec.h"

#include "base/Logging.h"
#include "base/Mutex.h"
#include "net/EventLoopThread.h"
#include "net/TcpClient.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;


class ChatClient : noncopyable
{
private:
    TcpClient           client_;
    LengthHeaderCodec   codec_;
    MutexLock           mutex_;
    TcpConnectionPtr    connection_;

private:
    /**
     * 连接成功之后，可以传输数据的回调
    */
    void onConnection(const TcpConnectionPtr& conn)
    {
        LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
        
        MutexLockGuard lock(mutex_);
        if (conn->connected())
            this->connection_ = conn;
        else 
            this->connection_.reset();
    }
    void onStringMessage(const TcpConnectionPtr&,
                         const string& message,
                         Timestamp)
    {
        printf("<<< %s\n", message.c_str());
    }

public:
    ChatClient(EventLoop* loop, const InetAddress& serverAddr)
        : client_(loop, serverAddr, "chatClient"),
          codec_(std::bind(&ChatClient::onStringMessage, this, _1, _2, _3))
    {
        client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
        client_.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
        client_.enableRetry();
    }

    void connect() { client_.connect(); }

    void disconnect() { client_.disconnect(); }

    void write(const StringPiece& message)
    {
        MutexLockGuard lock(mutex_);
        if (connection_)    /*如果连接还存在的话，才可以发送数据*/
        {
            codec_.send(get_pointer(connection_), message);
        }
    }
};


int main(int argc, char* argv[])
{
    LOG_INFO << "pid = " << getpid();
    if (argc > 2)
    {
        EventLoopThread loopThread;
        uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
        InetAddress serverAddr(argv[1], port);

        ChatClient clinet(loopThread.startLoop(), serverAddr);
        clinet.connect();
        std::string line;
        while( std::getline(std::cin, line))
        {
            clinet.write(line);
        }
        /**
         * 没有数据发送了
        */
        clinet.disconnect();
        CurrentThread::sleepUsec(1000 * 1000);
    }

    else 
        printf("Usege : %s host_ip port\n", argv[0]);
}