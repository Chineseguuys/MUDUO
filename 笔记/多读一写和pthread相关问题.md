要实现多读一写，并且写线程要优先。可以采用下面的实现方式。
我们需要下面几个临界区间的状态量

1. AR 正在读的线程的个数
2. WR 正在等待的读线程的个数
3. AW 正在写的线程的个数（要么是 0 ，要么是 1）
4. WW 正在等待的写线程的个数

临界区的资源：

> 可能是一个缓冲区 ```buffer``` ，也有可能是一个文件描述符 ```filefd``` 

一个互斥锁 ：

```pthread_mutex_t mutex_```

两个条件变量：

```pthread_cond_t canRead_``` 存储等待读的线程

```pthread_cond_t canWrite_``` 存储等待写的线程


> 线程在临界区的内部执行的时候，不需要持有锁，临界区的保护的工作，全部交给几个状态量。线程在进入临界区之前，通过对状态量的判断，来决定自己的行为

## 具体实现

```C
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
    /**
     * 写线程是优先的，所以如果临界区内部有写线程，等待队列当中有写线程进行等待，
     * 那么这个读线程必须进行等待
    */
    while(AW + WW > 0) {
        WR++; /*等待读线程的个数 + 1*/
        pthread_cond_wait(&canRead_, &mutex_);
        WR--; /**这个线程被唤醒之后，等待的线程 - 1，并重新判断 while 条件*/
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
```

## 几种可能的情况

1. 没有写线程，有多个读线程先后到达（或者几乎同时到达）

这几个读线程可以同时进入到临界区进行读操作，不需要进行任何的等待和阻塞

2. 写线程当前占领了临界区，没有写线程在等待，有多个读线程在等待

在写线程离开临界区的时候，会通知所有的读线程去竞争锁。

3. 写进程当前占领了临界区，有写线程在进行等待，也有读线程进行等待（没有也一样）

当前的写进程释放锁之后，通知一个写线程来使用临界区，其他的继续阻塞

4. 读线程当前占领了临界区，有写线程在进行等待，在写线程到达之后，有读线程到达

读线程结束读取过程，唤醒一个等待的写线程进入临界区，读线程等待，直到所有的写线程执行完毕。

> 由于写线程的优先，那么写线程即使后到达，也会排列在等待读的线程的前面，读线程可能会遭遇饥饿的现象（读线程一直无法得到进入临界区的机会）


## 自旋锁与互斥锁

互斥锁有阻塞形式的，也有非阻塞形式的

```c
int pthread_mutex_lock(pthread_mutex_t* mutex);
```
阻塞形式的加锁的过程，线程要么可以进入临界区从而拥有锁，要么被阻塞在 ```mutex``` 上面，```pthread_mutex_t``` 会维护一个阻塞队列，存储被阻塞的线程，一旦有线程释放锁，将会从阻塞队列中选择线程唤醒进行执行

```c
int pthread_mutex_trylock(pthread_mutex_t* mutex);
```

非阻塞形式的加锁的过程。如果没有获取到锁，那么这个函数直接返回，标识加锁失败，线程可以尝试过一段时间重新获取锁，或者不断的循环查询获取锁


### 自旋锁

```c
#include <pthread.h>

pthread_spinlock_t lock;
int pshared;
int ret;

/* initialize a spin lock */
ret = pthread_spin_init(&lock, pshared); 
```

我们可以使用一个整数来对自旋锁初始的属性进行设置： ```pshared``` 有下面的三个可以选择的项：

```PTHREAD_PROCESS_SHARED``` ： 这个自旋锁可以被任何可以访问自旋锁所在内存空间的线程进行访问和操作。如果这个自旋锁的内存地址在多个进程之间进行共享，那么多个进程的线程可以对其进行访问

```PTHREAD_PROCESS_PRIVATE``` ： 这个自旋锁只能在创建它的进程中被使用，来自其他进程的访问都将是未定义的行为


```c
pthread_spinlock_t lock;
int ret;

ret = pthread_spin_lock(&lock); /* lock the spinlock */
```

> 如果可以获取到锁，函数调用会立刻返回，表示线程已经获得了锁。如果自旋锁已经被其他的线程所拥有。当前的线程不会睡眠，而是不断的循环检测获取锁的条件（自旋） 直到它可以得到锁。

> 自旋锁非常的适合于占用临界区的时间非常的短的线程，在这个条件下，一个线程等待的时间不会很长，如果使用互斥锁去睡眠，消耗的系统资源比较高，不划算。但是如果线程占用临界区的时间比较长，线程等待的时间比较长。使用自旋锁的话，会导致等待的线程长时间进行毫无意义的空转，实在消耗 CPU 的能力，这个时候推荐互斥锁


```c
#include <pthread.h>

pthread_spinlock_t lock;
int ret;

ret = pthread_spin_trylock(&lock); /* try to lock the spin lock */
```

使用 ```pthread_spin_trylock()``` 可以防止没有意义的空转，因为没有获取到锁，这个函数会立刻返回

## 信号量

1. 二进制信号量 ： 取值 0 或者 1 。功能和 ```mutex``` 类似
2. 计数信号量

### 使用信号量实现生产者消费者模型

```c
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
```

## 读写锁

> 读写锁的目的其是就是为了实现上面的多读一写的目的，自然可以使用上面的互斥量的实现结构来实现读写锁。
> ```pthread``` 封装了读写锁的实现

读写锁定允许并发读取和互斥写入受保护的共享资源。读写锁是一个可以锁定为读写模式的实体。要修改资源，线程必须首先获取互斥写锁。在释放所有读取锁之前，不允许排他写入锁。

数据库访问可以与读写锁同步。读写锁支持并发读取数据库记录，因为读取操作不会更改记录的信息。当要更新数据库时，写操作必须获得独占写锁

```c
#include <pthread.h>

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
/**
 * 这个属性值 pthread_rwlockattr_t attr 在函数 pthread_rwlock_init() 当中进行使用，下面会进行介绍
*/
```

和上面的 ```pthread_condattr_t``` , ```sem_init()```, 设置属性的内容一样。只用设置进程之间的共享属性就可以了。即读写锁是否在不同的进程之间可见，还是只在创建它的进程当中可见。默认的情况下，都是只在当前的进程当中可见

除了在初始化的时候进行指定，你也可以在初始化结束之后单独设置这个属性

```c
#include <pthread.h>
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);
```
```pshared``` 可以选择的值在上面讲自旋锁的时候进行了介绍，这里的使用方法是一样的


```c
#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, 
          const pthread_rwlockattr_t *restrict attr);

pthread_rwlock_t  rwlock = PTHREAD_RWLOCK_INITIALIZER;
```

## 获取读锁

```c
#include <pthread.h>

int  pthread_rwlock_rdlock(pthread_rwlock_t *rwlock );
```

这个锁在没有写线程在写，或者没有写线程在等待的时候，可以获取到读锁（这里说明了是写优先）

```c
#include <pthread.h>

int  pthread_rwlock_wrlock(pthread_rwlock_t *rwlock );
```

