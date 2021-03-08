#pragma once
#include <string>

class StringConverter
{
public:
	/* Converts a string to wide string */
	static std::wstring StringToWide(std::string str);
};