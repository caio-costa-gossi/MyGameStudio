#pragma once
#include <string>
#include "Err.h"

class SystemPathHelper
{
public:
	static std::string GetFileName(const std::string& filepath);
	static std::string GetFileExtension(const std::string& filename);
	static std::string RemoveFileExtension(const std::string& filename);
	static std::string RemoveRelativeSlash(const std::string& filepath);
	static Err WinSeparatorToUnix(std::string& string);
	static Err UnixSeparatorToWin(std::string& string);

	static char GetWinSeparator();
	static char GetUnixSeparator();
};
