#include "TerminalFactory.h"
#include "Err.h"
#include "../MyGameStudio/ConsoleManager.h"

PROCESS_INFORMATION TerminalFactory::CreateTerminal(const std::string& command)
{
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	STARTUPINFOA si = 
	{
		sizeof(si),
		nullptr,
		nullptr,
		const_cast<LPSTR>("Game Console"),
		static_cast<DWORD>(screenWidth / 2),
		static_cast<DWORD>(screenHeight / 2),
		static_cast<DWORD>(screenWidth / 2),
		static_cast<DWORD>(screenHeight / 2),
		0,
		0,
		0,
		STARTF_USEPOSITION | STARTF_USESIZE,
		0,
		0,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	};

	PROCESS_INFORMATION pi;

	CreateProcessA(
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
	);

	return pi;
}
