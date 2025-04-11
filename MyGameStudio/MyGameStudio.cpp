#include "ConfigReader.h"
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

	ConfigReader reader("D:\\Documents\\Programs\\MyGameStudio\\MyGameStudio\\MyGameStudio\\x64\\Debug\\config.ini");

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
