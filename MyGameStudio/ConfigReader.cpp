#include "ConfigReader.h"
#include <fstream>

#include "Err.h"

ConfigReader::ConfigReader(const char* fileName)
{
	configMap_ = std::unordered_map<const char*, std::unordered_map<const char*, std::unique_ptr<char[]>>>();

	std::ifstream file(fileName, std::ios::in);

	auto buffer = std::unique_ptr<char[]>(new char[256]);

	while (file.getline(buffer.get(), 256, '\n'))
	{
		std::cout << buffer.get() << std::endl;
	}
}

const char* ConfigReader::GetConfig(const char* user, const char* config)
{
	return configMap_[user][config].get();
}

const std::unordered_map<const char*, std::unique_ptr<char[]>>& ConfigReader::GetUserMap(const char* user)
{
	return configMap_[user];
}

//Err ReadFile()