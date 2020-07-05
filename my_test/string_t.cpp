#include <string>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[], char* env[])
{
	const char* str_one = "I am a String";
	char* str_two(const_cast<char*>(str_one));
	str_two[3] = 'C';	/**这样使用的话，就会发生段错误，上面的字符串构建的过程是浅拷贝*/
	/**编译器无法知道这种错误的存在，也不会发出任何的警告*/
	printf("%s\n", str_two);
	printf("%s\n", str_one);
	return 0;
}