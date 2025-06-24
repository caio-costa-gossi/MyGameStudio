#include "GameBuilder.h"
#include <filesystem>
#include <sstream>

#include "ConfigManager.h"
#include "ConsoleManager.h"
#include "GameRuntimeTestManager.h"
#include "LocalizationManager.h"
#include "SystemPathHelper.h"
#include "SystemsInfoHelper.h"
#include "TerminalFactory.h"

Err GameBuilder::Configure()
{
	workingDir_ = std::filesystem::current_path().string();
	cmakePath_ = workingDir_ + ConfigManager::GetConfig("cmake_path");
	srcDir_ = workingDir_ + ConfigManager::GetConfig("game_source_dir");
	buildDir_ = workingDir_ + ConfigManager::GetConfig("game_build_dir");
	includeDir_ = workingDir_ + ConfigManager::GetConfig("include_dir");
	libDir_ = workingDir_ + ConfigManager::GetConfig("lib_dir");

	projectName_ = ConfigManager::GetConfig("game_name");
	projectVersion_ = ConfigManager::GetConfig("game_version");
	cppStandard_ = ConfigManager::GetConfig("cpp_standard");

	verifyCmakeCmd_ = cmakePath_ + " --version";
	prepareBuildCmd_ = "\"" + cmakePath_ + "\" -S " + srcDir_ + " -B " + buildDir_;
	buildCmd_ = "\"" + cmakePath_ + "\" --build " + buildDir_;

	return error_const::SUCCESS;
}

Err GameBuilder::BuildGame()
{
	// Check if cmake.exe is present
	if (!IsCmakePresent())
		return error_const::CMAKE_NOT_PRESENT;

	// Create CMakeLists.txt
	ConsoleManager::PrintInfo("Creating CMakeLists.txt...");
	Err err = CreateCMakeLists();
	if (err.Code())
		return err;

	// Call cmake.exe
	ConsoleManager::PrintInfo(LocalizationManager::GetLocalizedString(string_const::G_PREPARE_BUILD_GAME));
	ConsoleManager::PrintInfo("Executing '" + prepareBuildCmd_ + "'...");
	int returnCode = std::system(prepareBuildCmd_.c_str());

	if (returnCode)
		return error_const::COMMAND_UNSUCCESSFUL;

	// Call cmake build
	ConsoleManager::PrintInfo(LocalizationManager::GetLocalizedString(string_const::G_BUILD_GAME));
	ConsoleManager::PrintInfo("Executing '" + buildCmd_ + "'...");
	returnCode = std::system(buildCmd_.c_str());

	if (returnCode)
		return error_const::COMMAND_UNSUCCESSFUL;

	ConsoleManager::PrintInfo(LocalizationManager::GetLocalizedString(string_const::G_BUILD_SUCCESSFUL));
	return error_const::SUCCESS;
}

Err GameBuilder::RunGame(const enums::GameDebugType debugType)
{
	// Check if game is running already
	if (GameRuntimeTestManager::IsGameRunning())
		return error_const::GAME_ALREADY_RUNNING;

	ConsoleManager::PrintInfo(LocalizationManager::GetLocalizedString(string_const::G_RUN_GAME));

	std::string possibleExePaths[] = { "\\bin\\", "\\bin\\Debug\\", "\\bin\\Release\\" };

	const uint64_t scrX = SystemsInfoHelper::GetScreenWidth();
	const uint64_t scrY = SystemsInfoHelper::GetScreenHeight();

	for (int i = 0; i < 2; ++i)
	{
		runCmd_ = "\"" + buildDir_ + possibleExePaths[i] + projectName_ + ".exe\"";
		if (debugType != enums::no_debug_from_child)
		{
			runCmd_ += " " + std::to_string(debugType) + " DebugPipe";
		}

		ConsoleManager::PrintInfo("Executing '" + runCmd_ + "'...");
		const CreateTerminalInfo info = { runCmd_, "Game Terminal", scrX / 2, scrY / 2, scrX / 2, scrY / 2 };
		PROCESS_INFORMATION gameProcess = TerminalFactory::CreateTerminal(info, false).ProcessInfo;

		if (gameProcess.hProcess != nullptr)
		{
			GameRuntimeTestManager::SetGameRunning(gameProcess);
			break;
		}

		// Last possible path
		if (i == 2)
			return error_const::GAME_BIN_NOT_FOUND;
	}

	return error_const::SUCCESS;
}

Err GameBuilder::CreateCMakeLists()
{
	std::ifstream fileTemplate(srcDir_ + "\\CMakeTemplate.txt");
	std::ofstream targetFile(srcDir_ + "\\CMakeLists.txt", std::ios::trunc);

	std::stringstream buffer;
	buffer << fileTemplate.rdbuf();
	std::string templateText = buffer.str();

	if (templateText.empty())
		return error_const::CMAKE_TEMPLATE_NOT_FOUND;

	ReplaceInString(templateText, "@project_name", projectName_);
	ReplaceInString(templateText, "@version", projectVersion_);
	ReplaceInString(templateText, "@cpp_standard", cppStandard_);
	ReplaceInString(templateText, "@include_dir", includeDir_);
	ReplaceInString(templateText, "@lib_dir", libDir_);
	SystemPathHelper::WinSeparatorToUnix(templateText);

	targetFile << templateText;

	fileTemplate.close();
	targetFile.close();

	return error_const::SUCCESS;
}

Err GameBuilder::ReplaceInString(std::string& string, const std::string& lookFor, const std::string& replaceFor)
{
	if (string.empty())
		return error_const::SUCCESS;

	size_t startPos = 0;
	while ((startPos = string.find(lookFor, startPos)) != std::string::npos)
	{
		string.replace(startPos, lookFor.length(), replaceFor);
		startPos += replaceFor.length();
	}

	return error_const::SUCCESS;
}

bool GameBuilder::IsCmakePresent()
{
	return std::system(verifyCmakeCmd_.c_str()) == 0;
}

std::string GameBuilder::workingDir_;
std::string GameBuilder::cmakePath_;
std::string GameBuilder::srcDir_;
std::string GameBuilder::buildDir_;
std::string GameBuilder::includeDir_;
std::string GameBuilder::libDir_;

std::string GameBuilder::projectName_;
std::string GameBuilder::projectVersion_;
std::string GameBuilder::cppStandard_;

std::string GameBuilder::verifyCmakeCmd_;
std::string GameBuilder::prepareBuildCmd_;
std::string GameBuilder::buildCmd_;
std::string GameBuilder::runCmd_;