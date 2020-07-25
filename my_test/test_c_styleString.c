#include "./C_style_string.h"


int main(int argc, char* argv[])
{
    char* str = argv[1];
    int len = strLength(str);
    printf("%d\n", len);
    exit(0);
}