#include "EngineCoreManager.h"

Err EngineCoreManager::Startup()
{
	Err testManagerErr = testManager_.Startup();
	if (testManagerErr.Code() != 0)
		std::cout << "Error: " << testManagerErr;

	// Startup other systems

	std::cout << "All systems ready!" << std::endl;
	return error_const::SUCCESS;
}

TestManager EngineCoreManager::testManager_ = TestManager();