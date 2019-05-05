#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "entry.h"
#include "data_t.h"
#include "cond_t.h"
#include "set_t.h"
#include "tools.h"

class Table
{
private:
	enum type_t
	{
		INTEGER, DOUBLE, STRING
	};
	struct attr_t // 列属性
	{
		//int width;
		type_t type; // 列数据类型
		std::string name; // 列名称
		bool nonNull; // 是否要求非空
		std::string typeName() const; // 列数据类型的名称，用于Table::show()
	};
	std::vector<attr_t> attr; // 各列属性
	std::unordered_map<std::string, unsigned> attrIndex; // 用于由列名称查询是第几列
	std::vector<Entry> data; // 各行数据
	int primaryAttr = -1; // 主键是第几列，无主键则为-1

	cond_t atomCond(const tokens&); // 构造单运算符的判断条件，见cond_t
	set_t atomSet(const tokens&); // 构造单赋值符的修改器，目前仅支持赋为字面值，见set_t
	cond_t buildCond(const tokens&); // 构造判断条件
	set_t buildSet(const tokens&); // 构造修改器（目前仅支持单赋值符，行为同atomSet）
	Entry buildEntry(const tokens& attrName, const tokens& dataValue); // 构造一行数据，参数格式见实现处注释

public:
	Table(const tokens& attrClause);

	attrs attrList() const;
	int insert(const tokens& attrName, const tokens& attrValue);
	int remove(); // 清空数据，保留各列属性
	int remove(const tokens& whereClause);
	int update(const tokens& setClause); // 目前仅支持将单个列赋为字面值
	int update(const tokens& setClause, const tokens& whereClause);
	int select(const attrs& attrName);
	int select(const attrs& attrName, const tokens& whereClause);

	void show(std::ostream& = std::cout);
	void sort(std::string attrName = "");
};
