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

	ConfigManager::SetObject("general");
	const char* string = ConfigManager::GetConfig("language");

	//std::cout << LocalizationManager::GetLocalizedString("fala_3");

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
