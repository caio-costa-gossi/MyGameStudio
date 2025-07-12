#pragma once
#include "Err.h"
#include <string>

class SystemFileHelper
{
private:


public:
	static Err RemoveFile(const char* filepath);
	static Err ReadFileString(const char* filepath, std::string& fileContent);
};
