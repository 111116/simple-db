#include "entry.h"


Entry::~Entry()
{
	for (auto p: *this)
		if (p != nullptr)
			delete p;
}

Entry::Entry(Entry&& e): std::vector<data_t*>(e)
{
	e.clear();
}

Entry& Entry::operator= (Entry&& e)
{
	for (auto p: *this)
		if (p != nullptr)
			delete p;
	std::vector<data_t*>::operator= (static_cast<std::vector<data_t*>&&>(e));
	e.clear();
	return *this;
}

Entry Entry::copy()
{
	Entry e;
	for (auto p: *this)
		e.push_back(p!=nullptr? p->copy(): nullptr);
	return e;
}