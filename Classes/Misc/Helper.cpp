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