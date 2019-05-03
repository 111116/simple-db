#include "cond_t.h"

cond_t constCond(bool value)
{
	return [=](const Entry& x)
	{
		return value;
	};
}

cond_t operator && (const cond_t& a, const cond_t& b)
{
	return [=](const Entry& x)
	{
		return a(x) && b(x);
	};
}

cond_t operator || (const cond_t& a, const cond_t& b)
{
	return [=](const Entry& x)
	{
		return a(x) || b(x);
	};
}

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