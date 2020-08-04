# select 使用

## 函数原型

```c
int select( int n,   /**所有监测的文件描述符的最大值 + 1*/
            fd_set* readfds, 
            fd_set* writefds,
            fd_set* exceptfds,
            struct timerval* timeout);
```

### 三类文件描述符

1. readfds   集合中监测的文件描述符，确定其中是否有可读的数据

2. writefds  集合中监测的文件描述符，确定其中是否有一个写操作可以无阻塞的完成

3. exceptefds 集合中监测的文件描述符，确定其中是否有异常出现，或者出现外带的数据（out-of-band,这种情况只适合于套接字)

这个函数如果成功的调用，那么每一个集合当中只会返回对应的类型 I/O 就绪的文件描述符。比方说，7，9 两个文件描述符添加到了读文件监测当中，那么如果 7 文件描述符发生了可以读的事件，那么函数返回之后，readfds 中只会包含 7，9 就已经被移除了


```c
fd_set writefds;
FD_ZERO(&writefds);
```

> select() 函数每一次执行完之后都需要就文件描述符中的所有的文件全部移除，然会重新进行设置。

```c
FD_SET(fd, &writefds); /*添加一个文件描述符添加到集合当中*/
FD_CLR(fd, &writefds); /*从集合中移除一个文件描述符*/
FD_ISSET(fd, &readfds); /*查看一个文件描述符是否在集合当中, 通常在select函数返回的时候，进行查询*/
```

## 一个简单的例子

```c
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main(int argc, char* argv[]) {
    struct timeval tv;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    fd_set  readfds;
    int     ret;

    FD_ZERO(&readfds);
    FD_SET(STDOUT_FILENO, &readfds);

    ret = select(STDOUT_FILENO + 1, &readfds, NULL, NULL, &tv);

    if (ret == -1) {
        perror("select");
        return 1;
    }
    else if (ret == 0) {
        printf("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }

    if (FD_ISSET(STDOUT_FILENO, &readfds)) {
        char buf[BUF_LEN + 1];
        int len;
        len = read(STDOUT_FILENO, buf, sizeof BUF_LEN);
        if (len == -1) {
            perror("read");
            return 1;
        }

        if (len) {
            buf[len] = '\0';
            printf("read : %s \n", buf);
        }

        printf("have %ld time left\n", tv.tv_sec);
        return 0;
    }

    fprintf(stderr, "This should ont happen!\n");
    return 1;
}
```
