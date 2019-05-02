#pragma once

#include <string>
#include <algorithm>

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
};

class dataInt: public data_t
{
public:
	using data_t::data_t;
	bool operator < (const dataInt&) const;
	bool operator > (const dataInt&) const;
	bool operator == (const dataInt&) const;
};

class dataDouble: public data_t
{
public:
	using data_t::data_t;
	bool operator < (const dataDouble&) const;
	bool operator > (const dataDouble&) const;
	bool operator == (const dataDouble&) const;
};

class dataString: public data_t
{
public:
	using data_t::data_t;
	bool operator < (const dataString&) const;
	bool operator > (const dataString&) const;
	bool operator == (const dataString&) const;
};

std::string stringToLower(std::string);
