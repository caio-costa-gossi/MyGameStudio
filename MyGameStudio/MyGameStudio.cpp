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

	ConfigManager::SetObject("user");
	std::cout << ConfigManager::GetConfig("language");

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
