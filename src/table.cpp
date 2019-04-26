#include "table.h"

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
 * Inserts an entry to this table.
 * 
 * @param Entry to insert
 * @return Number of entries inserted
*/
int Table::insert(const Entry& entry)
{
	data.push_back(entry);
	return 1;
}

/**
 * Deletes entries which satisfy condition ${cond} from this table.
 * 
 * @param Delete condition
 * @return Number of entries deleted
*/
int Table::remove(cond_t cond)
{
	auto iter = std::remove_if(data.begin(), data.end(), cond);
	int entriesRemoved = std::distance(iter, data.end());
	data.erase(iter, data.end());
	return entriesRemoved;
}

/**
 * Updates entries which satisfy condition ${cond} with data ${set}.
 * 
 * @param Update method (as a function)
 * @param Update condition
 * @return Number of entries updated
*/
int Table::update(set_t set, cond_t cond)
{
	int entriesAffected = 0;
	for (Entry& e: data)
		if (cond(e)) set(e), ++entriesAffected;
	return entriesAffected;
}

/**
 * Selects entries which satisfy condition ${cond}, and puts the result into ${result} as a new table.
 * 
 * @param Select condition
 * @param Table for select result
 * @return Number of entries selected
*/
int Table::select(cond_t cond, Table& result)
{
	result.attr = this->attr;
	result.data.clear();
	result.primaryAttr = this->primaryAttr;

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
		Table::attr_t& a = attr[i];
		o << a.name << '\t' << a.typeName() << '\t' << (a.nonNull ? "YES" : "NO") << '\t' << (i == primaryAttr ? "PRI" : "") << "\tNULL" << "\t\n";
	}
}
