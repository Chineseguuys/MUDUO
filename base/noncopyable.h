#ifndef MUDUO_BASE_NONCOPYABLE_H
#define MUDUO_BASE_NONCOPYABLE_H

namespace muduo
{

/**
 * protecetd 类型的默认构造函数，意味着这个类不能单独进行实现，只能作为其他类的基类
 * 拷贝构造函数和复制都被设置为 delete。 则不可以进行拷贝
*/
class noncopyable
{
public:
	noncopyable(const noncopyable&) = delete;
	void operator=(const noncopyable&) = delete;

protected:
	noncopyable() = default;
	~noncopyable() = default;
};

} // namespace muduo


#endif