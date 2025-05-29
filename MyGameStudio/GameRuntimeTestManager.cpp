#include "GameRuntimeTestManager.h"

Err GameRuntimeTestManager::SetGameRunning(const PROCESS_INFORMATION& gameProcess)
{
	if (isGameRunning_)
		return error_const::GAME_ALREADY_RUNNING;

	isGameRunning_ = true;
	gameProcessInformation_ = gameProcess;

	return error_const::SUCCESS;
}

Err GameRuntimeTestManager::EndGameProcess()
{
	if (!isGameRunning_)
		return error_const::GAME_NOT_RUNNING;

	TerminateProcess(gameProcessInformation_.hProcess, 0);
	CloseHandle(gameProcessInformation_.hThread);
	CloseHandle(gameProcessInformation_.hProcess);

	isGameRunning_ = false;

	return error_const::SUCCESS;
}

bool GameRuntimeTestManager::IsGameRunning()
{
	return isGameRunning_;
}

bool GameRuntimeTestManager::isGameRunning_;
PROCESS_INFORMATION GameRuntimeTestManager::gameProcessInformation_;