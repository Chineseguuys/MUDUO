#include<pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


void *child_thread(void *arg)
{
    printf("child thread run!\n");
}



int main(int argc,char *argv[ ])

{

    pthread_t tid;

    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

    pthread_create(&tid,&attr,child_thread,NULL);
    sleep(2);
    pthread_attr_destroy(&attr);
    printf("Have Sleeped 1 s\nMain will returned\n");
    return 0;
}