#pragma once

class data_t
{
public:
	virtual bool operator < (const data_t&) = 0;
	virtual bool operator > (const data_t&) = 0;
	virtual bool operator == (const data_t&) = 0;
};

class dataInt : public data_t
{};

class dataDouble : public data_t
{};

class dataString : public data_t
{};
