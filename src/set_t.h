#pragma once

#include <functional>

#include "entry.h"

// 函数对象，定义了修改一条 Entry 的方法，应用于 SET 子句中。也可通过 Lambda 表达式赋值、通过 () 运算符执行。
typedef std::function<void(Entry&)> set_t;
