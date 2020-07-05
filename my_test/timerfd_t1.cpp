/********************************************************
* Filename: timerfd.c
* Author: zhangwj
* Desprition: a sample program of timerfd
* Date: 2017-04-17
* Warnning:
********************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>

#if 0
struct timespec {
    time_t tv_sec;                /* Seconds */
    long   tv_nsec;               /* Nanoseconds */
};

struct itimerspec {
    struct timespec it_interval;  /* Interval for periodic timer */
    struct timespec it_value;     /* Initial expiration */
};
#endif

#define EPOLL_LISTEN_CNT        256
#define EPOLL_LISTEN_TIMEOUT    500

#define LOG_DEBUG_ON 1

#ifdef LOG_DEBUG_ON
#define LOG_DEBUG(fmt, args...) \
    do {  \
        printf("[DEBUG]:");\
        printf(fmt "\n", ##args); \
    } while(0);
#define LOG_INFO(fmt, args...) \
    do { \
        printf("[INFO]:");\
        printf(fmt "\n", ##args); \
    } while(0);
#define LOG_WARNING(fmt, args...) \
    do { \
        printf("[WARNING]:");\
        printf(fmt "\n", ##args); \
    } while(0);
#else
#define LOG_DEBUG(fmt, args...)
#define LOG_INFO(fmt, args...)
#define LOG_WARNING(fmt, args...)
#endif
#define LOG_ERROR(fmt, args...) \
    do{ \
        printf("[ERROR]:");\
        printf(fmt "\n", ##args);\
    }while(0);

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

static int g_epollfd = -1;
static int g_timerfd = -1;
uint64_t tot_exp = 0;

static void help(void)
{
    exit(0);
}

static void print_elapsed_time(void)
{
    static struct timespec start;
    struct timespec curr;
    static int first_call = 1;
    int secs, nsecs;

    if (first_call) {
        first_call = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
            handle_error("clock_gettime");
    }

    if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1)
        handle_error("clock_gettime");

    secs = curr.tv_sec - start.tv_sec;
    nsecs = curr.tv_nsec - start.tv_nsec;
    if (nsecs < 0) {
        secs--;
        nsecs += 1000000000;
    }
    printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
}

void timerfd_handler(int fd)
{
    uint64_t exp = 0;

    read(fd, &exp, sizeof(uint64_t));
    tot_exp += exp;
    print_elapsed_time();
    printf("read: %llu, total: %llu\n", (unsigned long long)exp, (unsigned long long)tot_exp);

    return;
}

void epoll_event_handle(void)
{
    int i = 0;
    int fd_cnt = 0;
    int sfd;
    struct epoll_event events[EPOLL_LISTEN_CNT];

    memset(events, 0, sizeof(events));
    /**轮寻*/
    while(1)
    {
        /* wait epoll event */
        fd_cnt = epoll_wait(g_epollfd, events, EPOLL_LISTEN_CNT, EPOLL_LISTEN_TIMEOUT);
        /**
         * 该函数用于轮询 I/O 事件的发生
         * g_epollfd : 由 epoll_create 生成的 epoll 专用的文件描述符
         * events : 用于回传代理事件的数组（这个函数向 events 写入）
         * EPOLL_LISTEN_CNT ： 每次可以处理的事件数
         * EPOLL_LISTEN_TIMEOUT ： 等待 I/O 事件发生的超时值
         * 返回值 ： 成功的话返回发生的事件数，否则的话，返回 -1
        */
       /**
        * 没有事件发生的时候是阻塞的，有事件发生的时候会立刻处理事件
       */
        for(i = 0; i < fd_cnt; i++)
        {
            sfd = events[i].data.fd;
            if(events[i].events & EPOLLIN)
            {
                if (sfd == g_timerfd)
                {
                    timerfd_handler(sfd);
                }
            }
        }
    }
}

int epoll_add_fd(int fd)
{
    int ret;
    struct epoll_event event;

    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    /**
     * events表示感兴趣的事件和被触发的事件，可能的取值为：
        EPOLLIN：表示对应的文件描述符可以读；
        EPOLLOUT：表示对应的文件描述符可以写；
        EPOLLPRI：表示对应的文件描述符有紧急的数可读；

        EPOLLERR：表示对应的文件描述符发生错误；
        EPOLLHUP：表示对应的文件描述符被挂断；
        EPOLLET：    ET的epoll工作模式；
    */

    ret = epoll_ctl(g_epollfd, EPOLL_CTL_ADD, fd, &event);
    /**
     * 用于控制某个文件描述符上的事件，可以注册事件，修改事件，删除事件
     * EPOLL_CTL_ADD注册、EPOLL_CTL_MOD修改、EPOLL_CTL_DEL删除
     * 函数成功的执行，返回 0 失败的话返回 1
    */
    if(ret < 0) {
        LOG_ERROR("epoll_ctl Add fd:%d error, Error:[%d:%s]", fd, errno, strerror(errno));
        return -1;
    }

    LOG_DEBUG("epoll add fd:%d--->%d success", fd, g_epollfd);
    return 0;
}

int epollfd_init()
{
    int epfd;

    /* create epoll fd */
    epfd = epoll_create(EPOLL_LISTEN_CNT);
    /**
     * 该函数生成一个 epoll 的专用的描述符，其中的参数是指定生成描述符的最大的范围
     * 如果创建成功，返回一个 > 0 的文件描述符
    */
    if (epfd < 0) {
        LOG_ERROR("epoll_create error, Error:[%d:%s]", errno, strerror(errno));
        return -1;
    }
    g_epollfd = epfd;   /*全局的文件描述符*/
    LOG_DEBUG("epoll fd:%d create success", epfd);

    return epfd;
}

int timerfd_init()
{
    int tmfd;
    int ret;
    struct itimerspec new_value;

    new_value.it_value.tv_sec = 2;
    new_value.it_value.tv_nsec = 0;
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;
    /**
     * 第一次是两秒之后进行触发，之后每隔 1s 触发一次
    */

    tmfd = timerfd_create(CLOCK_MONOTONIC, 0);  /*相对时间*/
    if (tmfd < 0) {
        LOG_ERROR("timerfd_create error, Error:[%d:%s]", errno, strerror(errno));
        return -1;
    }

    ret = timerfd_settime(tmfd, 0, &new_value, NULL);
    if (ret < 0) {
        LOG_ERROR("timerfd_settime error, Error:[%d:%s]", errno, strerror(errno));
        close(tmfd);
        return -1;
    }

    if (epoll_add_fd(tmfd)) {
        close(tmfd);
        return -1;
    }
    g_timerfd = tmfd;

    return 0;
}

int main(int argc, char **argv)
{
    if (epollfd_init() < 0) {
        return -1;
    }

    if (timerfd_init()) {
        return -1;
    }

    /* event handle */
    epoll_event_handle();

    return 0;
}