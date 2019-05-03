#include <iostream>

#include "database.h"

/**
 * Destructs this database.
*/
Database::~Database()
{
	for (auto p: table)
		delete(p.second);
}

/**
 * Creates a table in this database.
 * 
 * @param Name of the table to be created
 * @param Columns with data types and constraints
*/
void Database::create(std::string tableName, const tokens& traits)
{
	table[tableName] = new Table(traits);
}

/**
 * Drops a table named ${tableName} from this database.
 * 
 * @param Name of the table to be dropped
*/
void Database::drop(std::string tableName)
{
	table.erase(tableName);
}

void Database::show()
{
	std::cout << "Table" << std::endl;
	for (auto p: table)
		std::cout << p.first << std::endl;
}

/**
 * Lists columns of a table specified in this database.
 * 
 * @param Name of the table
*/
void Database::show(std::string tableName)
{
	table[tableName]->show();
}

/**
 * Finds a table in this database
 * 
 * @param Name of the table
 * @return Pointer to specified table (NULL if not found)
*/
Table* Database::operator[](std::string tableName)
{
	if (table.count(tableName))
		return table[tableName];
	return nullptr;
}
