#pragma once

#include <map>

#include "table.h"

class Database
{
private:
public:
	std::map<std::string, Table*> table;
	~Database();
	void drop(std::string tableName);
	void create(std::string tableName, const tokens& traits);
	void show();
	void show(std::string tableName);
	Table* operator[] (std::string tableName);
};
