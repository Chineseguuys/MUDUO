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