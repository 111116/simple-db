#pragma once

#include <string>
#include <vector>
#include <functional>

#include <iostream>

#include "entry.h"
#include "data_t.h"
#include "cond_t.h"
#include "set_t.h"

class Table
{
private:
	struct attr_t
	{
		enum type_t
		{
			INTEGER, DOUBLE, STRING
		};
		int length;
		type_t type;
		std::string name;
		bool nonNull = false;
		std::string typeName();
	};
	std::vector<attr_t> attr;
	std::vector<Entry> data;
	int primaryAttr = -1;
	std::function<data_t*(const Entry&)> getDatabyAttr(std::string);
	cond_t atomCond(std::string);
	cond_t atomSet(std::string);

public:
	Table(std::string);

	cond_t buildCond(std::string);
	set_t buildSet(std::string);
	Entry buildEntry(std::string attrlist, std::string datalist);
	
	int insert(const Entry&);
	int remove(cond_t);
	int update(set_t, cond_t);
	int select(cond_t, Table&);
	void show(std::ostream& = std::cout);
};
