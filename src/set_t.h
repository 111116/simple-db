#pragma once

#include <string>
#include <vector>

#include "data_t.h"

typedef std::pair<int, data_t&> update_t; // Update column <first> to value <second>

typedef std::vector<update_t> set_t; // Support multiple SET clauses: UPDATE table SET attr1=value1, attr2=value2
