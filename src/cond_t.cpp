#include "cond_t.h"

// 返回一个永远返回 value 的函数
cond_t constCond(bool value)
{
	return [=](const Entry& x)
	{
		return value;
	};
}

// 两个条件通过逻辑与 (and) 运算复合
cond_t operator && (const cond_t& a, const cond_t& b)
{
	return [=](const Entry& x)
	{
		return a(x) && b(x);
	};
}

// 两个条件通过逻辑或 (or) 运算复合
cond_t operator || (const cond_t& a, const cond_t& b)
{
	return [=](const Entry& x)
	{
		return a(x) || b(x);
	};
}

// 条件取反 (not)
cond_t operator ! (const cond_t& a)
{
	return [=](const Entry& x)
	{
		return !a(x);
	};
}

cond_t& operator |= (cond_t& a, const cond_t& b)
{
	return a = a || b;
}

cond_t& operator &= (cond_t& a, const cond_t& b)
{
	return a = a && b;
}
