#include "data_t.h"

// 根据 C++ int 或 MySQL 字面值构造 dataInt 对象
dataInt::dataInt(int value): value(value) {}
dataInt::dataInt(std::string value): value(std::stoi(value)) {}

// 根据 C++ double 或 MySQL 字面值构造 dataDouble 对象
dataDouble::dataDouble(double value): value(value) {}
dataDouble::dataDouble(std::string value): value(std::stod(value)) {}

// dataString 的复制构造函数
dataString::dataString(const dataString& o): value(o.value) {}

// 根据单个 MySQL 字面值构造 dataString 对象
dataString::dataString(std::string str)
{
	value = "";
	if ((str[0] != '\'' && str[0] != '\"') || str[0] != str.back()) // 字符串字面量必须以相同的引号开始结束
		throw "Unrecognized string literal";
	for (unsigned k = 1; k < str.size() - 1; ++k)
	{
		if (str[k] == '\\') // 转义字符
		{
			++k;
			if (str[k] == '0') value.push_back('\0'); // 六个需要特殊处理的转义字符
			else if (str[k] == 'b') value.push_back('\b');
			else if (str[k] == 'n') value.push_back('\n');
			else if (str[k] == 'r') value.push_back('\r');
			else if (str[k] == 't') value.push_back('\t');
			else if (str[k] == 'Z') value.push_back('\x1a');
			else value.push_back(str[k]); // 其他情况取反斜杠后面的符号即可
		}
		else if (str[k] == str[0])
		{
			if (k + 1 == str.size() - 1 || str[k + 1] != str[0]) // 两个相同引号转义为一个（参考 MySQL 文档）
				throw "Unrecognized string literal";
			value.push_back(str[++k]);
		}
		else
			value.push_back(str[k]);
	}
}

// 默认的 data_t 派生类的大于号比较运算符
bool data_t::operator > (const data_t& b) const
{
	return b < *this;
}

// 默认的 data_t 派生类的等于号比较运算符
bool data_t::operator == (const data_t& b) const
{
	return !(*this < b) && !(b < *this);
}

// 用于简化小于号运算符编写的辅助宏，具体用法可参考下面的小于号的写法
//
// 假设比较表达式为 a<b。此处 dataType 表示 b 的类型，例如 dataInt、dataDouble。
// 如果 b 实际不为指定的类型，那么什么都不做。
//
// convertFunction 是一个将 value 值转换为实际参与比较的值的函数，
// 类似于 JavaScript 中 ToPrimitive 函数做的事情。在类似于「大小写不敏感的字符串大小比较」等场合下非常方便。
// 如果不需要转换，直接使用 value 的原始值，省略即可。
//
// 务必注意后面的 #undef compareHelper 预编译指令（防止命名污染），比较运算请一定在 #define 和 #undef 之间定义
#define compareHelper(dataType, convertFunction) \
{ \
	const dataType* dvar = dynamic_cast<const dataType*>(&b); \
	if (dvar) \
		return convertFunction(value) < convertFunction(dvar->value); \
}

// 小于号比较运算符，其中小于号左边为 dataInt 对象。
// dataInt 只能与 dataInt 和 dataDouble 相比较。
bool dataInt::operator < (const data_t &b) const
{
	compareHelper(dataInt, );
	compareHelper(dataDouble, double); // 由于 C++ 内建支持 int 和 double 的比较，这里实际上不需要转换。这种写法是一个示例。
	throw "Compare: data type mismatch"; // b 既不是 dataInt 也不是 dataDouble，就没法比较了
}

// 小于号比较运算符，其中小于号左边为 dataDouble 对象。
// dataDouble 只能与 dataInt 和 dataDouble 相比较。
bool dataDouble::operator < (const data_t &b) const
{
	compareHelper(dataInt, double);
	compareHelper(dataDouble, );
	throw "Compare: data type mismatch";
}

// 小于号比较运算符，其中小于号左边为 dataString 对象。
// dataString 只能与 dataString 相比较。
bool dataString::operator < (const data_t &b) const
{
	compareHelper(dataString, ); // 由大作业题目要求，此处字符串比较区分大小写，与 MySQL 默认情形不同
	throw "Compare: data type mismatch";
}

#undef compareHelper

// 将 dataInt 对象的内容转换为字符串并返回
std::string dataInt::get() const
{
	return std::to_string(value);
}

// 将 dataDouble 对象的内容转换为字符串并返回
std::string dataDouble::get() const
{
	return std::to_string(value);
}

// 将 dataString 对象的内容（本身就是字符串）返回
std::string dataString::get() const
{
	return value;
}

// 根据字面值构造 data_t 对象，并返回指向构造的对象的指针
// TODO 此处假定输入的字面值都是合法的（对于不合法的字面值，引发问题的部分会被忽略）
data_t* data_t::fromLiteral(std::string str)
{
	if (str[0] == '\'' || str[0] == '\"') // 引号打头，即为字符串
		return new dataString(str);
	else
	{
		if (str.length() <= 10 || (str.length() == 11 && str[0] == '-')) // int 类型的十进制字面量长度在 10（非负）或 11（负）以内
		{
			long long value = 0, flag = 1;
			for (unsigned i = 0; i < str.length(); ++i)
			{
				if (str[i] == '-' && i == 0) // 开头负号
					flag = -flag;
				else if (!isdigit(str[i])) // 非数字字符
				{
					value = 1ll << 60; // 1ll << 60 是个 magic number，它显然不能在 int 范围内装下，break 后会自动按 double 返回
					break;
				}
				else
					(value *= 10) += str[i] - '0'; // 拼接数位
			}
			value *= flag; // 处理符号
			if (value >= (int)0x80000000 && value <= (int)0x7fffffff)
				return new dataInt(str); // int
		}
		return new dataDouble(str); // double
	}
}

// 获取一个新的、具有相同值的 dataInt 对象
dataInt* dataInt::copy()
{
	return new dataInt(this->value);
}

// 获取一个新的、具有相同值的 dataDouble 对象
dataDouble* dataDouble::copy()
{
	return new dataDouble(this->value);
}

// 获取一个新的、具有相同值的 dataString 对象
dataString* dataString::copy()
{
	return new dataString(*this);
}

data_t::~data_t() {}

std::ostream& operator<<(std::ostream& o, const data_t& data)
{
	return o << data.get();
}
