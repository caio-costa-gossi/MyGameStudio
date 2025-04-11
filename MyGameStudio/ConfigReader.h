#pragma once
#include <memory>
#include <unordered_map>

class ConfigReader
{
private:
	std::unordered_map<const char*, std::unordered_map<const char*, std::unique_ptr<char[]>>> configMap_;

public:
	explicit ConfigReader(const char* fileName);

	const char* GetConfig(const char* user, const char* config);
	const std::unordered_map<const char*, std::unique_ptr<char[]>>& GetUserMap(const char* user);
};
