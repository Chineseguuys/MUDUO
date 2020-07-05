#include <stdio.h>
#include <stdint.h>
/**
 * 开辟负数的数组？
 * 
 * 编译期间报错
*/
int main(int argc, char* argv[], char* env[])
{
	char require[sizeof(int) >= sizeof(int32_t)? -1 : 1];

	return 0;
}