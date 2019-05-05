#include <memory>
#include <iostream>
#include <algorithm>

#include "table.h"
#include "tools.h"

/**
 * Returns attributes (data type, width, etc.) of this column in a string.
*/
std::string Table::attr_t::typeName() const
{
	switch (type)
	{
		case type_t::INTEGER:
		{
			//return "int(" + std::to_string(width) + ")";
			return "int(11)";
		}
		case type_t::DOUBLE:
		{
			//return "double(" + std::to_string(width) + ")";
			return "double";
		}
		case type_t::STRING:
		{
			//return "char(" + std::to_string(width) + ")";
			return "char(1)";
		}
	}
}


/**
 * Table constructor
 * attrlist format: (attrName1 Type1, ..., attrNameN TypeN NOT NULL, PRIMARY KEY(attrName1))
 *
 * @param attrlist
*/
Table::Table(const tokens& attrClause)
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
				throw "unrecognized data type";
			attrIndex[*t] = attr.size();
			attr.push_back((attr_t){type, *t, (p - t) == 4});
		}
		t = p + 1;
	}
	if (!primaryKey.empty())
		primaryAttr = attrIndex[primaryKey];
}

/**
 * Makes a boolean entry filter function with the string specified.
 *
 * @param Condition (single operation)
 * @return Boolean filter function based on this condition string.
 * @exception nullptr / "unrecognized condition" / exceptions in fromLiteral
*/
cond_t Table::atomCond(const tokens& cond)
{
	// TODO currently not concatenating consecutive strings
	if (cond.size() != 3)
		throw "unrecognized condition";
	const std::string& operand1 = cond[0];
	const std::string& operation = cond[1];
	const std::string& operand2 = cond[2];

	int index1 = attrIndex.count(operand1)? attrIndex[operand1]: -1;
	int index2 = attrIndex.count(operand2)? attrIndex[operand2]: -1;

	// extract operation
	std::function<bool(const data_t&, const data_t&)> op;
	if (operation == "<")
		op = std::less<data_t>();
	else if (operation == ">")
		op = std::greater<data_t>();
	else if (operation == "=")
		op = [](const data_t& a, const data_t& b){return a==b;};
	else throw "unrecognized condition";

	if (index1 != -1 && index2 != -1) // all variables
	{
		return [=](const Entry& e)
		{
			if (e[index1] == nullptr || e[index2] == nullptr)
				return false;
			return op(*e[index1], *e[index2]);
		};
	}
	if (index1 != -1 && index2 == -1) // var - literal
	{
		std::shared_ptr<data_t> val2 (data_t::fromLiteral(operand2));
		return [=](const Entry& e)
		{
			if (e[index1] == nullptr)
				return false;
			return op(*e[index1], *val2);
		};
	}
	if (index1 == -1 && index2 != -1) // literal - var
	{
		std::shared_ptr<data_t> val1 (data_t::fromLiteral(operand1));
		return [=](const Entry& e)
		{
			if (e[index2] == nullptr)
				return false;
			return op(*val1, *e[index2]);
		};
	}
	// all literals
	// conversion should throw exception if not literal
	std::shared_ptr<data_t> val1 (data_t::fromLiteral(operand1));
	std::shared_ptr<data_t> val2 (data_t::fromLiteral(operand2));
	return constCond(op(*val1, *val2));
}



/**
 * Makes a boolean entry filter function with the string specified.
 *
 * @param Condition
 * @return Boolean filter function based on this condition string.
*/
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


/**
 * Makes a entry modifier (one-value assignment) with the string specified.
*/
set_t Table::atomSet(const tokens& cond)
{
	if (cond.size() != 3 || cond[1] != "=")
		throw "unrecognized set";
	const std::string& attrName = cond[0];
	const std::string& attExpression = cond[2];

	if (!attrIndex.count(attrName))
		throw "no such attr";
	int index = attrIndex[attrName];
	std::shared_ptr<data_t> val (data_t::fromLiteral(attExpression));
	// TODO change fromLiteral to evaluate
	return [=](Entry& e)
	{
		delete e[index];
		e[index] = val->copy();
	};
}



/**
 * Makes a entry modifier function with the string specified.
 *
 * @param Operation
 * @return modifier function based on this operation string.
*/
set_t Table::buildSet(const tokens& cond)
{
	// currently only handling one-value assignment
	return atomSet(cond);
}


/**
 * Makes an entry with the string specified.
 * in example: INSERT INTO oop_info(stu_id, stu_name) VALUES (2018011343, "a");
 * attrName = {"stu_id", ",", "stu_name"}
 * dataValue = {"2018011343", ",", "\"a\""}
 *
 * @param attribute list (unparsed)
 * @param data list (unparsed)
 * @return entry constructed.
*/
Entry Table::buildEntry(const tokens& attrName, const tokens& dataValue)
{
	int n = attrName.size();
	if ((n!=0 && n%2==0) || n!=dataValue.size())
		throw "Table::buildEntry: unrecognized format";
	// attrName & dataValue should contain alternating string & comma
	Entry entry(attr.size(), nullptr);
	for (int i=0; i<n; ++i)
	{
		if (i%2 == 0)
		{
			entry[attrIndex[attrName[i]]] = data_t::fromLiteral(dataValue[i]);
		}
		else
		{
			if (attrName[i] != "," || dataValue[i] != ",")
				throw "Table::buildEntry: unrecognized format";
		}
	}
	return entry;
}


/**
 * Inserts an entry to this table.
 *
 * @param list of attribute names
 * @param list of attribute values
 * @return Number of entries inserted
*/
int Table::insert(const tokens& attrName, const tokens& attrValue)
{
	data.push_back(buildEntry(attrName, attrValue));
	return 1;
}

/**
 * Clears all entries in this table, but preserving table structure.
 *
 * @return Number of entries deleted
*/
int Table::remove()
{
	int entriesRemoved = std::distance(data.begin(), data.end());
	data.clear();
	return entriesRemoved;
}

/**
 * Deletes entries which satisfy condition ${cond} from this table.
 *
 * @param Delete condition
 * @return Number of entries deleted
*/
int Table::remove(const tokens& whereClause)
{
	cond_t cond = buildCond(whereClause);
	auto iter = std::remove_if(data.begin(), data.end(), cond);
	int entriesRemoved = std::distance(iter, data.end());
	data.erase(iter, data.end());
	return entriesRemoved;
}




/**
 * Updates all entries with ${setClause}.
 *
 * @param Update method (as a function)
 * @return Number of entries updated
*/
int Table::update(const tokens& setClause)
{
	set_t set = buildSet(setClause);
	for (Entry& e: data)
		set(e);
	return data.size();
}


/**
 * Updates entries which satisfy condition ${cond} with data ${set}.
 *
 * @param Update method (as a function)
 * @param Update condition
 * @return Number of entries updated
*/
int Table::update(const tokens& setClause, const tokens& whereClause)
{
	set_t set = buildSet(setClause);
	cond_t cond = buildCond(whereClause);
	int entriesAffected = 0;
	for (Entry& e: data)
		if (cond(e)) set(e), ++entriesAffected;
	return entriesAffected;
}



/**
 * print specified column (data field)
 * pass {"*"} as first argument to print all columns
 *
 * @param column specified
 * @return Number of entries printed
*/
int Table::select(const attrs& attrName)
{
	return select(attrName, split("1=1"));
}


/**
 * print specified column (data field) of entries that satisfy a condition
 * pass {"*"} as first argument to print all columns
 *
 * @param column specified
 * @param condition
 * @return Number of entries printed
*/
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

attrs Table::attrList() const
{
	attrs res;
	for (auto& x: attr)
		res.push_back(x.name);
	return res;
}

/**
 * Lists columns of this table.
 *
 * @param Stream for output
*/
void Table::show(std::ostream& o)
{
	o << "Field\tType\tNull\tKey\tDefault\tExtra\n";
	for (unsigned i = 0; i < attr.size(); ++i) {
		const Table::attr_t& a = attr[i];
		o << a.name << '\t' << a.typeName() << '\t'
			<< (a.nonNull ? "NO" : "YES") << '\t'
			<< (i == primaryAttr ? "PRI" : "") << "\tNULL" << "\t\n";
	}
}

/**
 * Physically sort entries with the column name specified.
 *
 * @param Name of the key. If empty the primary key will take place.
*/
void Table::sort(std::string attr)
{
	if (attr == "")
	{
		if (primaryAttr == -1) return;
		else attr = this->attr[primaryAttr].name;
	}
	int index = attrIndex[attr];
	std::sort(data.begin(), data.end(), [=](Entry& a, Entry& b) { return *a[index] < *b[index]; });
}
