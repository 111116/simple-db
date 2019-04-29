#include "data_t.h"

dataInt::dataInt(int value) : value(value) {}

dataInt::dataInt(std::string strValue) : value(std::stoi(strValue)) {}

bool dataInt::operator < (const dataInt& b) const {
	return value < b.value;
}

bool dataInt::operator > (const dataInt& b) const {
	return value > b.value;
}

bool dataInt::operator == (const dataInt &b) const {
	return value == b.value;
}

dataDouble::dataDouble(double value) : value(value) {}

dataDouble::dataDouble(std::string strValue) : value(std::stod(strValue)) {}

bool dataDouble::operator < (const dataDouble& b) const {
	return value < b.value;
}

bool dataDouble::operator > (const dataDouble& b) const {
	return value > b.value;
}

bool dataDouble::operator == (const dataDouble &b) const {
	return value == b.value;
}

dataString::dataString(std::string value) : value(value) {}

bool dataString::operator < (const dataString& b) const {
	return stringToLower(value) < stringToLower(b.value);
}

bool dataString::operator > (const dataString& b) const {
	return stringToLower(value) > stringToLower(b.value);
}

bool dataString::operator == (const dataString &b) const {
	return stringToLower(value) == stringToLower(b.value);
}

std::string stringToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
