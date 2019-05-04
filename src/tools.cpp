#include <algorithm>
#include <regex>
#include "tools.h"

// split a string
std::vector<std::string> split(std::string raw)
{
	raw.push_back('\n');
	std::vector<std::string> res;

	// accept "-?[0-9]+(\.[0-9]*)?" as number
	// accept "[_A-Za-z][_0-9A-Za-z]*" as identifier
	// accept "\'([^\'\\]|(\'\')|(\\.))*\'" as string
	// accept "\"([^\"\\]|(\"\")|(\\.))*\"" as string
	// accept "[-\+\*\/<>=,;\(\)<>=]" as separator

	// TODO
	return res;
}

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
