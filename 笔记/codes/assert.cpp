#include <stdio.h>
#include <assert.h>
#include <new>   // 如果你要使用 placement new，必须包含这个头文件

struct temp
{
public:
    temp() {}
    ~temp() { printf("dector temp\n"); }
};

double dev(double val_a, double val_b)
{
    assert(val_b != 0);
    return val_a / val_b;
}


int main(int argc, char* argv[])
{
    void* temp_ptr = operator new(sizeof(struct temp));
    // 等价于 void* temp_ptr = ::malloc(sizeof (struct temp));
    temp* t = static_cast<struct temp*>(new (temp_ptr) temp());

    if (temp_ptr == t)
        printf("right\n");

    double res = dev(12.3, 10.0);
    printf("%f\n", res);

    t->~temp();
    operator delete(temp_ptr);
    return 0;
}