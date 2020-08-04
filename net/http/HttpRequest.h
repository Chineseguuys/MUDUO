#ifndef MUDUO_NET_HTTP_HTTPREQUEST_H
#define MUDUO_NET_HTTP_HTTPREQUEST_H



#include "base/copyable.h"
#include "base/Timestamp.h"
#include "base/Types.h"


#include <map>
#include <assert.h>
#include <stdio.h>

namespace muduo
{
namespace net 
{

class HttpRequest : public muduo::copyable {
public:
	enum Method {
		KInvalid, KGet, KPost, KHead, KPut, KDelete
	};
	enum Version {
		KUnknow, KHttp10, KHttp11
	};

private:
	Method		method_;
	Version		version_;
	string		path_;
	string		query_;
	Timestamp	receiveTime_;
	std::map<string, string> headers_;

public:
	HttpRequest() : method_(KInvalid), version_(KUnknow) {}

	void setVersion(Version v) { this->version_ = v; }

	Version getVersion() const { return this->version_; }

	Method method() const { return this->method_; }

	bool setMethod(const char* start, const char* end)
	{
		assert(method_ == KInvalid);
		string m(start, end);
		if (m == "GET")
		{
		method_ = KGet;
		}
		else if (m == "POST")
		{
		method_ = KPost;
		}
		else if (m == "HEAD")
		{
		method_ = KHead;
		}
		else if (m == "PUT")
		{
		method_ = KPut;
		}
		else if (m == "DELETE")
		{
		method_ = KDelete;
		}
		else
		{
		method_ = KInvalid;
		}
		return method_ != KInvalid;
	}

	const char* methodString() const
	{
		const char* result = "UNKNOWN";
		switch(method_)
		{
		case KGet:
			result = "GET";
			break;
		case KPost:
			result = "POST";
			break;
		case KHead:
			result = "HEAD";
			break;
		case KPut:
			result = "PUT";
			break;
		case KDelete:
			result = "DELETE";
			break;
		default:
			break;
		}
		return result;
	}

	void setPath(const char* start, const char* end)
	{
		path_.assign(start, end);
	}

	const string& path() const
	{ return path_; }

	void setQuery(const char* start, const char* end)
	{
		query_.assign(start, end);
	}

	const string& query() const
	{ return query_; }

	void setReceiveTime(Timestamp t)
	{ receiveTime_ = t; }

	Timestamp receiveTime() const
	{ return receiveTime_; }


	void addHeader(const char* start, const char* colon, const char* end) {
		string field(start, colon);
		++colon;
		while(colon < end && isspace(*colon))	++colon;
		string value(colon, end);
		while (!value.empty() && isspace(value[value.size() - 1]))
			value.resize(value.size() - 1);
		headers_[field] = value;
	}

	string getHeader(const string& field) const {
		string result;
		std::map<string, string>::const_iterator it = headers_.find(field);
		if (it != headers_.end()) 
			result = it->second;
		return result;
	}

	const std::map<string, string>& headers() const
	{ return headers_; }

	void swap(HttpRequest& that)
	{
		std::swap(method_, that.method_);
		std::swap(version_, that.version_);
		path_.swap(that.path_);
		query_.swap(that.query_);
		receiveTime_.swap(that.receiveTime_);
		headers_.swap(that.headers_);
	}
};

}
}




#endif;