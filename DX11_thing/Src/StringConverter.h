#pragma once
#include <string>
class StringConverter
{
public: 
	static std::wstring stringToWide(std::string str) 
	{
		return std::wstring(str.begin(), str.end());
	}
};

