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

std::string SystemPathHelper::RemoveRelativeSlash(const std::string& filepath)
{
	std::string returnValue = filepath;

	if (filepath[0] == '\\' || filepath[0] == '/')
	{
		return returnValue.erase(0, 1);
	}

	return returnValue;
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

Err SystemPathHelper::UnixSeparatorToWin(std::string& string)
{
	for (size_t i = 0; i < string.size(); ++i)
	{
		if (string[i] == '/')
			string.replace(i, 1, "\\");
	}

	return error_const::SUCCESS;
}

char SystemPathHelper::GetWinSeparator()
{
	return '\\';
}

char SystemPathHelper::GetUnixSeparator()
{
	return '/';
}
