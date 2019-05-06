#include <algorithm>
#include "tools.h"

// 对 MySQL 语句进行分词
// 分词后的 tokens 放进一个 std::vector<std::string> 中后返回
std::vector<std::string> split(std::string raw)
{
	raw.push_back('\n');
	std::vector<std::string> res;
	std::string current;
	bool inString = false;
	char stringDelimiter; // 处理到字符串字面量中间时，该 stringDelimiter 变量保存所用的包围字符串的符号，为单引号或双引号

	auto split = [&]()
	{
		if (!current.empty())
		{
			res.push_back(std::move(current));
			current = "";
		}
	};
	for (int i=0; i<raw.length(); ++i)
	{
		if (inString)
		{
			if (raw[i] == '\n')
				throw "String literal missing terminating character";
			current += raw[i];
			if (raw[i] == stringDelimiter)
			{
				if (raw[i+1] == stringDelimiter)
				{
					current += stringDelimiter;
					i++;
				}
				else // 字符串字面量结束
				{
					inString = false;
				}
			}
			if (raw[i] == '\\' && raw[i+1] != '\n')
			{
				i++;
				current += raw[i];
			}
			continue;
		}
		// 不在字符串字面量中；在空白字符处分词
		if (raw[i] == ' ' || raw[i] == '\t' || raw[i] == '\n')
		{
			split();
			continue;
		}
		// 字符串字面量起始判断
		if (raw[i] == '\'' || raw[i] == '\"')
		{
			split();
			inString = true;
			stringDelimiter = raw[i];
			current = raw[i];
			continue;
		}
		// 减号（负号，连字符）、数字、大小写字母、下划线、小数点的连续组合被认为是一整个 token（可能是数字或标识符的名称）
		// "[-_0-9A-Za-z\.]+"
		if (isdigit(raw[i]) || isalpha(raw[i]) || raw[i] == '_' ||
			raw[i] == '-' || raw[i] == '.')
		{
			current += raw[i];
			continue;
		}
		// 其它特殊字符，分词
		split();
		res.push_back({raw[i]});
	}
	return res;
}

// 将字符串 str 中的大写字母转换为小写，并返回转换后的字符串
std::string stringToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
