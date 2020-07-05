#ifndef MUDUO_NET_POLLER_POLLPOLLER_H
#define MUDUO_NET_POLLER_POLLPOLLER_H


#include "net/Poller.h"
#include <vector>

struct pollfd;
/**
 * struct pollfd{
　　int fd;              //文件描述符
　　short events;    //请求的事件
　　short revents;   //返回的事件
　　};

 * events 表示调用着感兴趣的 fd 的事件，其中主要包含了下面的几个选项
 *  POLLIN ： 可以读取非高优先级的数据
 *  POLLPRI ： 可以读取高优先级的数据
 *  POLLREHUB : 对端套接字关闭
 *  POLLOUT : 普通数据可写
 *  POLLWRBAND ：优先级数据可写入
 * 
 * 当 poll 返回的时候，revents 被设置为表示该文件描述符上实际发生的事件：
 * 除了包含上面 events 中可以设置的事件以外，revents 还包含了若干个错误情况
 *   POLLERR : 有错误发生
 *   POLLHUP : 出现挂断
 *   POLLNVAL : 文件描述符没有打开 
*/

namespace muduo
{
namespace net
{

class PollPoller : public Poller  
{
public:
    PollPoller(EventLoop* loop);
    ~PollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;
private:
    typedef std::vector<struct pollfd>  PollFdList;
    PollFdList      pollfds_;

    void fillActiveChannles(int numEvents, ChannelList* activeChannels) const;
};

} // namespace net
    
}


#endif