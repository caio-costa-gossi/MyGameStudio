#pragma once
#include <string>
#include <Windows.h>
#include "Err.h"

class GameConsoleManager
{
private:
	static HANDLE hConsole_;

public:
	static Err Startup();

	static void PrintInfo(const std::string& msg);
	static void PrintWarning(const std::string& msg);
	static void PrintError(const std::string& msg);
	static void PrintError(const Err& err);
	static void PrintCritical(const std::string& msg);
	static void PrintSimple(const std::string& msg);
};
