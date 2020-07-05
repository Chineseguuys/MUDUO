#include "myself_muduo/base/Timestamp.h"

#include <stdio.h>

using muduo::Timestamp;


int main()
{
    Timestamp now(Timestamp::now());
    printf("%s\n", now.to_string().c_str());
    return 0;
}
