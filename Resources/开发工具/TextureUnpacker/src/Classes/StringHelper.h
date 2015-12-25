#ifndef __STRINGHELPER_H__
#define __STRINGHELPER_H__

#include <limits>
#include <vector>
#include <string>

namespace stringhelper
{
	/**
	 * 字符串分割
	 * @param std::string 源字符串
	 * @param const char* 分隔符
	 * @param size_t 最大分割数
	 * @return std::vector<std::string> 分割后的字符串
	 */
	std::vector<std::string> Split(const std::string &str, const char *sep, size_t maxsplit = std::numeric_limits<size_t>::max());

	/**
	 * 字符串反向分割
	 * @param std::string 源字符串
	 * @param const char* 分隔符
	 * @param size_t 最大分割数
	 * @return std::vector<std::string> 分割后的字符串
	 */
	std::vector<std::string> RSplit(const std::string &str, const char *sep, size_t maxsplit = std::numeric_limits<size_t>::max());
}

#endif