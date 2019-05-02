#include "table.h"

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
Table::Table(std::string attrClause)
{
	attrClause += ',';
	std::vector<std::string> str;
	str.push_back("");
	std::string primaryKey;
	for (auto c: attrClause){
		if (c == ' ' || c == '(' || c == ')')
		{
			if (!str.back().empty())
				str.push_back("");
		}
		else if (c == ',')
			{
				if (!str.back().empty())
					str.push_back("");
				if (str.size() == 4)
					primaryKey = str[2];
				else
				{
					str[1] = stringToLower(str[1]);
					type_t tmp;
					if (str[1] == "integer") tmp = type_t::INTEGER;
					if (str[1] == "double") tmp = type_t::DOUBLE;
					if (str[1] == "string") tmp = type_t::STRING;
					attr.push_back((attr_t){tmp, str[0], str.size() == 5});
					attrIndex[str[0]] = attr.size();
				}
				str.clear();
				str.push_back("");
			}
		else str.back() += c;
	}
	if (!primaryKey.empty())
		primaryAttr = attrIndex[primaryKey];
}


/**
 * Makes a boolean entry filter function with the string specified.
 *
 * @param Condition (single operation)
 * @return Boolean filter function based on this condition string.
*/
cond_t Table::atomCond(std::string)
{
	std::string operand1;
	std::string operation;
	std::string operand2;
	// TODO parse
	int index1 = attrIndex.count(operand1)? attrIndex[operand1]: -1;
	int index2 = attrIndex.count(operand2)? attrIndex[operand2]: -1;
	std::function<bool(const data_t&, const data_t&)> op;
	// TODO convert op

	if (index1 == -1 && index2 == -1) // all literals
	{
		// conversion should throw exception if not literal
		std::shared_ptr<data_t> val1 (fromLiteral(operand1));
		std::shared_ptr<data_t> val2 (fromLiteral(operand2));
		bool result = op(*val1, *val2);
		return [=](const Entry&) { return result; };
	}
	if (index1 != -1 && index2 != -1) // all variables
	{
		return [=](const Entry& e) { return op(e[index1], e[index2]); };
	}
	if (index1 != -1 && index2 == -1) // var - literal
	{
		std::shared_ptr<data_t> val2 (fromLiteral(operand2));
		return [=](const Entry& e) { return op(e[index1], *val2); };
	}
	if (index1 == -1 && index2 != -1) // literal - var
	{
		std::shared_ptr<data_t> val1 (fromLiteral(operand1));
		return [=](const Entry& e) { return op(*val1, e[index2]); };
	}
}



/**
 * Makes a boolean entry filter function with the string specified.
 *
 * @param Condition
 * @return Boolean filter function based on this condition string.
*/
cond_t Table::buildCond(std::string)
{

}



cond_t Table::atomSet(std::string)
{
	std::string attrName;
	std::string attExpression;
	// TODO parse
	if (!attrIndex.count(attrName))
		throw "no such attr";
	int index = attrIndex[operand1];
	std::shared_ptr<data_t> val (fromLiteral(attExpression));
	// change fromLiteral to evaluate at stage 2
	return [=](Entry& e)
	{
		e[index] = *val;
	};
}



/**
 * Makes a entry modifier function with the string specified.
 *
 * @param Operation
 * @return modifier function based on this operation string.
*/
set_t Table::buildSet(std::string)
{

}


/**
 * Makes an entry with the string specified.
 *
 * @param attribute list (unparsed)
 * @param data list (unparsed)
 * @return entry constructed.
*/
Entry Table::buildEntry(std::string attrlist, std::string datalist)
{

}


/**
 * Inserts an entry to this table.
 *
 * @param list of attribute names
 * @param list of attribute values
 * @return Number of entries inserted
*/
int Table::insert(std::string attrName, std::string attrValue)
{
	data.push_back(buildEntry(attrName, attrValue));
	return 1;
}


/**
 * Deletes entries which satisfy condition ${cond} from this table.
 *
 * @param Delete condition
 * @return Number of entries deleted
*/
int Table::remove(std::string whereClause);
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
int Table::update(std::string setClause)
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
int Table::update(std::string setClause, std::string whereClause)
{
	set_t set = buildSet(setClause);
	cond_t cond = buildCond(whereClause);
	int entriesAffected = 0;
	for (Entry& e: data)
		if (cond(e)) set(e), ++entriesAffected;
	return entriesAffected;
}



int Table::select(std::string attrName)
{
	if (attrName == "*")
	{
		for (int i = 0; i < attr.size() - 1; ++i)
			std::cout << attr[i].name << "\t";
		std::cout << attr[attr.size() - 1].name << std::endl;
		for (Entry& e: data)
		{
			for (int i = 0; i < e.size() - 1; ++i)
				std::cout << e[i].get() << "\t";
			std::cout << e[e.size() - 1].get() << std::endl;
		}
	}
	else
	{
		if (!attrIndex.count(attrName))
			throw "no such attr";
		std::cout << attrName << std::endl;
		int index = attrIndex[attrName];
		for (Entry& e: data)
			std::cout << e[index] << std::endl;
	}
	return data.size();
}


int Table::select(std::string attrName, std::string whereClause)
{
	int entriesAffected = 0;
	cond_t cond = buildCond(whereClause);
	if (attrName == "*")
	{
		for (int i = 0; i < attr.size() - 1; ++i)
			std::cout << attr[i].name << "\t";
		std::cout << attr[attr.size() - 1].name << std::endl;
		for (Entry& e: data)
			if (cond(e))
			{
				for (int i = 0; i < e.size() - 1; ++i)
					std::cout << e[i].get() << "\t";
				std::cout << e[e.size() - 1].get() << std::endl;
				++entriesAffected;
			}
	}
	else
	{
		if (!attrIndex.count(attrName))
			throw "no such attr";
		std::cout << attrName << std::endl;
		int index = attrIndex[attrName];
		for (Entry& e: data)
			if (cond(e))
			{
				std::cout << e[index] << std::endl;
				++entriesAffected;
			}
	}
	return entriesAffected;
}


/**
 * Selects entries which satisfy condition ${cond}, and puts the result into ${result} as a new table.
 *
 * @param Select condition
 * @param Table for select result
 * @return Number of entries selected
*/
int Table::filter(std::string whereClause, Table& result)
{
	result.attr = this->attr;
	result.attrIndex = this->attrIndex;
	result.primaryAttr = this->primaryAttr;
	result.data.clear();
	cond_t cond = buildCond(whereClause);
	for (Entry& e: data)
		if (cond(e)) result.insert(e);
	return result.data.size();
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
		o << a.name << '\t' << a.typeName() << '\t' << (a.nonNull ? "YES" : "NO") << '\t' << (i == primaryAttr ? "PRI" : "") << "\tNULL" << "\t\n";
	}
}
