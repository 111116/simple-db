#include <iostream>
#include <algorithm>

#include "client.h"
#include "tools.h"

std::map<std::string, Database*> dbList;
Database* selected = nullptr;

void create(std::string dbName)
{
	dbList[dbName] = new Database(dbName);
}

void drop(std::string dbName)
{
	if (dbList.count(dbName))
	{
		delete dbList[dbName];
		dbList.erase(dbName);
	}
}

void use(std::string dbName)
{
	selected = dbList[dbName];
}

void show()
{
	std::cout << "Database\n";
	for (auto t: dbList)
		std::cout << t.first << std::endl;
}
