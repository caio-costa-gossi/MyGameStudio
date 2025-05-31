#pragma once
#include "Err.h"
#include <Windows.h>

class GameDebugger
{
private:
	static std::string debuggerExePath_;
	static HANDLE hConsoleWriteTo_;
	static PROCESS_INFORMATION debugConsoleInfo_;
	static PROCESS_INFORMATION gameInfo_;

	static std::atomic<bool> runDebugger_;

	static Err UpdateDebuggerProcessInfo();
	static Err EndDebuggerProcess();

	static std::string FormatFloat(float number);

public:
	static Err Startup(const PROCESS_INFORMATION& gameInfo);
	static void Run();
	static Err Shutdown();
};
