# Types.h

```C
void memZero(void*, size_t)
```
* 将一段内存的内容清空

```C
template<typename To, typename From>
inline To implicit_cast(From const &f)
```
* 隐氏类型的转化，现在的C++ 标准中的 static_cast 已经支持这些

```C
template <typename To, typename From>
inline To down_cast(From* f)
```
* 将一个类的指针指向器子类的指针是不安全的形式，你无法确认一个子类的指针指向的是一个子类类型还是一个父类的类型因此你需要这个函数，在完成转化之前，查看转化的过程是否合法