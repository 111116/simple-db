#pragma once

#include <string>
#include <algorithm>

class data_t
{
//protected:
//	std::string value;
public:
	static data_t* fromLiteral(std::string);
	virtual bool operator < (const data_t&) const = 0;
	virtual bool operator > (const data_t&) const = 0;
	virtual bool operator == (const data_t&) const = 0;
	virtual std::string get() const;
};

class dataInt: public data_t
{
	friend class dataDouble;
private:
	int value;
public:
	//using data_t::data_t;
	dataInt(int);
	dataInt(std::string);
	bool operator < (const data_t&) const;
	bool operator > (const data_t&) const;
	bool operator == (const data_t&) const;
	virtual std::string get() const;
};

class dataDouble: public data_t
{
	friend class dataInt;
private:
	double value;
public:
//	using data_t::data_t;
	dataDouble(double);
	dataDouble(std::string);
	bool operator < (const data_t&) const;
	bool operator > (const data_t&) const;
	bool operator == (const data_t&) const;
	virtual std::string get() const;
};

class dataString: public data_t
{
private:
	std::string value;
public:
//	using data_t::data_t;
	dataString(std::string);
	bool operator < (const data_t&) const;
	bool operator > (const data_t&) const;
	bool operator == (const data_t&) const;
	virtual std::string get() const;
};

std::string stringToLower(std::string);
