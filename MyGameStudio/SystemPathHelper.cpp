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
