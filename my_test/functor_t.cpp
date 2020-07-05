#include <iostream>
#include <functional>

using namespace std;


typedef std::function<void ()> Functor;

void func(Functor&& cb)
{
    cb();
}

struct A
{
public:
    void p()
    {
        cout << "p" << endl;
    }
};

int main(int argc, char* argv[], char* env[])
{
    A a;
    void (A::*fp)() = &A::p;
    func(std::bind(fp, &a));
    return 0;
}