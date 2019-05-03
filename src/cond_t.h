#pragma once

#include <functional>

#include "entry.h"

typedef std::function<bool(const Entry&)> cond_t;

cond_t constCond(bool value);

cond_t operator && (const cond_t& a, const cond_t& b);
cond_t operator || (const cond_t& a, const cond_t& b);
cond_t operator ! (const cond_t& a);

cond_t& operator |= (cond_t& a, const cond_t& b);
cond_t& operator &= (cond_t& a, const cond_t& b);
