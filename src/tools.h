#pragma once

#include <string>
#include <vector>
#include <iostream>

// return readline (splited)
std::vector<std::string> split(std::string raw)
{
	raw.push_back('\n');
	std::vector<std::string> res;
	std::string current;
	bool inString = false;
	char stringDelimiter; // surrounding delimiter, if inside a string literal

	auto split = [&]()
	{
		if (!current.empty())
		{
			res.push_back(std::move(current));
			current = "";
		}
	};
	for (int i=0; i<raw.length(); ++i)
	{
		if (inString)
		{
			if (raw[i] == '\n')
				throw "string literal missing terminating character";
			current += raw[i];
			if (raw[i] == stringDelimiter)
			{
				if (raw[i+1] == stringDelimiter)
				{
					current += stringDelimiter;
					i++;
					current += raw[i];
				}
				else // end of a string literal
				{
					inString = false;
				}
			}
			if (raw[i] == '\\' && raw[i+1] != '\n')
			{
				i++;
				current += raw[i];
			}
			continue;
		}
		// outside a string literal
		// split at whitespace character
		if (raw[i] == ' ' || raw[i] == '\t' || raw[i] == '\n')
		{
			split();
			continue;
		}
		// if string literal starts
		if (raw[i] == '\'' || raw[i] == '\"')
		{
			split();
			inString = true;
			stringDelimiter = raw[i];
			current = raw[i];
			continue;
		}
		// if in [_0-9a-zA-Z], treat as valid char
		if (isdigit(raw[i]) || isalpha(raw[i]) || raw[i] == '_')
		{
			current += raw[i];
			continue;
		}
		// other spliting chars
		split();
		res.push_back({raw[i]});
	}
	return res;
}