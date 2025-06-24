#include "GameDebugger.h"

#include <bitset>
#include <sstream>
#include <iomanip>

#include "ConsoleManager.h"
#include "ProcessResourceViewer.h"
#include "SystemsInfoHelper.h"
#include "TerminalFactory.h"
#include "InputManager.h"

Err GameDebugger::Startup(const PROCESS_INFORMATION& gameInfo)
{
	if (runDebugger_)
		return error_const::GAME_DEBUGGER_ALREADY_RUNNING;

	gameInfo_ = gameInfo;
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
	resourceViewer_ = ProcessResourceViewer(gameInfo_);

	return error_const::SUCCESS;
}

void GameDebugger::Run(const bool inputDebug)
{
	uint32_t bytesWritten;

	while (runDebugger_)
	{
		std::string toWrite;

		// Poll information
		if (inputDebug)
			toWrite = GetInputInfo();
		else
			toWrite = GetUsageInfo();

		// Write to console
		WriteFile(hConsoleWriteTo_, toWrite.c_str(), static_cast<uint32_t>(toWrite.size()), reinterpret_cast<LPDWORD>(&bytesWritten), nullptr);

		Sleep(3000);
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

std::string GameDebugger::GetUsageInfo()
{
	const float cpuPercent = resourceViewer_.GetCpuUsage();
	const int64_t ramUsage = resourceViewer_.GetRamUsage() / 1024;

	// Write to process pipe
	return std::string("CPU: " + FormatFloat(cpuPercent) + "%, RAM: " + std::to_string(ramUsage) + "KB\n");
}

std::string GameDebugger::GetInputInfo()
{
	const InputState currentState = InputManager::GetInputState();

	std::string gamepadBtn = std::bitset<32>(currentState.Gamepads[0].State.BtnState).to_string();
	return gamepadBtn;
}

std::string GameDebugger::FormatFloat(const float number)
{
	std::ostringstream outStream;
	outStream << std::fixed << std::setprecision(2) << number;
	return outStream.str();
}


std::atomic<bool> GameDebugger::runDebugger_ = false;

std::string GameDebugger::debuggerExePath_ = "binaries/game/debugger.exe";
HANDLE GameDebugger::hConsoleWriteTo_;
PROCESS_INFORMATION GameDebugger::debugConsoleInfo_;
PROCESS_INFORMATION GameDebugger::gameInfo_;

ProcessResourceViewer GameDebugger::resourceViewer_;