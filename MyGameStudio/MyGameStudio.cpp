#include "ConsoleManager.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "InputManager.h"
#include "TestWindowCreator.h"


int main()
{
	Err err = EngineCoreManager::Startup();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}

	err = EngineCoreManager::Config();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}

	//ConsoleManager::RunConsole();

	err = TestWindowCreator::Startup();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}

	err = TestWindowCreator::Run();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}

	err = TestWindowCreator::Shutdown();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}
}
