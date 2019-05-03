#pragma once

#include <string>
#include <algorithm>

class data_t
{
public:
	virtual ~data_t();
	static data_t* fromLiteral(std::string);
	virtual data_t* copy();
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
	dataInt(int);
	dataInt(std::string);
	dataInt* copy();
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
	dataDouble(double);
	dataDouble(std::string);
	dataDouble* copy();
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
	dataString(std::string);
	dataString* copy();
	bool operator < (const data_t&) const;
	bool operator > (const data_t&) const;
	bool operator == (const data_t&) const;
	virtual std::string get() const;
};
