#include "MasterLoopManager.h"
#include "AnimationManager.h"
#include "GameConsoleManager.h"
#include "GameDebuggerChild.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "RenderingManager.h"
#include "TestWindowCreator.h"

Err MasterLoopManager::Run()
{
	loopRunning_ = true;

	while (loopRunning_)
	{
		if (debug_)
		{
			Err err = GameDebuggerChild::SendInfo();
			if (err.Code())
				GameConsoleManager::PrintError(err);
		}
		
		UpdateGame();

		Sleep(50);
	}

	return error_const::SUCCESS;
}

Err MasterLoopManager::Startup(const int argc, char** argv)
{
	// Main game timeline
	mainGameTimeline_ = Timeline(timeline::MICROSECOND);

	if (argc > 1)
		debug_ = true;

	Err err = GameConsoleManager::Startup();
	if (err.Code())
	{
		std::cout << error_const::CONSOLE_MANAGER_STARTUP_FAIL.Message();
		return err;
	}

	// Subsystem startup

	// Debugging information communication pipe
	if (debug_)
	{
		err = GameDebuggerChild::InitDebuggerPipe(argc, argv);
		if (err.Code())
			return err;
	}

	// Test window creator for input handling
	err = TestWindowCreator::Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	/*err = InputManager::Startup(nullptr);
	if (err.Code())
		GameConsoleManager::PrintError(err);*/

	GameConsoleManager::PrintInfo("Starting GameObjectManager...");
	err = GameObjectManager::Get().Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	GameConsoleManager::PrintInfo("Starting PhysicsManager...");
	err = PhysicsManager::Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	GameConsoleManager::PrintInfo("Starting AnimationManager...");
	err = AnimationManager::Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	GameConsoleManager::PrintInfo("Starting RenderingManager...");
	err = RenderingManager::Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	// Start time
	mainGameTimeline_.Start();

	return error_const::SUCCESS;
}

Err MasterLoopManager::Shutdown()
{
	// Stop time
	mainGameTimeline_.Pause();

	// Shutdown subsystems

	/*Err err = InputManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err);*/

	Err err = TestWindowCreator::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	if (debug_)
	{
		err = GameDebuggerChild::Shutdown();
		if (err.Code())
			GameConsoleManager::PrintError(err);
	}

	err = GameObjectManager::Get().Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	err = PhysicsManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	err = AnimationManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	err = RenderingManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err);

	return error_const::SUCCESS;
}

Err MasterLoopManager::UpdateGame()
{
	// Update time
	mainGameTimeline_.UpdateLastTime();

	// Update subsystems
	TestWindowCreator::Update();
	//InputManager::Update();
	GameObjectManager::Get().Update(mainGameTimeline_.GetDelta());
	PhysicsManager::Update();
	AnimationManager::Update();
	RenderingManager::Update();

	return error_const::SUCCESS;
}

Err MasterLoopManager::Stop()
{
	loopRunning_ = false;
	return error_const::SUCCESS;
}

bool MasterLoopManager::loopRunning_;
bool MasterLoopManager::debug_ = false;
Timeline MasterLoopManager::mainGameTimeline_;