#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
 
int main(int argc, char *argv[])
{
	char *ptr, **pptr;
	struct hostent *hptr;
	char str[32] = {0};
	
	ptr = argv[1];
	
	if((hptr = gethostbyname(ptr)) == NULL)
	{
		printf("gethostbyname error: %s\n", ptr);
		return 0;
	}
	
	printf("official hostname:%s\n", hptr->h_name);   //主机规范名
	
	for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)   //将主机别名打印出来
		printf("alias: %s\n", *pptr);
	
	switch(hptr->h_addrtype)  //根据地址类型，将地址打印出来
	{
		case AF_INET:
		case AF_INET6:
			pptr = hptr->h_addr_list;
		
			for(; *pptr != NULL; pptr++)   //将得到的所有地址打印出来
			{
				printf("address: %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));   //inet_ntop: 将网络字节序的二进制转换为文本字符串的格式
				printf("first address: %s\n", inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));
			}
			break;
		default:
			printf("unkown address type\n");
			break;
	}
	
	return 0;
}