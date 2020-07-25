#include <functional>
#include <memory>

using namespace std;


typedef function<void ()> func;

struct Data
{
private:
    int number;
public:
    Data() : number(100) {}
    void func() { printf("test %d\n", number); }
};

void test(func cb)
{
    cb();
}

int main()
{
    Data* d = new Data();
    func f(std::bind(&Data::func, d));
    test(f);
    delete d;
    test(f);
    return 0;
}