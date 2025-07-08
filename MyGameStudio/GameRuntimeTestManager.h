#pragma once
#include "Enums.h"
#include "windows.h"
#include "Err.h"

class GameRuntimeTestManager
{
private:
	static bool isGameRunning_;
	static PROCESS_INFORMATION gameProcessInformation_;

	static Err EndGameProcess();

public:
	static Err RunGame(enums::GameDebugType debugType = enums::no_debug_from_child, enums::ConsoleMessageType minLevel = enums::ConsoleMessageType::info, uint32_t activeChannelMask = 0xFFFF);
	static Err QuitGame();

	static Err SetGameRunning(const PROCESS_INFORMATION& gameProcess);

	static Err UpdateGameProcessStatus();
	static bool IsGameRunning();
};
