#pragma once

#include <string>
#include <algorithm>

class badTypeComparison {};

class data_t
{
protected:
	std::string value;
public:
	data_t(std::string);
	static data_t* fromLiteral(std::string);
	virtual bool operator < (const data_t&) const = 0;
	virtual bool operator > (const data_t&) const = 0;
	virtual bool operator == (const data_t&) const = 0;
	std::string& get() const;
};

class dataInt: public data_t
{
	friend class dataDouble;
public:
	using data_t::data_t;
	bool operator < (const data_t&) const;
	bool operator > (const data_t&) const;
	bool operator == (const data_t&) const;
};

class dataDouble: public data_t
{
	friend class dataInt;
public:
	using data_t::data_t;
	bool operator < (const data_t&) const;
	bool operator > (const data_t&) const;
	bool operator == (const data_t&) const;
};

class dataString: public data_t
{
public:
	using data_t::data_t;
	bool operator < (const data_t&) const;
	bool operator > (const data_t&) const;
	bool operator == (const data_t&) const;
};

std::string stringToLower(std::string);
