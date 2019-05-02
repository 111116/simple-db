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

/**
 * Macro for data comparison, work together with `bool operator<()`.
 *
 * @param Actual dataType for the right operand.
 * @param Conversion used to get primitive value for the operands. If no conversion is needed, just simply omit this parameter.
*/
#define compareHelper(dataType, convertFunction) \
{ \
	const dataType* dvar = dynamic_cast<const dataType*>(&b); \
	if (dvar) \
		return convertFunction(value) < convertFunction(dvar->value); \
}

bool dataInt::operator < (const data_t &b) const
{
	compareHelper(dataInt, std::stoi);
	compareHelper(dataDouble, std::stod);
	throw badTypeComparison();
}

bool dataDouble::operator < (const data_t &b) const
{
	compareHelper(dataInt, std::stod);
	compareHelper(dataDouble, std::stod);
	throw badTypeComparison();
}

bool dataString::operator < (const data_t &b) const
{
	compareHelper(dataString, stringToLower);
	throw badTypeComparison();
}

#undef compareHelper

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
			value *= flag;
			if (value >= (int)0x80000000 && value <= (int)0x7fffffff)
				return new dataInt(str);
		}
		return new dataDouble(str);
	}
}
