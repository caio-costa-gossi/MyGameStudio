#include "EngineCoreManager.h"
#include <windows.h>

#include "ConfigManager.h"
#include "ConsoleManager.h"
#include "FileManager.h"
#include "LocalizationManager.h"

Err EngineCoreManager::Startup()
{
	// Start time profiling
	engineBeginTime_ = std::chrono::high_resolution_clock::now();

	std::cout << "Initializing systems...\n";

	// Subsystem startup
	Err err = LocalizationManager::Startup("strings.csv", "string_id");
	if (err.Code() != 0)
		std::cout << "Error! " << err.Message();

	err = ConfigManager::Startup("config.ini");
	if (err.Code() != 0)
		std::cout << "Error! " << err.Message();

	err = FileManager::Startup();
	if (err.Code() != 0)
		std::cout << "Error! " << err.Message();

	err = ConsoleManager::Startup();
	if (err.Code() != 0)
		std::cout << "Error! " << err.Message();

	std::cout << "All systems ready!\n\n";
	return error_const::SUCCESS;
}

Err EngineCoreManager::Config()
{
	std::cout << "Configuring language...\n";

	SetConsoleOutputCP(CP_UTF8);

	const char* userLanguage = ConfigManager::GetConfigForObject("user", "language");
	if (userLanguage == nullptr) userLanguage = "en_us";
	LocalizationManager::SetLanguage(enums::StringToLanguage(userLanguage));

	std::cout << LocalizationManager::GetLocalizedString(string_const::G_LANG_SELECTED) << userLanguage << "\n";

	const char* sysVer = ConfigManager::GetConfigForObject("global", "version");
	if (sysVer == nullptr) sysVer = "Not found";
	std::cout << LocalizationManager::GetLocalizedString(string_const::G_STARTUP_MSG) << sysVer << "\n";

	return error_const::SUCCESS;
}

Err EngineCoreManager::Shutdown()
{
	// Finish time profiling
	engineFinishTime_ = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = engineFinishTime_ - engineBeginTime_;

	std::cout << "\n" << LocalizationManager::GetLocalizedString(string_const::G_SYS_SHUTDOWN) << "\n";

	// Shutdown systems
	FileManager::Shutdown();
	ConsoleManager::Shutdown();

	std::cout << LocalizationManager::GetLocalizedString(string_const::G_TOTAL_RUN_TIME) << duration.count() << "s.\n";

	return error_const::SUCCESS;
}

std::chrono::time_point<std::chrono::steady_clock> EngineCoreManager::engineBeginTime_;
std::chrono::time_point<std::chrono::steady_clock> EngineCoreManager::engineFinishTime_;