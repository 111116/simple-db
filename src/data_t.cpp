#include "data_t.h"

data_t::data_t(std::string value) : value(value) {}

bool data_t::operator > (const data_t& b) const
{
	return b < *this;
}

bool data_t::operator == (const data_t& b) const
{
	return !(*this < b) && !(b < *this);
}

bool dataInt::operator < (const data_t &b) const
{
	const dataInt* dint = dynamic_cast<const dataInt*>(&b);
	if (dint)
		return std::stoi(value) < std::stoi(dint->value);
	const dataDouble* ddouble = dynamic_cast<const dataDouble*>(&b);
	if (ddouble)
		return std::stod(value) < std::stod(ddouble->value);
	throw badTypeComparison();
}

bool dataDouble::operator < (const data_t &b) const
{
	const dataInt* dint = dynamic_cast<const dataInt*>(&b);
	if (dint)
		return std::stod(value) < std::stod(dint->value);
	const dataDouble* ddouble = dynamic_cast<const dataDouble*>(&b);
	if (ddouble)
		return std::stod(value) < std::stod(ddouble->value);
	throw badTypeComparison();
}

bool dataString::operator < (const data_t &b) const
{
	const dataString* dstring = dynamic_cast<const dataString*>(&b);
	if (dstring)
		return stringToLower(value) < stringToLower(dstring->value);
	throw badTypeComparison();
}

std::string stringToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

data_t* data_t::fromLiteral(std::string str)
{
	if (str[0] == '\'' || str[0] == '\"')
		return new dataString(str);
	else
	{
		if (str.length() <= 10 || (str.length() == 11 && str[0] == '-'))
		{
			long long value = 0, flag = 1;
			for (unsigned i = 0; i < str.length(); ++i)
			{
				if (str[i] == '-') flag = -flag;
				else (value *= 10) += str[i] - '0';
			}
			if (value >= (int)0x80000000 && value <= (int)0x7fffffff)
				return new dataInt(str);
		}
		return new dataDouble(str);
	}
}
