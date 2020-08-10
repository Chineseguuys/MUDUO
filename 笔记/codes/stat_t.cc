#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    const char* fileName =  "./istream.cpp";
    struct stat st;
    stat(fileName, &st);
    printf("file size = %ld\n", st.st_size);
    printf("IO buffer size = %ld\n", st.st_blksize);
    printf("Block size = %ld\n", st.st_blocks);

    int fd = open(fileName, O_RDWR);
    int size = lseek(fd, 0, SEEK_END);
    printf("file size = %ld\n", size);
    close(fd);
    return 0;
}