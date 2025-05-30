#include "GameDebugger.h"

Err GameDebugger::Startup()
{
	return error_const::SUCCESS;
}

Err GameDebugger::Update()
{
	return error_const::SUCCESS;
}

Err GameDebugger::Shutdown()
{
	return error_const::SUCCESS;
}


std::string GameDebugger::debuggerExePath_ = "binaries/game/debugger.exe";
HANDLE GameDebugger::hConsoleWriteTo_;
PROCESS_INFORMATION GameDebugger::debugConsoleInfo_;