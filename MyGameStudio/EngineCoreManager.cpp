#include "EngineCoreManager.h"
#include <windows.h>

#include "AssetDatabase.h"
#include "AssetRuntimeManager.h"
#include "ConfigManager.h"
#include "ConsoleManager.h"
#include "FileManager.h"
#include "GameBuilder.h"
#include "LocalizationManager.h"

Err EngineCoreManager::Startup()
{
	// Start time profiling
	engineBeginTime_ = std::chrono::high_resolution_clock::now();

	ConsoleManager::Print("Initializing systems...", enums::ConsoleMessageType::info);

	// Subsystem startup
	Err err = LocalizationManager::Startup("strings.csv", "string_id");
	if (err.Code())
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

	err = ConfigManager::Startup("config.ini");
	if (err.Code())
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

	err = FileManager::Startup();
	if (err.Code())
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

	err = ConsoleManager::Startup();
	if (err.Code())
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

	err = AssetDatabase::Startup();
	if (err.Code())
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

	err = AssetRuntimeManager::Startup();
	if (err.Code())
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

	// All systems initialized
	ConsoleManager::Print("All systems ready!\n", enums::ConsoleMessageType::info);
	return error_const::SUCCESS;
}

Err EngineCoreManager::Config()
{
	// Get language
	ConsoleManager::Print("Configuring language...", enums::ConsoleMessageType::info);

	SetConsoleOutputCP(CP_UTF8);

	const char* userLanguage = ConfigManager::GetConfigForObject("user", "language");
	if (userLanguage == nullptr) userLanguage = "en_us";
	LocalizationManager::SetLanguage(enums::StringToLanguage(userLanguage));

	ConsoleManager::Print(LocalizationManager::GetLocalizedString(string_const::G_LANG_SELECTED) + std::string(userLanguage), enums::ConsoleMessageType::info);

	// Configure CMake commands
	ConsoleManager::Print("Configuring GameBuilder...", enums::ConsoleMessageType::info);
	GameBuilder::Configure();

	// Get version
	const char* sysVer = ConfigManager::GetConfigForObject("global", "version");
	if (sysVer == nullptr) sysVer = "Not found";
	ConsoleManager::Print(LocalizationManager::GetLocalizedString(string_const::G_STARTUP_MSG) + std::string(sysVer), enums::ConsoleMessageType::info);

	return error_const::SUCCESS;
}

Err EngineCoreManager::Shutdown()
{
	// Finish time profiling
	engineFinishTime_ = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = engineFinishTime_ - engineBeginTime_;

	ConsoleManager::Print(LocalizationManager::GetLocalizedString(string_const::G_SYS_SHUTDOWN), enums::ConsoleMessageType::info);

	// Shutdown systems
	FileManager::Shutdown();
	ConsoleManager::Shutdown();
	AssetDatabase::Shutdown();

	ConsoleManager::Print(LocalizationManager::GetLocalizedString(string_const::G_TOTAL_RUN_TIME) + std::to_string(duration.count()) + std::string("s.\n"), enums::ConsoleMessageType::info);

	return error_const::SUCCESS;
}

std::chrono::time_point<std::chrono::steady_clock> EngineCoreManager::engineBeginTime_;
std::chrono::time_point<std::chrono::steady_clock> EngineCoreManager::engineFinishTime_;