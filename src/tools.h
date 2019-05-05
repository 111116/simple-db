#pragma once

#include <string>
#include <vector>
#include <algorithm>

typedef std::vector<std::string> tokens;
typedef std::vector<std::string> attrs;

// split a string
std::vector<std::string> split(std::string);

std::string stringToLower(std::string);
