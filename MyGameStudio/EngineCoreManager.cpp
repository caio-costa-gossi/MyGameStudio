#include "EngineCoreManager.h"
#include <windows.h>

#include "ConfigManager.h"
#include "LocalizationManager.h"

Err EngineCoreManager::Startup()
{
	// Start time profiling
	engineBeginTime_ = std::chrono::high_resolution_clock::now();

	std::cout << "Initializing systems...\n";

	// Subsystem startup
	Err err = LocalizationManager::Startup("D:\\Documents\\Programs\\MyGameStudio\\MyGameStudio\\MyGameStudio\\x64\\Debug\\test.csv", "string_id");
	if (err.Code() != 0)
		std::cout << "Error! " << err.Message();

	err = ConfigManager::Startup("D:\\Documents\\Programs\\MyGameStudio\\MyGameStudio\\MyGameStudio\\x64\\Debug\\config.ini");
	if (err.Code() != 0)
		std::cout << "Error! " << err.Message();

	std::cout << "All systems ready!\n\n";
	return error_const::SUCCESS;
}

Err EngineCoreManager::Config()
{
	SetConsoleOutputCP(CP_UTF8);

	const char* userLanguage = ConfigManager::GetConfigForObject("user", "language");
	if (userLanguage == nullptr) userLanguage = "en_us";
	LocalizationManager::SetLanguage(enums::StringToLanguage(userLanguage));

	return error_const::SUCCESS;
}

Err EngineCoreManager::Shutdown()
{
	// Finish time profiling
	engineFinishTime_ = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = engineFinishTime_ - engineBeginTime_;

	std::cout << "\nShutting down...\n";
	std::cout << "Total time spent: " << duration.count() << " seconds.\n";

	return error_const::SUCCESS;
}

std::chrono::time_point<std::chrono::steady_clock> EngineCoreManager::engineBeginTime_;
std::chrono::time_point<std::chrono::steady_clock> EngineCoreManager::engineFinishTime_;