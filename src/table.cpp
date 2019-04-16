#include "table.h"

cond_t Table::buildCond(std::string)
{

}

void Table::insert(const Entry& entry)
{
	data.push_back(entry);
}

void Table::remove(cond_t cond)
{
	data.erase(std::remove_if(data.begin(), data.end(), cond), data.end());
}

void Table::update(set_t set, cond_t cond)
{
	for (Entry& e: list)
		if (cond(e)) set(e);
}

void Table::select(cond_t cond)
{
	for (Entry& e: list)
		if (cond(e)) e.print();
}
