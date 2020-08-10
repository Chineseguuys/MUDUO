#include <string>
#include <iostream>
#include <stdio.h>


using namespace std;

int main(int argc, char* argv[]) {
    string str_a = "This is a String\n";
    string str_b = str_a;

    char* a_ch_pointer = &str_a[0];
    char* b_ch_pointer = &str_b[0];
    printf("%0x, %0x\n", a_ch_pointer, b_ch_pointer);
    return 0;
}