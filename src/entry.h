#pragma once

#include <vector>

#include "data_t.h"

class Entry: public std::vector<data_t*>
{
public:
	using std::vector<data_t*>::vector;
	Entry(const Entry&) = delete;
	~Entry()
	{
		for (auto p: *this)
			if (p != nullptr)
				delete p;
	}
};
