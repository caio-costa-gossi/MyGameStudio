#include <iostream>
#include <string>
#include "MasterLoopManager.h"

int main(int argc, char** argv)
{
	std::cout << "Initializing game systems...\n";
	MasterLoopManager::Startup();

	std::cout << "Running game. WARNING: Loop currently executes infinitely. Please use qgame to quit it.\n";
	MasterLoopManager::Run();

	return 0;
}
