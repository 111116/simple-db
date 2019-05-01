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
					type tmp;
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
	if (!PrimaryKey.empty())
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
	
	return [=](Entry& entry)
	{
		(index1==-1? )
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
	if (!attrIndex.count(attrName))
		throw "no such attr";
	int index = attrIndex[attrName];
	for (Entry& e: data)
		e[index].print();
	return data.size();
}


int Table::select(std::string attrName, std::string whereClause)
{
	if (!attrIndex.count(attrName))
		throw "no such attr";
	int index = attrIndex[attrName];
	cond_t cond = buildCond(whereClause);
	int entriesAffected = 0;
	for (Entry& e: data)
		if (cond(e))
		{
			e[index].print();
			++entriesAffected;
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
