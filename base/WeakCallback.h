#ifndef MUDUO_BASE_WEAKCALLBACK_H
#define MUDUO_BASE_WEAKCALLBACK_H


#include <functional>
#include <memory>


namespace muduo
{
template <typename CLASS, typename... ARGS>
class WeakCallback
{
private:
    std::weak_ptr<CLASS> object_;   /**类的指针对象*/
    std::function<void (CLASS*, ARGS...)> function_;  /**类的成员函数*/
public:
    WeakCallback(const std::weak_ptr<CLASS>& object,
               const std::function<void (CLASS*, ARGS...)>& function)
    : object_(object), function_(function)
    {
    }

    void operator()(ARGS&&... args) const
    {
        std::shared_ptr<CLASS> ptr(object_.lock());
        /**
         * weak_ptr 管理的指针可能会在外部被销毁，lock() 未必会返回一个有效的指针
         * 已经被销毁的话，返回 NULL
        */
        if (ptr)
        {
            function_(ptr.get(), std::forward<ARGS>(args)...);
        }
    }    
};

/**
 * CLASS*::function 一种可能的传参方式是  &std::vector<int>::push_back
 * 类的成员函数指针
*/
template <typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(const std::shared_ptr<CLASS>& object, void (CLASS::*function)(ARGS...) const)
{
    return WeakCallback<CLASS, ARGS...>(object, function);
}

template<typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(const std::shared_ptr<CLASS>& object, void (CLASS::*function)(ARGS...))
{
    return WeakCallback<CLASS, ARGS...>(object, function);
}

} // namespace moduo


#endif