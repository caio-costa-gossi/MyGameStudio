#pragma once
#include <windows.h>

#include "Enums.h"
#include "Err.h"

class ConsoleManager
{
private:
	static HANDLE hWinConsole_;

	static bool consoleRunning_;

	static char* fullCommand_;
	static char* usableCommand_;
	static char* mainCommand_;
	static uint8_t argc_;
	static char** argv_;
	static char** argn_;

	static void AllocMem();
	static void DeleteMem();
	static void ReceiveCommand();
	static void LTrimCommand();
	static void ParseCommand();
	static void ParseCommandAlternative();

	static void ReplaceWithTerminator(char* string, const uint16_t charCount);

public:
	static Err Startup();
	static Err Shutdown();
	static void RunConsole();
	static void StopConsole();

	static void Print(const std::string& message, enums::ConsoleMessageType type);
};
