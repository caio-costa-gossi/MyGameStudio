#include "GameRuntimeTestManager.h"

#include <thread>

#include "Enums.h"
#include "GameBuilder.h"
#include "GameDebugger.h"

Err GameRuntimeTestManager::RunGame(const enums::GameDebugType debugType)
{
	// Check if game is running already
	Err err = UpdateGameProcessStatus();
	if (err.Code())
		return err;

	if (isGameRunning_)
		return error_const::GAME_ALREADY_RUNNING;

	// Build game
	err = GameBuilder::BuildGame();
	if (err.Code())
		return err;

	// Run game
	err = GameBuilder::RunGame(debugType);
	if (err.Code())
		return err;

	// Init debug information console
	err = GameDebugger::Startup(gameProcessInformation_, debugType);
	if (err.Code())
		return err;

	// Run debug information console on another thread
	std::thread debugThread(GameDebugger::Run);
	debugThread.detach();

	return error_const::SUCCESS;
}

Err GameRuntimeTestManager::QuitGame()
{
	// Check if game is already stopped
	Err err = UpdateGameProcessStatus();
	if (err.Code())
		return err;

	if (!isGameRunning_)
		return error_const::GAME_NOT_RUNNING;

	// End debugger thread
	err = GameDebugger::Shutdown();
	if (err.Code())
		return err;

	// End game process
	EndGameProcess();

	return error_const::SUCCESS;
}

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

Err GameRuntimeTestManager::UpdateGameProcessStatus()
{
	if (!isGameRunning_)
		return error_const::SUCCESS;

	const DWORD result = WaitForSingleObject(gameProcessInformation_.hProcess, 0);

	if (result == WAIT_OBJECT_0)
	{
		CloseHandle(gameProcessInformation_.hThread);
		CloseHandle(gameProcessInformation_.hProcess);
		isGameRunning_ = false;
	}

	return error_const::SUCCESS;
}

bool GameRuntimeTestManager::IsGameRunning()
{
	return isGameRunning_;
}

bool GameRuntimeTestManager::isGameRunning_;
PROCESS_INFORMATION GameRuntimeTestManager::gameProcessInformation_;