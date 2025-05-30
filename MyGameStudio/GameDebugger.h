#pragma once
#include "Err.h"
#include <Windows.h>

class GameDebugger
{
private:
	static std::string debuggerExePath_;
	static HANDLE hConsoleWriteTo_;
	static PROCESS_INFORMATION debugConsoleInfo_;

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();
};
