#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>


#defnie FIFO  "./my_fifo"

int main(int argc, char* argv[]) {
    char buf_r[100];
    int fd;
    int nread;
    
}