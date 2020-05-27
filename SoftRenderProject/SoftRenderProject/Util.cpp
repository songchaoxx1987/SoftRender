#include "define.h"

void split(std::string str, std::vector<std::string>& ret, char delim)
{
	int begin = 0;
	int i = 0;
	while (i < str.length())
	{
		if (str[i] == delim)
		{
			int cnt = i - begin;
			if (cnt > 0)
			{
				std::string temp;
				temp = str.substr(begin, i - begin);
				ret.push_back(temp);
			}
			begin = i + 1;
		}
		++i;
	}
	if (i > begin)
	{
		std::string temp;
		temp = str.substr(begin, i - begin);
		ret.push_back(temp);
	}
}