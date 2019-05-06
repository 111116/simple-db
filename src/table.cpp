#include <memory>
#include <iostream>
#include <algorithm>

#include "table.h"
#include "tools.h"

std::string Table::attr_t::typeName() const // 返回列数据类型的名称
{
	switch (type)
	{
		case type_t::INTEGER:
		{
			return "int(11)";
		}
		case type_t::DOUBLE:
		{
			return "double";
		}
		case type_t::STRING:
		{
			return "char(1)";
		}
	}
}

Table::Table(const tokens& attrClause) // 构造新表，参数通过 split() 函数对 SQL 语句中表的属性分词得到
{
	std::string primaryKey;
	for (auto t = attrClause.begin(); t < attrClause.end(); )
	{
		auto p = find(t, attrClause.end(), ",");
		if (p - t == 5)
			primaryKey = *(t + 3);
		else
		{
			type_t type;
			auto typeLower = stringToLower(*(t + 1));
			if (typeLower == "int") type = type_t::INTEGER; else
			if (typeLower == "double") type = type_t::DOUBLE; else
			if (typeLower == "char") type = type_t::STRING; else
				throw "Unrecognized data type";
			attrIndex[*t] = attr.size();
			attr.push_back((attr_t){type, *t, (p - t) == 4});
		}
		t = p + 1;
	}
	if (!primaryKey.empty())
		primaryAttr = attrIndex[primaryKey];
}

// 根据 WHERE 子句构造原子判断条件函数
// 注意此处只能包含单个比较运算
cond_t Table::atomCond(const tokens& cond)
{
	// TODO 当前尚不支持自动连接连续书写的多个字符串："abc" "def" => "abcdef"
	if (cond.size() != 3)
		throw "Unrecognized condition";
	const std::string& operand1 = cond[0];
	const std::string& operation = cond[1];
	const std::string& operand2 = cond[2];

	int index1 = attrIndex.count(operand1)? attrIndex[operand1]: -1;
	int index2 = attrIndex.count(operand2)? attrIndex[operand2]: -1;

	// 提取比较运算
	std::function<bool(const data_t&, const data_t&)> op;
	if (operation == "<")
		op = std::less<data_t>();
	else if (operation == ">")
		op = std::greater<data_t>();
	else if (operation == "=")
		op = [](const data_t& a, const data_t& b){return a==b;};
	else throw "Unrecognized condition";

	if (index1 != -1 && index2 != -1) // 运算符两边都是变量
	{
		return [=](const Entry& e)
		{
			if (e[index1] == nullptr || e[index2] == nullptr)
				return false;
			return op(*e[index1], *e[index2]);
		};
	}
	if (index1 != -1 && index2 == -1) // 左边为变量，右边为字面值
	{
		std::shared_ptr<data_t> val2 (data_t::fromLiteral(operand2));
		return [=](const Entry& e)
		{
			if (e[index1] == nullptr)
				return false;
			return op(*e[index1], *val2);
		};
	}
	if (index1 == -1 && index2 != -1) // 左边为字面值，右边为变量
	{
		std::shared_ptr<data_t> val1 (data_t::fromLiteral(operand1));
		return [=](const Entry& e)
		{
			if (e[index2] == nullptr)
				return false;
			return op(*val1, *e[index2]);
		};
	}
	// 两边都是字面值
	std::shared_ptr<data_t> val1 (data_t::fromLiteral(operand1));
	std::shared_ptr<data_t> val2 (data_t::fromLiteral(operand2));
	return constCond(op(*val1, *val2));
}

// 根据 token 列表构造完整的 WHERE 子句判断函数对象
cond_t Table::buildCond(const tokens& cond)
{
	cond_t stack0 = constCond(false);
	cond_t stack1 = constCond(true);
	auto last = cond.begin();
	for (auto p = cond.begin(); p != cond.end(); ++p)
	{
		if (stringToLower(*p) == "and")
		{
			stack1 &= atomCond(tokens(last,p));
			last = p+1;
		}
		if (stringToLower(*p) == "or")
		{
			stack0 |= stack1 && atomCond(tokens(last,p));
			stack1 = constCond(true);
			last = p+1;
		}
	}
	return stack0 || stack1 && atomCond(tokens(last, cond.end()));
}

// 构造原子修改操作函数对象
// 参数 cond 中只能包含一条修改操作
set_t Table::atomSet(const tokens& cond)
{
	if (cond.size() != 3 || cond[1] != "=")
		throw "Unrecognized set";
	const std::string& attrName = cond[0];
	const std::string& attExpression = cond[2];

	if (!attrIndex.count(attrName))
		throw "No such attribute";
	int index = attrIndex[attrName];
	std::shared_ptr<data_t> val (data_t::fromLiteral(attExpression));
	// TODO 以后可能需要将单个的 fromLiteral 读取字面值修改为赋值表达式的计算
	return [=](Entry& e)
	{
		delete e[index];
		e[index] = val->copy();
	};
}

// 构造复合修改操作函数对象
// TODO 当前只支持单个修改操作（不支持在一个 SET 子句中修改多个列的值），因此目前该函数行为与 atomSet 等同
set_t Table::buildSet(const tokens& cond)
{
	return atomSet(cond);
}

// 根据列名称和相应的字面量构造一行数据
// attrNames 中未涉及的列将被设置为 NULL (nullptr)
//
// 示例：INSERT INTO oop_info(stu_id, stu_name) VALUES (2018011343, "a");
// attrName = {"stu_id", ",", "stu_name"}
// dataValue = {"2018011343", ",", "\"a\""}
Entry Table::buildEntry(const tokens& attrNames, const tokens& attrValues)
{
	int n = attrNames.size();
	if ((n!=0 && n%2==0) || n!=attrValues.size())
		throw "Table::buildEntry: Unrecognized format";
	// attrNames & attrValues should contain alternating string & comma
	Entry entry(attr.size(), nullptr);
	for (int i=0; i<n; ++i)
	{
		if (i%2 == 0)
		{
			entry[attrIndex[attrNames[i]]] = data_t::fromLiteral(attrValues[i]);
		}
		else
		{
			if (attrNames[i] != "," || attrValues[i] != ",")
				throw "Table::buildEntry: Unrecognized format";
		}
	}
	return entry;
}

// 向表中插入一行，参数格式与 Table::buildEntry 相同
// 返回插入的行数（总是 1）
int Table::insert(const tokens& attrNames, const tokens& attrValues)
{
	data.push_back(buildEntry(attrNames, attrValues));
	return 1;
}

// 清空表中数据，但保留表的结构
// 返回删除的行数
int Table::remove()
{
	int entriesRemoved = std::distance(data.begin(), data.end());
	data.clear();
	return entriesRemoved;
}

// 删除表中满足 whereClause 指定的条件的数据
// 返回删除的行数
int Table::remove(const tokens& whereClause)
{
	cond_t cond = buildCond(whereClause);
	auto iter = std::remove_if(data.begin(), data.end(), cond);
	int entriesRemoved = std::distance(iter, data.end());
	data.erase(iter, data.end());
	return entriesRemoved;
}

// 通过 setClause 修改表中每一行的数据
// 返回被修改的行数
int Table::update(const tokens& setClause)
{
	set_t set = buildSet(setClause);
	for (Entry& e: data)
		set(e);
	return data.size();
}

// 通过 setClause 修改表中所有满足 whereClause 的行的数据
// 返回被修改的行数
int Table::update(const tokens& setClause, const tokens& whereClause)
{
	set_t set = buildSet(setClause);
	cond_t cond = buildCond(whereClause);
	int entriesAffected = 0;
	for (Entry& e: data)
		if (cond(e)) set(e), ++entriesAffected;
	return entriesAffected;
}

// 对于表中的每一行输出 attrName 指定的列的数据
// 返回被输出的行数
int Table::select(const attrs& attrName)
{
	return select(attrName, split("1=1"));
}

// 对于表中满足 whereClause 的每一行输出 attrName 指定的列的数据
// 返回被输出的行数
int Table::select(const attrs& attrName, const tokens& whereClause)
{
	std::vector<int> index;
	for (auto& x: attrName)
		if (!attrIndex.count(x))
			throw "no such attr";
		else
			index.push_back(attrIndex[x]);
	int entriesAffected = 0;
	cond_t cond = buildCond(whereClause);
	auto print = [](data_t* p, char end='\n')
	{
		if (p != nullptr)
			std::cout << *p << end;
		else
			std::cout << "NULL" << end;
	};
	this->sort();
	for (Entry& e: data)
		if (cond(e))
		{
			if (entriesAffected == 0)
			{
				for (int i = 0; i < attrName.size(); ++i)
					std::cout << attrName[i] << (i + 1 == attrName.size() ? '\n' : '\t');
			}
			for (int i = 0; i < attrName.size() - 1; ++i)
				print(e[index[i]], '\t');
			print(e[index.back()]);
			++entriesAffected;
		}
	return entriesAffected;
}

// 汇总表中所有列的名称并返回
attrs Table::attrList() const
{
	attrs res;
	for (auto& x: attr)
		res.push_back(x.name);
	return res;
}

// 打印表的结构
void Table::show()
{
	std::cout << "Field\tType\tNull\tKey\tDefault\tExtra\n";
	for (unsigned i = 0; i < attr.size(); ++i) {
		const Table::attr_t& a = attr[i];
		std::cout << a.name << '\t' << a.typeName() << '\t'
			<< (a.nonNull ? "NO" : "YES") << '\t'
			<< (i == primaryAttr ? "PRI" : "") << "\tNULL" << "\t\n";
	}
}

// 对表中所有数据排序，会直接修改数据存储的顺序
// 参数 attrName 指定要根据哪一列进行排序，如果留空则尝试按主键排序，如果同时表没有主键则什么都不做
void Table::sort(std::string attrName)
{
	if (attrName == "")
	{
		if (primaryAttr == -1) return;
		else attrName = this->attr[primaryAttr].name;
	}
	int index = attrIndex[attrName];
	std::sort(data.begin(), data.end(), [=](Entry& a, Entry& b) { return *a[index] < *b[index]; });
}
