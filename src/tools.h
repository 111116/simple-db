#pragma once

#include <string>
#include <vector>
#include <algorithm>

typedef std::vector<std::string> tokens; // 保存被分词的 MySQL 语句，包含原 MySQL 语句中的每个 token
typedef std::vector<std::string> attrs; // 保存若干个列的名称。虽然与 tokens 本质相同，但在程序中的语义不同

std::vector<std::string> split(std::string raw); // 对语句进行分词
std::string stringToLower(std::string str); // 将字符串中所有大写字母转换为小写
