#include "EngineCoreManager.h"
#include "LocalizationManager.h"

Err EngineCoreManager::Startup()
{
	// Subsystem startup
	Err err = LocalizationManager::Startup("D:\\Documents\\Programs\\MyGameStudio\\MyGameStudio\\MyGameStudio\\x64\\Debug\\mock1.csv", "string_id");
	if (err.Code() != 0)
		std::cout << "Error! " << err.Message();

	std::cout << "All systems ready!\n\n";
	return error_const::SUCCESS;
}