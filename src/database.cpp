#include <iostream>

#include "database.h"

// 指定数据库名称，创建数据库
Database::Database(std::string dbName): dbName(dbName) {} // 该名称仅在 Database::show() 中使用

// 析构当前数据库，同时删除存储的所有表
Database::~Database()
{
	for (auto p: table)
		delete(p.second);
}

// 指定表的名称和结构，创建新表
void Database::create(std::string tableName, const tokens& traits)
{
	table[tableName] = new Table(traits); // 同时存储到 map table 中建立索引
}

// 删除名为 tableName 的表
void Database::drop(std::string tableName)
{
	table.erase(tableName);
}

// 显示当前数据库中所有表的名称
void Database::show()
{
	if (table.size() > 0)
	{
		std::cout << "Tables_in_" << dbName << std::endl; // 格式要求
		for (auto p: table)
			std::cout << p.first << std::endl;
	}
}

// 显示表 tableName 的结构
// TODO 没有处理指定表不存在的情况
void Database::show(std::string tableName)
{
	table[tableName]->show();
}

// 查找名为 tableName 的表，若找不到返回空指针（nullptr）
Table* Database::operator[](std::string tableName)
{
	if (table.count(tableName))
		return table[tableName];
	return nullptr;
}
