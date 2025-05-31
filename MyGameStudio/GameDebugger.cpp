#include "GameDebugger.h"
#include "SystemsInfoHelper.h"
#include "TerminalFactory.h"

Err GameDebugger::Startup()
{
	if (runDebugger_)
		return error_const::GAME_DEBUGGER_ALREADY_RUNNING;

	runDebugger_ = true;

	// Init debugger console
	const uint64_t scrX = SystemsInfoHelper::GetScreenWidth();
	const uint64_t scrY = SystemsInfoHelper::GetScreenHeight();

	const CreateTerminalInfo info = { debuggerExePath_, "Game Debugger", scrX / 2, 0, scrX / 2, scrY / 4 };
	const NewTerminal debugTerminal = TerminalFactory::CreateTerminal(info, true);

	if (debugTerminal.ProcessInfo.hProcess == nullptr)
		return error_const::TERMINAL_OPEN_FAILURE;

	if (debugTerminal.WriteHandle == nullptr)
		return error_const::TERMINAL_PIPE_OPEN_FAILURE;

	hConsoleWriteTo_ = debugTerminal.WriteHandle;
	debugConsoleInfo_ = debugTerminal.ProcessInfo;

	return error_const::SUCCESS;
}

void GameDebugger::Run()
{
	while (runDebugger_)
	{
		// Poll information

		// Write to process pipe
	}
}

Err GameDebugger::Shutdown()
{
	Err err = UpdateDebuggerProcessInfo();
	if (err.Code())
		return err;

	if (!runDebugger_)
		return error_const::GAME_DEBUGGER_NOT_RUNNING;

	runDebugger_ = false;

	err = EndDebuggerProcess();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err GameDebugger::EndDebuggerProcess()
{
	TerminateProcess(debugConsoleInfo_.hProcess, 0);
	CloseHandle(debugConsoleInfo_.hThread);
	CloseHandle(debugConsoleInfo_.hProcess);

	return error_const::SUCCESS;
}

Err GameDebugger::UpdateDebuggerProcessInfo()
{
	if (!runDebugger_)
		return error_const::SUCCESS;

	const DWORD result = WaitForSingleObject(debugConsoleInfo_.hProcess, 0);

	if (result == WAIT_OBJECT_0)
	{
		CloseHandle(debugConsoleInfo_.hThread);
		CloseHandle(debugConsoleInfo_.hProcess);
		runDebugger_ = false;
	}

	return error_const::SUCCESS;
}

std::atomic<bool> GameDebugger::runDebugger_ = false;
std::string GameDebugger::debuggerExePath_ = "binaries/game/debugger.exe";
HANDLE GameDebugger::hConsoleWriteTo_;
PROCESS_INFORMATION GameDebugger::debugConsoleInfo_;