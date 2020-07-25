#include <memory>
#include <iostream>

using namespace std;


struct Data
{
public:
    Data() {}
    ~Data() { cout << "~Data()" << endl; }
};

void test(shared_ptr<Data> int_store)
{
    cout << int_store.use_count() << endl;
    int_store.reset();
}



int main()
{
    shared_ptr<Data> int_store(new Data());
    cout << int_store.use_count() << endl;
    test(int_store);
    cout << (bool)int_store << endl;
    return 0;
}