#include "client.h"
#include "tools.h"

void create(std::string dbName)
{
	dbList[dbName] = new Database;
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

int main()
{
	std::string input;
	while (getline(std::cin, input))
	{
		auto str = split(input), strLower = str;
		std::transform(strLower.begin(), strLower.end(), strLower.begin(), ::stringToLower);
		
		if (str.size() < 2) continue;
		
		if (strLower[0] == "create" && strLower[1] == "database")
			create(str[2]);
		if (strLower[0] == "drop" && strLower[1] == "database")
			drop(str[2]);
		if (strLower[0] == "use" && strLower[1] != "tables")
			use(str[1]);
		if (strLower[0] == "show" && strLower[1] == "databases")
			show();
		
		if (strLower[0] == "create" && strLower[1] == "table")
		{
			auto& tableName = str[2];
			auto traits = tokens(str.begin() + 4, str.end() - 2);
			selected->create(tableName, traits);
		}
		if (strLower[0] == "drop" && strLower[1] == "table")
		{
			auto& tableName = str[2];
			selected->drop(tableName);
		}
		if (strLower[0] == "show" && strLower[1] == "tables")
			selected->show();
		if (strLower[0] == "show" && strLower[1] == "columns")
		{
			auto& tableName = str[3];
			selected->show(tableName);
		}
		
		if (strLower[0] == "insert" && strLower[1] == "into")
		{
			auto& tableName = str[2];
			auto t = find(strLower.begin(), strLower.end(), "values") - strLower.begin();
			auto attrName = tokens(str.begin() + 4, str.begin() + t - 1);
			auto attrValue = tokens(str.begin() + t + 2, str.end() - 2);
			selected->table[tableName]->insert(attrName, attrValue);
		}
		if (strLower[0] == "delete" && strLower[1] == "from")
		{
			auto& tableName = str[2];
			auto t = find(strLower.begin(), strLower.end(), "where") - strLower.begin();
			if (t == (int)strLower.size())
				selected->table[tableName]->remove();
			else
			{
				auto whereClause = tokens(str.begin() + t + 1, str.end() - 1);
				selected->table[tableName]->remove(whereClause); 
			}
		}
		if (strLower[0] == "update")
		{
			auto& tableName = str[1];
			auto t = find(strLower.begin(), strLower.end(), "where") - strLower.begin();
			auto setClause = tokens(str.begin() + 3, str.begin() + t); 
			if (t == (int)strLower.size())
				selected->table[tableName]->update(setClause);
			else
			{
				auto whereClause = tokens(str.begin() + t + 1, str.end() - 1);
				selected->table[tableName]->update(setClause, whereClause);
			}
		}
		if (strLower[0] == "select")
		{
			auto p = find(strLower.begin(), strLower.end(), "from") - strLower.begin();
			auto& tableName = str[p + 1];
			auto attrName = tokens(str.begin() + 1, str.begin() + p);
			if (str[1] == "*")
				attrName = selected->table[tableName]->attrList();
			auto t = find(strLower.begin(), strLower.end(), "where") - strLower.begin();
			if (t == (int)strLower.size())
				selected->table[tableName]->select(attrName);
			else
			{
				auto whereClause = tokens(str.begin() + t + 1, str.end() - 1);
				selected->table[tableName]->select(attrName, whereClause);
			}
		}
	}
}
