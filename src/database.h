#pragma once

#include <map>

#include "table.h"

class Database
{
private:
	std::map<std::string, Table*> table;
public:
	~Database();
	void drop(std::string tableName);
	void create(std::string tableName, std::string traits);
	void show(std::string tableName);
	Table* operator[] (std::string tableName);
}