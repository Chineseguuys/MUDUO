#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

// 临界区
int file_fd;
int AW; // 正在写的线程的数量
int WW; // 正在等待的线程的数量
int AR; // 正在读的线程的数量
int WR; // 正在等待的读的线程的数量
pthread_mutex_t mutex_;
pthread_cond_t  canRead_;
pthread_cond_t  canWrite_;

void startRead() {
    pthread_mutex_lock(&mutex_);
    while(AW + WW > 0) {
        WR++;
        pthread_cond_wait(&canRead_, &mutex_);
        WR--;
    }
    AR++;
    pthread_mutex_unlock(&mutex_);
}


void endRead() {
    pthread_mutex_lock(&mutex_);
    AR--;
    if (AR == 0 && WW > 0)
        pthread_cond_signal(&canWrite_);
    pthread_mutex_unlock(&mutex_);
}


void canWrite() {
    pthread_mutex_lock(&mutex_);
    while (AR + AW > 0) {
        ++WW;
        pthread_cond_wait(&canWrite_, &mutex_);
        --WW;
    }
    ++AW;
    pthread_mutex_unlock(&mutex_);
}

void endWrite() {
    pthread_mutex_lock(&mutex_);
    --AW;
    if (WW > 0) {
        pthread_cond_signal(&canWrite_);
    }
    else if (WR > 0) {
        pthread_cond_broadcast(&canRead_);
    }
    pthread_mutex_unlock(&mutex_);
}

int main(int argc, char** argv) {
    AW = WW = AR = WR = 0;
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&canRead_, NULL);
    pthread_cond_init(&canWrite_, NULL);
    
    file_fd = open("./tempfile", O_RDWR | O_CREAT | O_CLOEXEC);

    // 多线程的创建以及，相关的读写操作

    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&canRead_);
    pthread_cond_destroy(&canWrite_);
    close(file_fd);
    return 0;
}