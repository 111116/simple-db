#include <algorithm>
#include <regex>
#include "tools.h"

// 对 MySQL 语句进行分词
// 分词后的 tokens 放进一个 std::vector<std::string> 中后返回
std::vector<std::string> split(std::string raw)
{
	raw.push_back('\n');
	std::vector<std::string> res;

	// accept "-?[0-9]+(\.[0-9]*)?" as number
	// accept "[_A-Za-z][_0-9A-Za-z]*" as identifier
	// accept "\'([^\'\\]|(\'\')|(\\.))*\'" as string
	// accept "\"([^\"\\]|(\"\")|(\\.))*\"" as string
	// accept "[-\+\*\/<>=,;\(\)<>=]" as separator

	// TODO
	return res;
}

// 将字符串 str 中的大写字母转换为小写，并返回转换后的字符串
std::string stringToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
