#include "data_t.h"

//data_t::data_t(std::string value) : value(value) {}

dataInt::dataInt(int value): value(value) {}
dataInt::dataInt(std::string value): value(std::stoi(value)) {}

dataDouble::dataDouble(double value): value(value) {}
dataDouble::dataDouble(std::string value): value(std::stod(value)) {}

dataString::dataString(std::string str)
{
	value = "";
	unsigned i = 0, j = str.length() - 1;
	while (str[i] != '\'' && str[i] != '\"') ++i;
	while (str[j] != str[i]) --j;
	for (unsigned k = i + 1; k < j; ++k)
	{
		if (str[k] == '\\')
		{
			++k;
			if (str[k] == '0') value.push_back('\0');
			else if (str[k] == 'b') value.push_back('\b');
			else if (str[k] == 'n') value.push_back('\n');
			else if (str[k] == 'r') value.push_back('\r');
			else if (str[k] == 't') value.push_back('\t');
			else if (str[k] == 'Z') value.push_back('\x1a');
			else value.push_back(str[k]);
		}
		else if (str[k] == str[i])
			value.push_back(str[++k]);
		else
			value.push_back(str[k]);
	}
}

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
	compareHelper(dataInt, );
	compareHelper(dataDouble, );
	throw "Compare: data type mismatch";
}

bool dataDouble::operator < (const data_t &b) const
{
	compareHelper(dataInt, );
	compareHelper(dataDouble, );
	throw "Compare: data type mismatch";
}

bool dataString::operator < (const data_t &b) const
{
	compareHelper(dataString, ); // String compare is case-sensitive (different from MySQL default), so here we don't need to convert lowercase
	throw "Compare: data type mismatch";
}

#undef compareHelper
/*
std::string& get() const
{
	return value;
}*/

std::string dataInt::get() const
{
	return std::to_string(value);
}

std::string dataDouble::get() const
{
	return std::to_string(value);
}

std::string dataString::get() const
{
	return value;
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

dataInt* dataInt::copy()
{
	return new dataInt(this->value);
}

dataDouble* dataDouble::copy()
{
	return new dataDouble(this->value);
}

dataString* dataString::copy()
{
	return new dataString(this->value);
}
