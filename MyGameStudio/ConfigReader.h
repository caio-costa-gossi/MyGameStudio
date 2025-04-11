#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "CsvParser.h"

class ConfigReader
{
private:
	std::unordered_map<const char*, std::unordered_map<const char*, std::unique_ptr<char[]>, CStrHash, CStrEqual>, CStrHash, CStrEqual> configMap_;
	std::vector<std::unique_ptr<char[]>> stringStorage_ = std::vector<std::unique_ptr<char[]>>();

public:
	explicit ConfigReader(const char* fileName);

	const char* GetConfig(const char* user, const char* config);
	const std::unordered_map<const char*, std::unique_ptr<char[]>, CStrHash, CStrEqual>& GetUserMap(const char* user);
};
