#pragma once
#include <Windows.h>

#include "ProcessResourceViewer.h"
#include "Err.h"

class GameDebugger
{
private:
	static std::string debuggerExePath_;
	static HANDLE hConsoleWriteTo_;
	static PROCESS_INFORMATION debugConsoleInfo_;
	static PROCESS_INFORMATION gameInfo_;

	static ProcessResourceViewer resourceViewer_;

	static std::atomic<bool> runDebugger_;

	static Err UpdateDebuggerProcessInfo();
	static Err EndDebuggerProcess();

	static std::string GetUsageInfo();
	static std::string GetInputInfo();

	static std::string FormatFloat(float number);

public:
	static Err Startup(const PROCESS_INFORMATION& gameInfo);
	static void Run(bool inputDebug);
	static Err Shutdown();
};
