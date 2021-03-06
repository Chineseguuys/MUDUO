#ifndef MUDUO_BASE_TYPES_H
#define MUDUO_BASE_TYPES_H

#include <stdint.h>
#include <string.h>
#include <string>

#ifndef NDEBUG
#include <assert.h>
#endif

namespace muduo
{
	
using std::string;

inline void memZero(void* p, size_t n)
{
	memset(p, 0, n);
}

template <typename To, typename From>
inline To implicit_cast(From const &f)
{
	return f;
}

/**
 * 将一个类的指针指向器子类的指针是不安全的形式，你无法确认一个子类的指针指向的是一个子类类型还是一个父类的类型
 * 因此你需要这个函数，在完成转化之前，查看转化的过程是否合法
*/
template <typename To, typename From>
inline To down_cast(From* f)
{
	if (false)
	{
		implicit_cast<From*, To>(0);
	}

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
	assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif

	return static_cast<To>(f);
}

} // namespace muduo


#endif