#include <iostream>
#include "Err.h"
#include "MasterLoopManager.h"
#include "GameConsoleManager.h"

int main(int argc, char** argv)
{
	std::cout << "Initializing game systems...\n";
	Err err = MasterLoopManager::Startup(argc, argv);
	if (err.Code())
		GameConsoleManager::PrintError(err.Message());

	GameConsoleManager::PrintInfo("Running game. WARNING: Loop currently executes infinitely. Please use qgame to quit it.\n");
	err = MasterLoopManager::Run();
	if (err.Code())
		GameConsoleManager::PrintError(err.Message());

	GameConsoleManager::PrintInfo("Shutting game down...\n");
	err = MasterLoopManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err.Message());

	return 0;
}
