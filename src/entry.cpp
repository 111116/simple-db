#include "entry.h"

// 析构 Entry，同时删除其中所有数据
Entry::~Entry()
{
	for (auto p: *this)
		if (p != nullptr)
			delete p;
}

// 移动构造
Entry::Entry(Entry&& e): std::vector<data_t*>(e)
{
	e.clear(); // 清空原 Entry 防止重复删除数据
}

// 移动赋值
Entry& Entry::operator= (Entry&& e)
{
	for (auto p: *this)
		if (p != nullptr)
			delete p; // 原有的数据要全部删除
	std::vector<data_t*>::operator= (static_cast<std::vector<data_t*>&&>(e));
	e.clear();
	return *this;
}

// 复制一个保存了同样数据的 Entry
Entry Entry::copy()
{
	Entry e;
	for (auto p: *this)
		e.push_back(p != nullptr ? p->copy() : nullptr);
	return e;
}
