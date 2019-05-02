#pragma once

#include <functional>

#include "entry.h"

typedef std::function<bool(const Entry&)> cond_t;

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
