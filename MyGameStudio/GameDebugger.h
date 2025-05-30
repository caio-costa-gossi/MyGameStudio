#pragma once
#include "Err.h"
#include <Windows.h>

class GameDebugger
{
private:
	static std::string debuggerExePath_;
	static HANDLE hConsoleWriteTo_;
	static PROCESS_INFORMATION debugConsoleInfo_;

	static std::atomic<bool> runDebugger_;

	static Err UpdateDebuggerProcessInfo();
	static Err EndDebuggerProcess();

public:
	static Err Startup();
	static void Run();
	static Err Shutdown();
};
