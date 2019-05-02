#include "data_t.h"

data_t::data_t(std::string value) : value(value) {}

bool dataInt::operator < (const dataInt& b) const {
	return std::stoi(value) < std::stoi(b.value);
}

bool dataInt::operator > (const dataInt& b) const {
	return std::stoi(value) > std::stoi(b.value);
}

bool dataInt::operator == (const dataInt& b) const {
	return std::stoi(value) == std::stoi(b.value);
}

bool dataDouble::operator < (const dataDouble& b) const {
	return std::stod(value) < std::stod(b.value);
}

bool dataDouble::operator > (const dataDouble& b) const {
	return std::stod(value) > std::stod(b.value);
}

bool dataDouble::operator == (const dataDouble& b) const {
	return std::stod(value) == std::stod(b.value);
}

bool dataString::operator < (const dataString& b) const {
	return stringToLower(value) < stringToLower(b.value);
}

bool dataString::operator > (const dataString& b) const {
	return stringToLower(value) > stringToLower(b.value);
}

bool dataString::operator == (const dataString& b) const {
	return stringToLower(value) == stringToLower(b.value);
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
