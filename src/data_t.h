#pragma once

#include <string>
#include <algorithm>

class data_t
{
public:
	virtual bool operator < (const data_t&) = 0;
	virtual bool operator > (const data_t&) = 0;
	virtual bool operator == (const data_t&) = 0;
};

class dataInt : public data_t
{
private:
	int value;
public:
	dataInt(int);
	dataInt(std::string);
	virtual bool operator < (const dataInt&) const;
	virtual bool operator > (const dataInt&) const;
	virtual bool operator == (const dataInt&) const;
};

class dataDouble : public data_t
{
private:
	double value;
public:
	dataDouble(double);
	dataDouble(std::string);
	virtual bool operator < (const dataDouble&) const;
	virtual bool operator > (const dataDouble&) const;
	virtual bool operator == (const dataDouble&) const;
};

class dataString : public data_t
{
private:
	std::string value;
public:
	dataString(std::string);
	virtual bool operator < (const dataString&) const;
	virtual bool operator > (const dataString&) const;
	virtual bool operator == (const dataString&) const;
};

std::string stringToLower(std::string);
