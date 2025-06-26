#include "GameDebuggerChild.h"

#include <bitset>

#include "Enums.h"
#include "GameConsoleManager.h"
#include "InputManager.h"
#include "InputState.h"
#include "Win32PipeManager.h"

Err GameDebuggerChild::InitDebuggerPipe(const int argc, char** argv)
{
	if (argc <= 2)
		return error_const::INVALID_PARAMETERS;

	debugType_ = static_cast<enums::GameDebugType>(std::strtol(argv[1], nullptr, 10));
	GameConsoleManager::PrintInfo("Debug initialized. DebugType selected: " + std::to_string(debugType_));

	const std::string pipeName = argv[2];
	GameConsoleManager::PrintInfo("Debug pipe name: " + pipeName);

	const HANDLE hPipe = Win32PipeManager::GetPipeHandleFromName(pipeName);

	Err err = Win32PipeManager::SetPipeHandle(hPipe);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err GameDebuggerChild::SendInfo()
{
	switch (debugType_)
	{
	case enums::input_debug:
		GetInputDebugInfo();
		break;

	case enums::no_debug_from_child:
	default:
		debugInfo_ = "undefined";
		break;
	}

	Err err = Win32PipeManager::SendPipeMessage(debugInfo_);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err GameDebuggerChild::Shutdown()
{
	Err err = Win32PipeManager::ClosePipeHandle();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

void GameDebuggerChild::GetInputDebugInfo()
{
	const InputState currentState = InputManager::GetInputState();
	debugInfo_ = std::bitset<32>(currentState.Gamepads[0].State.BtnState).to_string();
}


enums::GameDebugType GameDebuggerChild::debugType_ = enums::no_debug_from_child;
std::string GameDebuggerChild::debugInfo_;