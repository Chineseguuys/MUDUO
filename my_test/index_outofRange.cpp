#include <stdio.h>


int main(int argc, char** argv, char** env)
{
	int array[10];
	array[10] = 10;
	printf("%d\n", array[10]);
	return 0;
}