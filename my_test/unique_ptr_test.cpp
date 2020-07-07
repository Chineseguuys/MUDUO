#include <memory>
#include <stdio.h>


using namespace std;

struct Temp
{
public:
    Temp(){}
    void some_func() { printf("some_func()\n"); }
    ~Temp() { printf("have been deleted\n"); }
};


int main()
{
    Temp* temp = new Temp();
    {
        unique_ptr<Temp> temp_ptr(temp);
    }

    printf("temp : %p\n", temp);
    temp->some_func();
    return 0;
}