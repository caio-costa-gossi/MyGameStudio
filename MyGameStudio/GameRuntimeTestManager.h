#pragma once
#include "windows.h"
#include "Err.h"

class GameRuntimeTestManager
{
private:
	static bool isGameRunning_;
	static PROCESS_INFORMATION gameProcessInformation_;

	static Err EndGameProcess();
	static Err UpdateGameProcessStatus();

public:
	static Err RunGame();
	static Err QuitGame();

	static Err SetGameRunning(const PROCESS_INFORMATION& gameProcess);
	static bool IsGameRunning();
};
