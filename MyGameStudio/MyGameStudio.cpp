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

	/*err = TestWindowCreator::StartTestWindow();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}

	err = TestWindowCreator::UpdateTestWindow();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}*/

	err = InputManager::Startup(nullptr);
	if (err.Code())
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

	while (true)
	{
		err = InputManager::Update();
		if (err.Code())
			ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

		Sleep(1 / 30);
	}

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}
}
