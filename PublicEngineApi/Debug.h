#pragma once
#include <string>

class Debug
{
public:
	static void Log(const char* message);
	static void Log(const std::string& message);
	static void LogError(const char* message);
	static void LogError(const std::string& message);
};
