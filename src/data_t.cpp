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
	compareHelper(dataString, ); // String compare is case-sensitive (different from MySQL default), so here we don't need to convert lowercase
	throw badTypeComparison();
}

#undef compareHelper

/**
 * Converts a string to lowercase.
 *
 * @param String to convert
 * @return Lowercase string
*/
std::string stringToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

/**
 * Returns a data_t pointer from a string literal.
 *
 * @param A string literal
 * @return A derived data_t pointer storing according value
*/
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
				if (str[i] == '-' && i == 0)
					flag = -flag;
				else if (!isdigit(str[i])) // Not a digit => not an int
				{
					value = 1ll << 60; // This value is obviously not an int
					break; // Break to get a double from this literal
				}
				else
					(value *= 10) += str[i] - '0';
			}
			value *= flag;
			if (value >= (int)0x80000000 && value <= (int)0x7fffffff)
				return new dataInt(str);
		}
		return new dataDouble(str);
	}
}
