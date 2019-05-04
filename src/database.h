#pragma once

#include <map>
#include <string>

#include "table.h"

class Database
{
private:
	std::string dbName;
public:
	std::map<std::string, Table*> table;
	Database(std::string);
	~Database();
	void drop(std::string tableName);
	void create(std::string tableName, const tokens& traits);
	void show();
	void show(std::string tableName);
	Table* operator[] (std::string tableName);
};
