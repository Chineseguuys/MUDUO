#ifndef MUDUO_NET_TCPCLIENT_H
#define MUDUO_NET_TCPCLIENT_H

#include "base/Mutex.h"
#include "net/TcpConnection.h"

namespace muduo
{
namespace net
{
class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : public noncopyable 
{
private:
    EventLoop*              loop_;
    ConnectorPtr            connector_;
    const string            name_;
    ConnectionCallback      connectionCallback_;
    MessageCallback         messageCallback_;
    WriteCompleteCallback   writeCompleteCallback_;

    bool    retry_;
    bool    connect_;
    id_t    nextConnId_;

    mutable MutexLock   mutex_;
    TcpConnectionPtr    connection_ GUARDED_BY(mutex_);    
private:
    void newConnection(int sockfd);
    void removeConnection(const TcpConnectionPtr& conn);
public:
    TcpClient(EventLoop* loop,
            const InetAddress& serverAddr,
            const string& nameArg);
    ~TcpClient();  // force out-line dtor, for std::unique_ptr members.

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const
    {
        MutexLockGuard lock(mutex_);
        return connection_;
    }

    EventLoop* getLoop() const { return loop_; }
    bool retry() const { return retry_; }
    void enableRetry() { retry_ = true; }

    const string& name() const
    { return name_; }

    /// Set connection callback.
    /// Not thread safe.
    void setConnectionCallback(ConnectionCallback cb)
    { connectionCallback_ = std::move(cb); }

    /// Set message callback.
    /// Not thread safe.
    void setMessageCallback(MessageCallback cb)
    { messageCallback_ = std::move(cb); }

    /// Set write complete callback.
    /// Not thread safe.
    void setWriteCompleteCallback(WriteCompleteCallback cb)
    { writeCompleteCallback_ = std::move(cb); }
};
} // namespace net

} // namespace muduo



#endif