#pragma once
#include "Err.h"

class GameBuilder
{
private:
	static std::string workingDir_;
	static std::string cmakePath_;
	static std::string srcDir_;
	static std::string buildDir_;
	static std::string projectName_;

	static std::string verifyCmakeCmd_;
	static std::string prepareBuildCmd_;
	static std::string buildCmd_;
	static std::string runCmd_;

	static bool IsCmakePresent();

public:
	static Err Configure();
	static Err BuildGame();
	static Err RunGame();
};
