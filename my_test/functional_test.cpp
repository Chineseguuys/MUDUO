#include <functional>
#include <stdio.h>


using namespace std;

static int NUM = 100;

int main()
{
    typedef function<void (int&)> Functor;
    Functor fb();
    Functor(NUM);
    return 0;
}