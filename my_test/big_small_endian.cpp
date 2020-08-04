#include <stdint.h>
#include <endian.h>
#include <stdio.h>




int main(int argc, char* argv[]) {
    uint32_t value = 0xff55ff55;
    uint32_t new_value = htobe32(value);

    printf("Done\n");
    return 0;
} 