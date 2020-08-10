#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BSIZE 1024

typedef struct {
    char buf[BSIZE];
    sem_t   occupied; /*当前的 buffer 当中有多少的数据*/
    sem_t   empty;   /*当前的 buffer 当中有多少的空闲的空间*/
    int     nextin;  /*下一个装载的位置*/
    int     nextout; /*下一个提取的位置*/
    sem_t   pmut;   /*生产者锁*/
    sem_t   cmut;   /*消费者锁*/
}buffer_t;


buffer_t buffer;


void producer(buffer_t* b, char item) {
    sem_wait(&b->empty);
    sem_wait(&b->pmut);
    /**
     * 原子操作
     * 当 pmut 的值为 0 的时候，这个函数会被阻塞，如果 pmut 的值大于 0 ，那么它 -- pmut，然后进入临界区
    */

    b->buf[b->nextin] = item;
    b->nextin += 1;
    b->nextin %= BSIZE;

    sem_post(&b->pmut); /**自动为 pmut + 1*/
    /**
     * 原子操作
     * 这个函数不会阻塞
    */
    sem_post(&b->occupied); /*自动为 occupied + 1， 表示 buffer 当中多了一个产品可以使用*/
}

char consumer(buffer_t* b) {
    char item;
    
    sem_wait(&b->occupied);
    sem_wait(&b->cmut);
    
    item = b->buf[b->nextout];
    b->nextout += 1;
    b->nextout %= BSIZE;

    sem_post(&b->cmut);
    sem_post(&b->empty);

    return item;
}

int main(int argc, char* argv[]) {
    sem_init(&buffer.occupied, 0, 0);
    /**
     * 当前 buffer 中产品的个数初始化为 0，开始的时候，buffer 当中没有产品
    */
    sem_init(&buffer.empty, 0, BSIZE); 
    /**
     * 当前 buffer 当中空闲的位置初始化为 BSIZE, 开始的时候 buffer 当中全部都是空的
    */
    sem_init(&buffer.pmut, 0, 1);
    /**
     * 这两个是二进制信号量，当作 mutex 互斥锁来进行使用，所以初始化为 1
    */
    sem_init(&buffer.cmut, 0, 1);

    buffer.nextin = buffer.nextout = 0;
}