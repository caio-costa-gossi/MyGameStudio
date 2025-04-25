#include <cassert>

#include "AssetDatabase.h"
#include "CommandFactory.h"
#include "ConsoleManager.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "SqliteDatabase.h"
#include <zlib.h>

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

	//ConsoleManager::RunConsole();

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
