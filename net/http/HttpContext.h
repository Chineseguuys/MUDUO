#ifndef MUDUO_NET_HTTP_HTTPCONTEXT_H
#define MUDUO_NET_HTTP_HTTPCONTEXT_H

#include "base/copyable.h"
#include "net/http/HttpRequest.h"



namespace muduo
{
namespace net
{

class Buffer;

class HttpContext : public muduo::copyable {
public:
    enum HttpRequestParseState {
        kExpectRequestLine,
        kExpectHeaders,
        kExpectBody,
        kGotAll,
    };
private: 
    HttpRequestParseState   state_;
    HttpRequest             request_;
private:
    bool processRequestLine(const char* begin, const char* end);
public:
    HttpContext()
        : state_(kExpectRequestLine)
    {
    }

    bool parseRequest(Buffer* buf, Timestamp receiveTime);

    bool gotAll() const
    { return state_ == kGotAll; }

    void reset()
    {
        state_ = kExpectRequestLine;
        HttpRequest dummy;
        request_.swap(dummy);
    }

    const HttpRequest& request() const
    { return request_; }

    HttpRequest& request()
    { return request_; }
};

} // namespace net

} // namespace muduo



#endif