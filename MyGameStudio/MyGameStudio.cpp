#include <windows.h>

#include "ConfigManager.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "FileManager.h"

void MyCallback(FileIoTask* caller)
{
	std::cout << "\n\nI'm in the callback!\n\n";
	std::cout << *caller->StreamBuffer << std::endl;
}

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

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
