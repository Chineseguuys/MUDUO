#include "base/CurrentThread.h"

#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

namespace muduo
{

/**
 * 用于保存线程的各种信息
 * id
 * 名称
 * 线程中堆栈的情况
*/
namespace CurrentThread
{

__thread int        t_cachedTid = 0;
__thread char 		t_tidString[32];
__thread int 		t_tidStringLength = 6;
__thread const char* 	t_threadName = "unknown";

/**
 * 在编译期间就查看 pid_t 的类型是否是 int 类型，不是的话，编译出错
*/
static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

string stackTrace(bool demangle)
{
	string stack;
	const int max_frames = 200;
	void* frame[max_frames];
	/**
	 * int backtrace(void** buffer, int size)
	 * 该函数用与获取当前线程的调用堆栈,获取的信息将会被存放在buffer中,它是一个指针数组。
	 * 参数 size 用来指定buffer中可以保存多少个void* 元素。
	 * 函数返回值是实际获取的指针个数,最大不超过size大小在
	 * buffer中的指针实际是从堆栈中获取的返回地址,每一个堆栈框架有一个返回地址
	*/
	int nptrs = ::backtrace(frame, max_frames);
	/**
	 * char** backtrace_symbols(void* const* buffer, int size)
	 * backtrace_symbols将从backtrace函数获取的信息转化为一个字符串数组. 
	 * 参数buffer应该是从backtrace函数获取的数组指针,
	 * size是该数组中的元素个数(backtrace的返回值)，
	 * 函数返回值是一个指向字符串数组的指针,它的大小同buffer相同.
	 * 每个字符串包含了一个相对于buffer中对应元素的可打印信息.
	 * 它包括函数名，函数的偏移地址,和实际的返回地址
	*/
	char** strings = ::backtrace_symbols(frame, nptrs);

	if (strings)
	{
		size_t len = 256;
		char*	demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;

		for (int i = 1; i < nptrs; ++i)
		{
			/**
			 * strings 中的每一条存储的都是堆栈上的信息，可能是下面的格式
			 * ./function_name(myfunc+0x21)
			 * ./function_name(myfunc+0x32)
			*/
			if (demangle)
			{
				char* left_par = nullptr;
				char* plus = nullptr;

				for (char* p = strings[i]; *p; ++p)
				{
					if (*p == '(')
						left_par = p;
					else if (*p == '+')
						plus = p;
				}

				if (left_par && plus)
				{
					*plus = '\0';
					/**
					 * 格式变成了 ./function_name(myfunc'\0'0x21)
					 * left_par + 1 = "myfunc"
					*/
					int status = 0;
					/**
					 * 获取 myfunc 完整的类型名称
					*/
					char* ret = abi::__cxa_demangle(left_par + 1, demangled, &len, &status);
					*plus = '+';
					/**
					 * 格式又恢复到了 ./function_name(myfunc+0x21)
					*/
					if (status == 0)
					{
						demangled = ret;
						stack.append(strings[i], left_par + 1);
						stack.append(demangled);
						stack.append(plus);
						stack.push_back('\n');
						continue;
					}
				}
			}
			stack.append(strings[i]);
			stack.push_back('\n');
		}
		free(demangled);
		free(strings);
	}
	return stack;
}

} // namespace CurrentThread


} // namespace muduo
