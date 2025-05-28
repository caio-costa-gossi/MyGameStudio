#include "MasterLoopManager.h"

#include "GameObjectManager.h"

Err MasterLoopManager::Run()
{
	loopRunning_ = true;

	while (loopRunning_)
	{
		RunGame();
	}

	return error_const::SUCCESS;
}

Err MasterLoopManager::Startup()
{
	GameObjectManager::Startup();

	return error_const::SUCCESS;
}

Err MasterLoopManager::Shutdown()
{
	return error_const::SUCCESS;
}

Err MasterLoopManager::Stop()
{
	loopRunning_ = false;
	return error_const::SUCCESS;
}

Err MasterLoopManager::RunGame()
{
	GameObjectManager
}
