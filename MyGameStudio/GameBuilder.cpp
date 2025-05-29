#include "GameBuilder.h"
#include <filesystem>
#include <sstream>

#include "ConfigManager.h"
#include "ConsoleManager.h"

Err GameBuilder::Configure()
{
	workingDir_ = std::filesystem::current_path().string();
	cmakePath_ = workingDir_ + ConfigManager::GetConfig("cmake_path");
	srcDir_ = workingDir_ + ConfigManager::GetConfig("game_source_dir");
	buildDir_ = workingDir_ + ConfigManager::GetConfig("game_build_dir");

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
	// Create CMakeLists.txt
	ConsoleManager::Print("Creating CMakeLists.txt...", enums::ConsoleMessageType::info);
	Err err = CreateCMakeLists();
	if (err.Code())
		return err;

	// Call cmake.exe
	ConsoleManager::Print("Preparing build...", enums::ConsoleMessageType::info);
	ConsoleManager::Print("Executing '" + prepareBuildCmd_ + "'...", enums::ConsoleMessageType::info);
	int returnCode = std::system(prepareBuildCmd_.c_str());

	if (returnCode)
		return error_const::COMMAND_UNSUCCESSFUL;

	// Call cmake build
	ConsoleManager::Print("Building...", enums::ConsoleMessageType::info);
	ConsoleManager::Print("Executing '" + buildCmd_ + "'...", enums::ConsoleMessageType::info);
	returnCode = std::system(buildCmd_.c_str());

	if (returnCode)
		return error_const::COMMAND_UNSUCCESSFUL;

	ConsoleManager::Print("Build was successful.", enums::ConsoleMessageType::info);
	return error_const::SUCCESS;
}

Err GameBuilder::RunGame()
{
	ConsoleManager::Print("Running...", enums::ConsoleMessageType::info);

	std::string possibleExePaths[] = { "\\bin\\", "\\bin\\Debug\\", "\\bin\\Release\\" };

	for (int i = 0; i < 2; ++i)
	{
		runCmd_ = "\"" + buildDir_ + possibleExePaths[i] + projectName_ + ".exe\"";

		ConsoleManager::Print("Executing '" + runCmd_ + "'...", enums::ConsoleMessageType::info);
		int returnCode = std::system(runCmd_.c_str());

		if (!returnCode)
			break;

		// Last possible path
		if (i == 2)
			ConsoleManager::Print("Game binary not found...", enums::ConsoleMessageType::error);
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
	ReplaceInString(templateText, "@project_name", projectName_);
	ReplaceInString(templateText, "@version", projectVersion_);
	ReplaceInString(templateText, "@cpp_standard", cppStandard_);

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

std::string GameBuilder::projectName_;
std::string GameBuilder::projectVersion_;
std::string GameBuilder::cppStandard_;

std::string GameBuilder::verifyCmakeCmd_;
std::string GameBuilder::prepareBuildCmd_;
std::string GameBuilder::buildCmd_;
std::string GameBuilder::runCmd_;