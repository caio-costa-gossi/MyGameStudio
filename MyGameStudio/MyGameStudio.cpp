#include "ConsoleManager.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "TerminalFactory.h"

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

	ConsoleManager::RunConsole();
	//PROCESS_INFORMATION pi = TerminalFactory::CreateTerminal("cmd.exe");

	/*TerminateProcess(pi.hProcess, 0);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);*/

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}
}
