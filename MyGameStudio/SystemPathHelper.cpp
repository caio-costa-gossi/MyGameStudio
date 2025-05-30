#include "SystemPathHelper.h"

std::string SystemPathHelper::GetFileName(const std::string& filepath)
{
	return filepath.substr(filepath.find_last_of('/') + 1);
}

std::string SystemPathHelper::GetFileExtension(const std::string& filename)
{
	return filename.substr(filename.find_last_of('.') + 1);
}

std::string SystemPathHelper::RemoveFileExtension(const std::string& filename)
{
	return filename.substr(0, filename.find_last_of('.'));
}

Err SystemPathHelper::WinSeparatorToUnix(std::string& string)
{
	for (size_t i = 0; i < string.size(); ++i)
	{
		if (string[i] == '\\')
			string.replace(i, 1, "/");
	}

	return error_const::SUCCESS;
}
