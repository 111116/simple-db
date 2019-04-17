#pragma once

#include "entry.h"
#include "cond.h"
#include "set.h"


class Table
{
private:
	struct attr_t
	{
		enum type_t
		{
			INTEGER, DOUBLE, STRING
		};
		type_t type;
		std::string name;
		bool nonNull = false;
	};
	std::vector<attr_t> attr;
	std::vector<Entry> data;
	int primaryAttr = -1;
	std::function<data_t*(const Entry&)> getDatabyAttr(std::string);
	cond_t atomCond(std::string);

public:
	cond_t buildCond(std::string);
	void insert(Entry);
	void remove(cond_t);
	void update(set_t, cond_t);
	void select(cond_t);
	void show();
};
