#include "entry.h"
#include "cond.h"
#include "set.h"

class Table
{
private:
	std::vector<Entry> data;
	std::vector<std::string> attr;
public:
	cond_t buildCond(std::string);
	void insert(Entry);
	void remove(cond_t);
	void update(set_t, cond_t);
	void select(cond_t);
};
