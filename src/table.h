#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include <iostream>

#include "entry.h"
#include "data_t.h"
#include "cond_t.h"
#include "set_t.h"

class Table
{
private:
	enum type_t
	{
		INTEGER, DOUBLE, STRING
	};
	struct attr_t
	{
		//int width;
		type_t type;
		std::string name;
		bool nonNull;
		std::string typeName() const;
	};
	std::vector<attr_t> attr;
	std::unordered_map<std::string, unsigned> attrIndex;
	std::vector<Entry> data;
	int primaryAttr = -1;

	cond_t atomCond(std::string);
	cond_t atomSet(std::string);
	cond_t buildCond(std::string);
	set_t buildSet(std::string);
	Entry buildEntry(std::string attrName, std::string dataValue);

public:
	Table(std::string attrClause);

	int insert(std::string attrName, std::string attrValue);
	int remove();
	int remove(std::string whereClause);
	int update(std::string setClause);
	int update(std::string setClause, std::string whereClause);
	int select(std::string attrName);
	int select(std::string attrName, std::string whereClause);
	int filter(std::string whereClause, Table& result);

	void show(std::ostream& = std::cout);
};
