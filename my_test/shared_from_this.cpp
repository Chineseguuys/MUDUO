#include <iostream>
#include <memory>
 
struct Foo : public std::enable_shared_from_this<Foo> {
    Foo() { std::cout << "Foo::Foo\n"; }
    ~Foo() { std::cout << "Foo::~Foo\n"; } 
    std::shared_ptr<Foo> getFoo() { return shared_from_this(); }
};

int main() {
    Foo *f = new Foo;
    std::shared_ptr<Foo> pf1;

    {
        std::shared_ptr<Foo> pf2(f);
        pf1 = pf2->getFoo();  // 与 pf2 的对象共享所有权
        std::cout << "user count : " << pf1.use_count() << std::endl;
    }
    std::cout << "pf2 is gone\n";   
}