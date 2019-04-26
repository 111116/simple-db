#pragma once

#include <string>

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
	virtual bool operator < (const dataInt&);
	virtual bool operator > (const dataInt&);
	virtual bool operator == (const dataInt&);
};

class dataDouble : public data_t
{
private:
	double value;
public:
	virtual bool operator < (const dataDouble&);
	virtual bool operator > (const dataDouble&);
	virtual bool operator == (const dataDouble&);
};

class dataString : public data_t
{
private:
	std::string value;
public:
	virtual bool operator < (const dataString&);
	virtual bool operator > (const dataString&);
	virtual bool operator == (const dataString&);
};
