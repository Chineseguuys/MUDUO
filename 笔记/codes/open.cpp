#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/uio.h>  // iovec

char buf[64];
char extra_buf[1024 * 1024];

void init_iovec(iovec*);
void init_iovec2(iovec (&)[2]);

int main(int argc, char* argv[])
{
    const char* file_name = "open.cpp";
    int fd = open(file_name, O_RDONLY);
    iovec bufs[2];
    init_iovec(bufs);
    int read_len  = readv(fd, bufs, 2);
    if (read_len > 0){
        printf("%s", bufs[0].iov_base);
        printf("%s\n", bufs[1].iov_base);
    }
    else 
    {
        printf("error");
    }
    close(fd);
    return 0;
}


void init_iovec(iovec* iov)
{
    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof buf;

    iov[1].iov_base = extra_buf;
    iov[1].iov_len = sizeof extra_buf;
}

void init_iovec2(iovec (&iov) [2])
{
    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof buf;

    iov[1].iov_base = extra_buf;
    iov[1].iov_len = sizeof extra_buf;
}