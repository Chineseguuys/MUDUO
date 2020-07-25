#include <stdlib.h>
#include <stdio.h>


#ifndef _C_STYLE_STRING_
#define _C_STYLE_STRING_


int strLength(char* str)
{
	int count = 0;
	char* pos = str;
	while (*pos++ != '\0')
		++count;
	
	return count;
}




#endif