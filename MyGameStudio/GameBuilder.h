#pragma once
#include "Err.h"
#include "GameDebugger.h"

class GameBuilder
{
private:
	static std::string workingDir_;
	static std::string cmakePath_;
	static std::string srcDir_;
	static std::string buildDir_;
	static std::string includeDir_;
	static std::string libDir_;

	static std::string projectName_;
	static std::string projectVersion_;
	static std::string cppStandard_;

	static std::string verifyCmakeCmd_;
	static std::string prepareBuildCmd_;
	static std::string buildCmd_;
	static std::string runCmd_;

	static bool IsCmakePresent();
	static Err CreateCMakeLists();
	static Err ReplaceInString(std::string& string, const std::string& lookFor, const std::string& replaceFor);

public:
	static Err Configure();
	static Err BuildGame();
	static Err RunGame(enums::GameDebugType debugType, enums::ConsoleMessageType minSeverity, uint32_t activeChannelMask);
};
