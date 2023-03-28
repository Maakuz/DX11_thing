#pragma once
#include <string>
#include <vector>

class StringSplitter
{
public:
	static std::vector<std::string> split(std::string string, char splitOn)
	{
		std::string temp;
		std::vector<std::string> ret;
		for (const char& c : string)
		{
			if (c != splitOn)
				temp += c;

			else
			{
				ret.push_back(temp);
				temp = "";
			}
		}

		if (!temp.empty())
			ret.push_back(temp);

		return ret;
	}

};

