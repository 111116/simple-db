#include "data_t.h"

bool dataInt::operator < (const dataInt& b) {
	return value < b.value;
}

bool dataInt::operator > (const dataInt& b) {
	return value > b.value;
}

bool dataInt::operator == (const dataInt &b) {
	return value == b.value;
}

bool dataDouble::operator < (const dataDouble& b) {
	return value < b.value;
}

bool dataDouble::operator > (const dataDouble& b) {
	return value > b.value;
}

bool dataDouble::operator == (const dataDouble &b) {
	return value == b.value;
}

bool dataString::operator < (const dataString& b) {
	return value < b.value;
}

bool dataString::operator > (const dataString& b) {
	return value > b.value;
}

bool dataString::operator == (const dataString &b) {
	return value == b.value;
}
