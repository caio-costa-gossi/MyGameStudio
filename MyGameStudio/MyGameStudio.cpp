#include <windows.h>

#include "ConfigManager.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "FileManager.h"

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

	char* myBuffer = new char[2048];
	char* buffer2 = new char[10000];

	const FileIoTaskJanitor task = FileManager::ReadFileAsync(1, "config.ini", &myBuffer, 2048);

	while (true)
	{
		if (task.GetTask().TaskState == enums::IoTaskState::finished || task.GetTask().TaskState == enums::IoTaskState::error)
		{
			std::cout << "Task completed!";
			std::cout << myBuffer;
			break;
		}
	}

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
