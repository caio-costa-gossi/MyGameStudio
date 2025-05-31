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

	std::vector<char*> memoryBlocks;
	while (true)
	{
		const size_t blockSize = 1024 * 1024;
		char* block = new char[blockSize];
		std::fill(block, block + blockSize, 0);
		memoryBlocks.push_back(block);
		Sleep(1000);
	}

	return 0;
}
