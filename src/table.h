#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

#include <iostream>

#include "entry.h"
#include "data_t.h"
#include "cond_t.h"
#include "set_t.h"

typedef std::vector<std::string> tokens;

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

	cond_t atomCond(const tokens&);
	cond_t atomSet(const tokens&);
	cond_t buildCond(const tokens&);
	set_t buildSet(const tokens&);
	Entry buildEntry(const tokens& attrName, const tokens& dataValue);

public:
	Table(std::string attrClause);

	int insert(const tokens& attrName, const tokens& attrValue);
	int remove();
	int remove(const tokens& whereClause);
	int update(const tokens& setClause);
	int update(const tokens& setClause, const tokens& whereClause);
	int select(const tokens& attrName);
	int select(const tokens& attrName, const tokens& whereClause);
	int filter(const tokens& whereClause, Table& result);

	void show(std::ostream& = std::cout);
};
