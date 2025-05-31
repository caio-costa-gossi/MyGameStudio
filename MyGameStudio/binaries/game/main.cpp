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
	//MasterLoopManager::Run();

	volatile double x = 0.0001;

    while (true) {
        x = std::exp(std::sin(std::sqrt(std::abs(std::log(x + 1e-10) + 1.0))));
    }

	return 0;
}
