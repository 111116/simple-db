#pragma once

#include <functional>

#include "entry.h"

typedef std::function<void(Entry&)> set_t;
