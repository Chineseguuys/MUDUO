#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/eventfd.h>


int create_eventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK  | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        exit(2);
    }
    return evtfd;
}


int main()
{
    int evtfd = create_eventfd();
    uint64_t one;
    ::write(evtfd, &one, sizeof one);
    //::write(evtfd, &one, sizeof one);
    int ret = ::read(evtfd, &one, sizeof one);
    if (ret < 0)
        printf("error\n");
    ret = ::read(evtfd, &one, sizeof one); //不可以反复的读
    if (ret < 0)
        printf("error\n");
    ::close(evtfd);
}