#pragma once
#include "ConfigReader.h"
#include "Err.h"

class ConfigManager
{
private:
	static ConfigReader reader_;
	static const char* currentObject_;

public:
	static Err Startup(const char* filePath);

	static Err SetObject(const char* object);

	static const char* GetConfigForObject(const char* object, const char* config);
	static const char* GetConfig(const char* config);
};
