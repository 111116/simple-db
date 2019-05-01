#include <map>
#include <string>

#include "client.h"

void create(std::string dbName)
{
	dbList[dbName] = new Database;
}

void drop(std::string dbName)
{
	if (dbList.count(dbName){
		delete dbList[dbName].second;
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

std::string read()
{
	std::string str;
	std::cin >> str;
	if (str.back() == ';')
		str.pop_back();
	return str;
}

int main()
{
	while (true)
	{
		auto str1 = read(), str2 = read();
		auto st1 = stringToLower(str1), st2 = stringToLower(str2);
		
		if (st1 == "create" && st2 == "database")
			create(read());
		if (st1 == "drop" && st2 == "database")
			drop(read());
		if (st1 == "use" && str2 != "table")
			use(str2);
		if (st1 == "show" && st2 == "databases")
			show();

		if (st1 == "create" && st2 == "table")
		{
			auto str = read();
			auto t = str.find('(');
			auto tableName = str.substr(0, t);
			auto traits = str.substr(t + 1, str.size() - t - 2);
			selected->create(tableName, traits);
		}
		if (st1 == "drop" && st2 == "table")
			selected->drop(read());
		if (st1 == "show" && st2 == "tables")
			selected->show();
		if (st1 == "show" && st2 == "columns")
		{
			read();
			selected->show(read());
		}

		if (st1 == "insert" && st2 == "into")
		{
			auto str = read();
			auto t = str.find('(');
			auto tableName = str.substr(0, t);
			auto attrlist = str.substr(t + 1, (str.size() - 2) - (t + 1) + 1);
			read();
			auto datalist = str.substr(1, str.size() - 2);
			selected->table[tableName]->insert(attrlist, datalist);
		}
		if (st1 == "delete" && st2 == "from")
		{
			auto tableName = read();
			std::string whereClause;
			std::getline(std::cin, whereClause);
			auto t = stringToLower(whereClause).find("where");
			if (t == whereClause.npos)
				selected->table[tableName]->remove();
			else {
				whereClause = whereClause.substr(t + 6, (whereClause.size() - 1) - (t + 6) + 1);
				selected->table[tableName]->remove(whereClause);
			}
		}
		if (st1 == "update")
		{
			auto& tableName = str2;
			read(); getchar();
			std::string setClause;
			std::getline(std:cin, setClause);
			auto t = stringToLower(setClause).find("where");
			if (t == setClause.npos)
				selected->table[tableName]->update(setClause);
			else {
				auto whereClause = setClause.substr(t + 6, (setClause.size() - 1) - (t + 6) + 1);
				setClause = setClause.substr(0, t - 1);
				selected->table[tableName]->update(setClause, WhereClause);
			}
			
		}
		if (st1 == "select")
		{
			auto& attrName = str2;
			read();
			auto tableName = read();
			std::string whereClause;
			std::getline(std::cin, whereClause);
			auto t = stringToLower(whereClause).find("where");
			if (t == whereClause.npos)
				selected->table[tableName]->select(attrName);
			else {
				whereClause = whereClause.substr(t + 6, (whereClause.size() - 1) - (t + 6) + 1);
				selected->table[tableName]->select(whereClause);
			}
		}
	}
}
