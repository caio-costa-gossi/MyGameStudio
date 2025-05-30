#pragma once
#include <string>
#include "Err.h"

class SystemPathHelper
{
public:
	static std::string GetFileName(const std::string& filepath);
	static std::string GetFileExtension(const std::string& filename);
	static std::string RemoveFileExtension(const std::string& filename);
	static Err WinSeparatorToUnix(std::string& string);
};
