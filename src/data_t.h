#pragma once

#include <string>
#include <iostream>

class data_t
{
public:
	static data_t* fromLiteral(std::string);
	virtual data_t* copy() = 0;
	virtual bool operator < (const data_t&) const = 0;
	virtual bool operator > (const data_t&) const;
	virtual bool operator == (const data_t&) const;
	virtual std::string get() const = 0;
	virtual ~data_t();
};

class dataInt: public data_t
{
	friend class dataDouble;
private:
	int value;
public:
	dataInt(int);
	dataInt(std::string);
	dataInt* copy();
	bool operator < (const data_t&) const;
	using data_t::operator >;
	using data_t::operator ==;
	virtual std::string get() const;
};

class dataDouble: public data_t
{
	friend class dataInt;
private:
	double value;
public:
	dataDouble(double);
	dataDouble(std::string);
	dataDouble* copy();
	bool operator < (const data_t&) const;
	using data_t::operator >;
	using data_t::operator ==;
	virtual std::string get() const;
};

class dataString: public data_t
{
private:
	std::string value;
public:
	dataString(std::string);
	dataString* copy();
	bool operator < (const data_t&) const;
	using data_t::operator >;
	using data_t::operator ==;
	virtual std::string get() const;
};

std::ostream& operator<<(std::ostream&, const data_t&);
