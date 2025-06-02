#include "MasterLoopManager.h"
#include "AnimationManager.h"
#include "GameConsoleManager.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "RenderingManager.h"

Err MasterLoopManager::Run()
{
	loopRunning_ = true;

	while (loopRunning_)
	{
		UpdateGame();
	}

	return error_const::SUCCESS;
}

Err MasterLoopManager::Startup()
{
	Err err = GameConsoleManager::Startup();
	if (err.Code())
	{
		std::cout << error_const::CONSOLE_MANAGER_STARTUP_FAIL.Message();
		return err;
	}

	err = InputManager::Startup();
	if (err.Code())
		GameConsoleManager::PrintError("Error code" + std::to_string(err.Code()) + ": " + err.Message());

	err = GameObjectManager::Startup();


	err = PhysicsManager::Startup();


	err = AnimationManager::Startup();


	err = RenderingManager::Startup();

	return error_const::SUCCESS;
}

Err MasterLoopManager::Shutdown()
{
	InputManager::Shutdown();
	GameObjectManager::Shutdown();
	PhysicsManager::Shutdown();
	AnimationManager::Shutdown();
	RenderingManager::Shutdown();

	return error_const::SUCCESS;
}

Err MasterLoopManager::UpdateGame()
{
	InputManager::Update();
	GameObjectManager::Update();
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