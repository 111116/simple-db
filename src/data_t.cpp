#include "data_t.h"

data_t::data_t(int value) : value(value) {}

bool dataInt::operator < (const dataInt& b) const override {
	return std::stoi(value) < std::stoi(b.value);
}

bool dataInt::operator > (const dataInt& b) const override {
	return std::stoi(value) > std::stoi(b.value);
}

bool dataInt::operator == (const dataInt& b) const override {
	return std::stoi(value) == std::stoi(b.value);
}

bool dataDouble::operator < (const dataDouble& b) const override {
	return std::stod(value) < std::stod(b.value);
}

bool dataDouble::operator > (const dataDouble& b) const override {
	return std::stod(value) > std::stod(b.value);
}

bool dataDouble::operator == (const dataDouble& b) const override {
	return std::stod(value) == std::stod(b.value);
}

bool dataString::operator < (const dataString& b) const override {
	return stringToLower(value) < stringToLower(b.value);
}

bool dataString::operator > (const dataString& b) const override {
	return stringToLower(value) > stringToLower(b.value);
}

bool dataString::operator == (const dataString& b) const override {
	return stringToLower(value) == stringToLower(b.value);
}

std::string stringToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
