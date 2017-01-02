#include "Utilities.h"
#include <sstream>
std::vector<std::string> Utilities::SplitString(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) 
	{
		if (item != "")
		{
			elems.push_back(item);
		}
	}
	return elems;
}
std::vector<std::string> Utilities::SplitString(const std::string &s, const char* delims)
{
	std::vector<std::string> elems;
	std::stringstream stringStream(s);
	std::string line;
	while (std::getline(stringStream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(delims, prev)) != std::string::npos)
		{
			if (pos > prev)
				elems.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length())
			elems.push_back(line.substr(prev, std::string::npos));
	}
	return elems;
}