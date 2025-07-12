#include "SystemFileHelper.h"
#include <fstream>
#include <sstream>

Err SystemFileHelper::RemoveFile(const char* filepath)
{
	if (std::remove(filepath))
		return error_const::FILE_DELETION_FAIL;

	return error_const::SUCCESS;
}

Err SystemFileHelper::ReadFileString(const char* filepath, std::string& fileContent)
{
	const auto file = std::ifstream(filepath);

	if (!file)
		return error_const::FILE_NOT_FOUND;

	std::stringstream buffer;
	buffer << file.rdbuf();

	fileContent = buffer.str();

	return error_const::SUCCESS;
}
