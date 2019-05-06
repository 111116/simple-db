#pragma once

#include <map>
#include <string>

#include "table.h"

class Database
{
private:
	std::string dbName; // 数据库名称，仅在 Database::show() 中使用
public:
	std::map<std::string, Table*> table; // 表名到表对象指针的映射
	Database(std::string dbName); // 根据表名构造空表
	~Database();
	void drop(std::string tableName); // 删除表
	void create(std::string tableName, const tokens& traits); // 创建表
	void show(); // 显示所有表名
	void show(std::string tableName); // 显示某个表的结构
	Table* operator[] (std::string tableName); // 重载下标操作符，相当于直接在 map table 上使用下标操作符，简化写法
};
