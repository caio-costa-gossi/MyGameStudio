#include "ConfigManager.h"
#include "EngineCoreManager.h"
#include "Err.h"

int main()
{
	Err err = EngineCoreManager::Startup();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}

	err = EngineCoreManager::Config();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}

	ConfigManager::Startup("D:\\Documents\\Programs\\MyGameStudio\\MyGameStudio\\MyGameStudio\\x64\\Debug\\config.ini");
	ConfigManager::SetObject("general");
	const char* string = ConfigManager::GetConfig("language");

	std::cout << string;

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
