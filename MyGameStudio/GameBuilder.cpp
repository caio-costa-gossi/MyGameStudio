#include "GameBuilder.h"
#include <filesystem>
#include "ConsoleManager.h"

Err GameBuilder::Configure()
{
	workingDir_ = std::filesystem::current_path().string();
	cmakePath_ = workingDir_ + "\\binaries\\cmake\\bin\\cmake.exe";
	srcDir_ = workingDir_ + "\\binaries\\game";
	buildDir_ = workingDir_ + "\\binaries\\game\\build";
	projectName_ = "MyGameBinary";

	verifyCmakeCmd_ = cmakePath_ + " --version";
	prepareBuildCmd_ = "\"" + cmakePath_ + "\" -S " + srcDir_ + " -B " + buildDir_;
	buildCmd_ = "\"" + cmakePath_ + "\" --build " + buildDir_;
	runCmd_ = "\"" + buildDir_ + "\\bin\\Debug\\" + projectName_ + ".exe\"";

	return error_const::SUCCESS;
}

Err GameBuilder::BuildGame()
{
	ConsoleManager::Print("Preparing build...", enums::ConsoleMessageType::info);
	ConsoleManager::Print("Executing '" + prepareBuildCmd_ + "'...", enums::ConsoleMessageType::info);
	int returnCode = std::system(prepareBuildCmd_.c_str());

	if (returnCode)
		return error_const::COMMAND_UNSUCCESSFUL;

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
	ConsoleManager::Print("Executing '" + buildCmd_ + "'...", enums::ConsoleMessageType::info);
	int returnCode = std::system(runCmd_.c_str());

	if (returnCode)
		return error_const::COMMAND_UNSUCCESSFUL;

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
std::string GameBuilder::verifyCmakeCmd_;
std::string GameBuilder::prepareBuildCmd_;
std::string GameBuilder::buildCmd_;
std::string GameBuilder::runCmd_;