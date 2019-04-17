#pragma once

#include "entry.h"

typedef std::function<bool(Entry)> cond_t;


cond_t operator && (const cond_t& a, const cond_t& b)
{
	return [&](Entry x)
	{
		return a(x) && b(x);
	};
}

cond_t operator || (const cond_t& a, const cond_t& b)
{
	return [&](Entry x)
	{
		return a(x) || b(x);
	};
}

cond_t operator ! (const cond_t& a)
{
	return [&](Entry x)
	{
		return !a(x);
	};
}