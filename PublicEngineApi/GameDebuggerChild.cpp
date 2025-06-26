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
	debugInfo_ = "";

	// Gamepads
	for (uint8_t padIndex = 0; padIndex < currentState.GamepadCount; ++padIndex)
	{
		const Gamepad gamepad = currentState.Gamepads[padIndex];
		debugInfo_ += "====GAMEPAD " + std::to_string(padIndex) + "====\n\n";

		// Buttons
		debugInfo_ += "Buttons: " + std::bitset<32>(gamepad.State.BtnState).to_string();
		debugInfo_ += "\n";

		// Axes
		for (uint8_t axisId = 0; axisId < gamepad_axis_count; ++axisId)
			debugInfo_ += "Axis " + std::to_string(axisId) + ": " + std::to_string(gamepad.State.AxisState[axisId]) + " || ";
	}

	debugInfo_ += "\n";

	// Keyboard
	debugInfo_ += "\n====KEYBOARD====\n\n";
	for (const bool keyState : currentState.KeyboardState.RawState)
		debugInfo_ += std::to_string(keyState);

	// Mouse
	debugInfo_ += "\n\n====MOUSE====\n\n";
	debugInfo_ += "Buttons: " + std::bitset<8>(currentState.MouseState.BtnState).to_string();
	debugInfo_ += "\nXPos: " + std::to_string(currentState.MouseState.XPos) + " || YPos: " + std::to_string(currentState.MouseState.YPos);
	debugInfo_ += "\nXVel: " + std::to_string(currentState.MouseState.XVel) + " || YVel: " + std::to_string(currentState.MouseState.YVel);
	debugInfo_ += "\nWheel XVel: " + std::to_string(currentState.MouseState.WheelXVel) + " || Wheel YVel: " + std::to_string(currentState.MouseState.WheelYVel);
}


enums::GameDebugType GameDebuggerChild::debugType_ = enums::no_debug_from_child;
std::string GameDebuggerChild::debugInfo_;