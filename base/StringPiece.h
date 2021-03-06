#ifndef MUDUO_BASE_STRINGPIECE_H
#define MUDUO_BASE_STRINGPIECE_H

#include <string.h>
#include <iosfwd>

#include "base/Types.h"

namespace muduo
{

class StringArg	// copyable
{
public:
	StringArg(const char* str)
		:	str_(str)
	{}

	StringArg(const string& str)
		: str_(str.c_str())
	{}

	const char* c_str() const { return this->str_; }
private:
	const char* str_;
};

class StringPiece {
private:
	const char*			ptr_;
	int					length_;

public: /*浅拷贝*/
	StringPiece()
		: ptr_(NULL), length_(0)
	{}
	StringPiece(const char* str)
		: ptr_(str), length_(static_cast<int>(strlen(ptr_)))
	{}
	StringPiece(const unsigned char* str)
		: ptr_(reinterpret_cast<const char*>(str)), 
		length_(static_cast<int>(strlen(ptr_)))
	{}
	StringPiece(const string& str)
		: ptr_(str.data()), length_(static_cast<int>(str.size()))
	{}
	StringPiece(const char* offset, int len)
		: ptr_(offset), length_(len)
	{}

	const char* data() const { return ptr_; }
	int size() const { return length_; }
	bool empty() const { return length_ == 0; }
	const char* begin() const { return ptr_; }
	const char* end() const { return ptr_ + length_; }

	void clear() { ptr_ = NULL; length_ = 0; }
	void set(const char* buffer, int len) { ptr_ = buffer; length_ = len; }
	void set(const char* str) {
		ptr_ = str;
		length_ = static_cast<int>(strlen(str));
	}
	void set(const void* buffer, int len) {
		ptr_ = reinterpret_cast<const char*>(buffer);
		length_ = len;
	}

	char operator[](int i) const { return ptr_[i]; }

	void remove_prefix(int n) {	// 移除前缀
		ptr_ += n;
		length_ -= n;
	}

	void remove_suffix(int n) {	// 移除后缀
		length_ -= n;
	}

	bool operator==(const StringPiece& x) const {
		return ((length_ == x.length_) &&
				(memcmp(ptr_, x.ptr_, length_) == 0));
	}

	bool operator!=(const StringPiece& x) const {
		return !(*this == x);
	}

	#define STRINGPIECE_BINARY_PREDICATE(cmp,auxcmp)                             \
	bool operator cmp (const StringPiece& x) const {                           \
		int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_); \
		return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.length_)));          \
	}
	STRINGPIECE_BINARY_PREDICATE(<,  <);
	STRINGPIECE_BINARY_PREDICATE(<=, <);
	STRINGPIECE_BINARY_PREDICATE(>=, >);
	STRINGPIECE_BINARY_PREDICATE(>,  >);
	#undef STRINGPIECE_BINARY_PREDICATE

	int compare(const StringPiece& str) const {
		int r = memcmp(ptr_, str.ptr_, length_ < str.length_ ? length_ : str.length_);
		if (r == 0)
		{
			if (length_ < str.length_) r = -1;
			else if (length_ > str.length_) r = +1;
		}
		return r;
	}

	string as_string() const {
		return string(data(), size());
	}

	void CopyToString(string* target) const {
		target->assign(ptr_, length_);
	}

	// Does "this" start with "x"
	bool starts_with(const StringPiece& x) const {
		return ((length_ >= x.length_) && (memcmp(ptr_, x.ptr_, x.length_) == 0));
	}

};

} // namespace muduo


#ifdef HAVE_TYPE_TRAITS
template<> struct __type_traits<muduo::StringPiece> {
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};
#endif

std::ostream& operator << (const std::ostream& o, const muduo::StringPiece& piece);

#endif