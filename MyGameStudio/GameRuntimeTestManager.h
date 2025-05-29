#pragma once
#include "windows.h"
#include "Err.h"

class GameRuntimeTestManager
{
private:
	static bool isGameRunning_;
	static PROCESS_INFORMATION gameProcessInformation_;

public:
	static Err SetGameRunning(const PROCESS_INFORMATION& gameProcess);
	static Err EndGameProcess();

	static bool IsGameRunning();
};
