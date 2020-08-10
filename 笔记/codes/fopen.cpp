#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char buf[1024 * 1024];  // 1Mb


int main(int argc, char* argv[])
{
    const char* file_name = "open.cpp";
    FILE* file_stream = fopen(file_name, "r");
    size_t len = fread(buf, sizeof buf, 1, file_stream);

    if (len == 0)
    {
        printf("%s\n", buf);
    }
    else if (len > 0)
    {
        printf("%zu\n%s\n", len, buf);
    }
    else 
        printf("error\n");
    
    fclose(file_stream);
    return 0;
}