#pragma once

class data_t
{
public:
	virtual bool operator < (const data_t&) = 0;
	virtual bool operator > (const data_t&) = 0;
	virtual bool operator == (const data_t&) = 0;
};