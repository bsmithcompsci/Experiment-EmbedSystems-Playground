#include "global/Utils.h"

std::vector<std::string> splitStrs(const std::string &_str, char _delimiter)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, cur_pos = 0;
	while ((cur_pos = _str.find(_delimiter, cur_pos)) != std::string::npos)
	{
		std::string substring(_str.substr(prev_pos, cur_pos - prev_pos));
		output.push_back(substring);
		prev_pos = ++cur_pos;
	}
	output.push_back(_str.substr(prev_pos, cur_pos - prev_pos)); // Catch the last bits.

	return output;
}
std::vector<int> splitInts(const std::string &_str, char _delimiter)
{
	std::vector<int> output;

	std::string::size_type prev_pos = 0, cur_pos = 0;
	while ((cur_pos = _str.find(_delimiter, cur_pos)) != std::string::npos)
	{
		std::string substring(_str.substr(prev_pos, cur_pos - prev_pos));
		output.push_back(atoi(substring.c_str()));
		prev_pos = ++cur_pos;
	}
	output.push_back(atoi(_str.substr(prev_pos, cur_pos - prev_pos).c_str())); // Catch the last bits.

	return output;
}