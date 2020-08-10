#ifndef _CODE_C_EXAMPLES
#define _CODE_C_EXAMPLES


#include "base/Logging.h"
#include "net/Buffer.h"
#include "net/Endian.h"
#include "net/TcpConnection.h"


class LengthHeaderCodec : public muduo::noncopyable
{
public:
    typedef std::function<void (const muduo::net::TcpConnectionPtr&, 
            const muduo::string& message,
            muduo::Timestamp)> StringMessageCallback;
private:
    StringMessageCallback messageCallback_;
    const static size_t kHeaderLen = sizeof(int32_t);

public:
    explicit LengthHeaderCodec(const StringMessageCallback& cb)
        : messageCallback_(cb)
    {
    }
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf,
                 muduo::Timestamp receiveTime)
    {
        while (buf->readableBytes() >= kHeaderLen)
        {
            const void* data = buf->peek();
            int32_t be32 = *static_cast<const int32_t*>(data);
            /**
             * 先读取消息头，查看消息的长度是多少
            */
            const int32_t len = muduo::net::sockets::networkToHost32(be32);
            if (len > 65536 || len < 0)
            {
                LOG_ERROR << "Invalid length " << len;
                conn->shutdown();
                break;
            }
            else if (buf->readableBytes() >= len + kHeaderLen)
            {
                buf->retrieve(kHeaderLen);
                muduo::string message(buf->peek(), len);
                messageCallback_(conn, message, receiveTime);
                buf->retrieve(len);
            }
            else 
                break;
                /**
                 * 当前的 buf 当中没有消息头部指定的那么多的数据（消息非常的长，还有部分的数据没有发送过来） 
                 * 那么这一次的可读事件，就不读去任何的数据，等待剩下的消息的达到
                */
        }
    }

    void send(muduo::net::TcpConnection* conn, const muduo::StringPiece& message)
    {
        muduo::net::Buffer buf;
        buf.append(message.data(), message.size()); /*深拷贝*/
        int32_t len = static_cast<int32_t>(message.size());
        int32_t be32 = muduo::net::sockets::hostToNetwork32(len);
        /**
         * 每一个消息发送出去之后，会在这个消息的前面加上这个消息的长度信息
        */
        buf.prepend(&be32, sizeof be32);
        conn->send(&buf); /**非阻塞也不需要担心，函数结束之后 buf 失效的问题，在 conn->send() 当中将其转换为了 string*/
    }
};


#endif