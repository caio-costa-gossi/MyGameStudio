#include "TerminalFactory.h"
#include "Err.h"
#include "../MyGameStudio/ConsoleManager.h"

PROCESS_INFORMATION TerminalFactory::CreateTerminal(const std::string& command)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	STARTUPINFOA si = 
	{
		sizeof(si),
		nullptr,
		nullptr,
		(LPSTR) "Game Console",
		static_cast<DWORD>(screenWidth / 2),
		static_cast<DWORD>(screenHeight / 2),
		static_cast<DWORD>(screenWidth / 2),
		static_cast<DWORD>(screenHeight / 2),
		0,
		0,
		0,
		STARTF_USEPOSITION | STARTF_USESIZE
	};

	PROCESS_INFORMATION pi;

	if (!CreateProcessA(
		nullptr,
		const_cast<LPSTR>(command.c_str()),
		nullptr,
		nullptr,
		FALSE,
		CREATE_NEW_CONSOLE,
		nullptr,
		nullptr,
		&si,
		&pi
	))
	{
		ConsoleManager::Print("Could not create the process", enums::ConsoleMessageType::error);
	}

	return pi;
}
