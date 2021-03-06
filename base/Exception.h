#ifndef MUDUO_BASE_EXCEPTION_H
#define MUDUO_BASE_EXCEPTION_H

#include "base/Types.h"
#include <exception>

namespace muduo
{

class Exception : public std::exception
{
public:
    Exception(string what);

    ~Exception() noexcept override = default;

    const char* what() const noexcept override
    {
        return message_.c_str(); 
    }

    /**
     * 多了一个堆栈信息
    */
    const char* stackTrace() const noexcept
    {
        return stack_.c_str();
    }

private:
    string message_;
    string stack_;
};

} // namespace muduo




#endif