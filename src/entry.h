#pragma once

#include <vector>

#include "data_t.h"
#include "set_t.h"

class Entry
{
private:
	std::vector<data_t&> data;
public:
	void set(set_t);
};
