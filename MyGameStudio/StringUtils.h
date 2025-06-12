#pragma once
#include <string>
#include "Err.h"

class StringUtils
{
private:


public:
	static Err ReplaceInString(std::string& string, const std::string& lookFor, const std::string& replaceFor);
	static bool CanParseAsBool(const std::string& string);
};
