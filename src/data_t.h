#pragma once

#include <string>
#include <iostream>

class data_t // 封装数据库支持的基本数据类型
{
public:
	static data_t* fromLiteral(std::string); // 根据字面值构造相应的 data_t 派生类对象
	virtual data_t* copy() = 0; // 复制当前 data_t 的内容到新的对象
	virtual bool operator < (const data_t&) const = 0; // 大小比较运算符
	virtual bool operator > (const data_t&) const;
	virtual bool operator == (const data_t&) const;
	virtual std::string get() const = 0; // 获取保存的数据的字符串表示
	virtual ~data_t();
};

class dataInt: public data_t
{
	friend class dataDouble; // 为支持 dataInt 和 dataDouble 之间的大小比较
private:
	int value;
public:
	dataInt(int);
	dataInt(std::string);
	dataInt* copy();
	bool operator < (const data_t&) const;
	using data_t::operator >;
	using data_t::operator ==;
	virtual std::string get() const;
};

class dataDouble: public data_t
{
	friend class dataInt; // 为支持 dataInt 和 dataDouble 之间的大小比较
private:
	double value;
public:
	dataDouble(double);
	dataDouble(std::string);
	dataDouble* copy();
	bool operator < (const data_t&) const;
	using data_t::operator >;
	using data_t::operator ==;
	virtual std::string get() const;
};

class dataString: public data_t
{
private:
	std::string value;
public:
	dataString(std::string);
	dataString(const dataString&);
	dataString* copy();
	bool operator < (const data_t&) const;
	using data_t::operator >;
	using data_t::operator ==;
	virtual std::string get() const;
};

std::ostream& operator<<(std::ostream&, const data_t&);
