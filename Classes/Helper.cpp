#include "Helper.h"


std::array<std::string, 2> Splitext(const std::string &filename)
{
	std::array<std::string, 2> text;
	std::string::size_type pos = filename.rfind('.');
	if (std::string::npos != pos)
	{
		text[1] = filename.substr(pos);
		text[0] = filename.substr(0, pos);
	}
	else
	{
		text[0] = filename;
	}
	return text;
}

std::vector<std::string> Split(const std::string &str, const char *sep, size_t maxsplit)
{
	size_t len = strlen(sep);
	std::vector<std::string> result;
	std::string::size_type offset = 0;
	std::string::size_type current_pos = str.find(sep);

	while (current_pos != std::string::npos && result.size() < maxsplit)
	{
		if (current_pos - offset > 0)
		{
			result.push_back(str.substr(offset, current_pos - offset));
		}
		offset = current_pos + len;
		current_pos = str.find(sep, offset);
	}
	if (offset != str.size() && result.size() < maxsplit)
	{
		result.push_back(str.substr(offset));
	}

	return result;
}