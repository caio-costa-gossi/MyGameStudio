#include "ConfigManager.h"

Err ConfigManager::Startup(const char* filePath)
{
	reader_.LoadConfig(filePath);
	return error_const::SUCCESS;
}

Err ConfigManager::SetObject(const char* object)
{
	currentObject_ = object;
	return error_const::SUCCESS;
}

const char* ConfigManager::GetConfig(const char* config)
{
	return reader_.GetConfig(currentObject_, config);
}

const char* ConfigManager::GetConfigForObject(const char* object, const char* config)
{
	return reader_.GetConfig(object, config);
}

const char* ConfigManager::currentObject_;
ConfigReader ConfigManager::reader_;