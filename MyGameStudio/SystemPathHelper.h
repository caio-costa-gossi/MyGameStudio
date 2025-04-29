#pragma once
#include <string>

class SystemPathHelper
{
public:
	static std::string GetFileName(const std::string& filepath);
	static std::string GetFileExtension(const std::string& filename);
	static std::string RemoveFileExtension(const std::string& filename);
};
