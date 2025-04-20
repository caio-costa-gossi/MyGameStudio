#pragma once
#include "Err.h"

class ConsoleManager
{
private:
	static bool consoleRunning_;

	static char* fullCommand_;
	static char* mainCommand_;
	static uint8_t argc_;
	static char** argv_;
	static char** argn_;

	static void AllocMem();
	static void DeleteMem();
	static void ReceiveCommand();
	static void ParseCommand();

public:
	static Err Startup();
	static Err Shutdown();
	static void RunConsole();
	static void StopConsole();
};
