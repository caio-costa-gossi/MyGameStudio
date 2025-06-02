#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "MasterLoopManager.h"

int main(int argc, char** argv)
{
	std::cout << "Initializing game systems...\n";
	MasterLoopManager::Startup();

	std::cout << "Running game. WARNING: Loop currently executes infinitely. Please use qgame to quit it.\n";
	MasterLoopManager::Run();

	std::cout << "Shutting game down...\n";
	MasterLoopManager::Shutdown();
	return 0;
}
