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
 * Drops a table named ${tableName} from this database.
 * 
 * @param Name of the table to be dropped
*/
void Database::drop(std::string tableName)
{
	table.erase(tableName);
}

/**
 * Creates a table in this database.
 * 
 * @param Name of the table to be created
 * @param Columns with data types and constraints
*/
void Database::create(std::string tableName, std::string traits)
{
	table[tableName] = new Table(traits);
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
 * Find a table in this database
 * 
 * @param Name of the table
 * @return Pointer to specified table (NULL if not found)
*/
Table* Database::operator[](std::string tableName)
{
	if (table.count(tableName))
		return table[tableName];
	return NULL;
}
