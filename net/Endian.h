#ifndef MUDUO_NET_ENDIAN_H
#define MUDUO_NET_ENDIAN_H

#include <stdint.h>
#include <endian.h>

namespace muduo
{
namespace net
{
namespace sockets
{

// the inline assembler code makes type blur,
// so we disable warnings for a while.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"

/**
 * 从主机的字节顺序转化为网络的字节顺序
*/
inline uint64_t hostToNetwork64(uint64_t host64)
{
	/**
	 * 所为的 be = big endian 大端存储
	 * 指的是在内存中，高位存储在较低的地址位置，低位存储在比较高的地址位置
	 * 小端存储和大端刚好是相反的
	 * TCP/IP 协议中规定，数值使用大端存储进行处理  
	 * 内存的存储是以字节为最小的单位进行的，不是比特
	*/
  	return htobe64(host64);
}

inline uint32_t hostToNetwork32(uint32_t host32)
{
  	return htobe32(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16)
{
  	return htobe16(host16);
}

inline uint64_t networkToHost64(uint64_t net64)
{
  	return be64toh(net64);
}

inline uint32_t networkToHost32(uint32_t net32)
{
  	return be32toh(net32);
}

inline uint16_t networkToHost16(uint16_t net16)
{
  	return be16toh(net16);
}

#pragma GCC diagnostic pop

}  // namespace sockets
}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_ENDIAN_H