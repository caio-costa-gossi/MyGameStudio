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

	char* myBuffer = new char[2048];
	char* buffer2 = new char[10000];

	const FileIoTaskJanitor task = FileManager::ReadFileAsync(1, "config.ini", &myBuffer, 2048, MyCallback);
	const FileIoTaskJanitor task2 = FileManager::ReadFileAsync(2, "strings.csv", &buffer2, 10000, MyCallback);

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
