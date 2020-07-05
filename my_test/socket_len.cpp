#include <netinet/in.h>
#include <stdio.h>

int main()
{
    printf("sizeof sockaddr \t %d\nsizeof sockaddr_in \t %d\nsizeof sockaddr_in6 \t %d\n",
        sizeof( struct sockaddr),
        sizeof( struct sockaddr_in),
        sizeof( struct sockaddr_in6));
    return 0;
}