#include "StringUtils.h"

Err StringUtils::ReplaceInString(std::string& string, const std::string& lookFor, const std::string& replaceFor)
{
	if (string.empty())
		return error_const::SUCCESS;

	size_t startPos = 0;
	while ((startPos = string.find(lookFor, startPos)) != std::string::npos)
	{
		string.replace(startPos, lookFor.length(), replaceFor);
		startPos += replaceFor.length();
	}

	return error_const::SUCCESS;
}

bool StringUtils::CanParseAsBool(const std::string& string)
{
	return std::strcmp(string.c_str(), "false") || std::strcmp(string.c_str(), "true");
}
