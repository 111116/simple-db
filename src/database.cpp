#include "database.h"

Database::~Database()
{
	for (auto p: table)
		delete(p->second);
}

void Database::drop(std::string tableName)
{
	table.erase(tableName);
}

void Database::create(std::string tableName, std::string traits)
{
	table[tableName] = new(Table)(traits);
}

void Database::show(std::string tableName)
{
	table[tableName]->show();
}
