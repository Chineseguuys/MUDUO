#include <time.h>
#include <stdio.h>
#include <stdlib.h>




int main(int argc, char* argv[], char* env[])
{
	time_t time_now = time(NULL);
	struct tm* localTime = localtime(&time_now);
	struct tm* gmTime = gmtime(&time_now);

	printf("%s \t%s \n", localTime->tm_zone, gmTime->tm_zone);
	printf("%ld\t%ld\n", localTime->tm_gmtoff, gmTime->tm_gmtoff);
	printf("%s\n%s\n", asctime(localTime), asctime(gmTime));

	return 0;
}