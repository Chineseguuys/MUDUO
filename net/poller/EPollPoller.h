#ifndef MUDUO_NET_POLLER_EPOLLPOLLER_H
#define MUDUO_NET_POLLER_EPOLLPOLLER_H

#include "net/Poller.h"

#include <vector>

struct epoll_event;
/**
 *     typedef union epoll_data {
        void *ptr;
         int fd;
         __uint32_t u32;
         __uint64_t u64;
     } epoll_data_t;//保存触发事件的某个文件描述符相关的数据

     struct epoll_event {
         __uint32_t events;
         epoll_data_t data; 
    }
*/

namespace muduo
{
namespace net
{

class EPollPoller : public Poller 
{
public:
 public:
     EPollPoller(EventLoop* loop);
    ~EPollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;
private:
    static const int KInitEventListSize = 16;
    static const char* operationToString(int top);

    void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;
    void update(int operation, Channel* channel);

    typedef std::vector<struct epoll_event> EventList;

    int epollfd_;
    EventList events_;
};

} // namespace net

} // namespace muduo


#endif