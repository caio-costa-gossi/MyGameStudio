#include "GameConsoleManager.h"
#include <iostream>

Err GameConsoleManager::Startup()
{
	hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
	return error_const::SUCCESS;
}

void GameConsoleManager::PrintInfo(const std::string& msg)
{
	SetConsoleTextAttribute(hConsole_, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "[INFO] ";
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << msg << "\n";
}

void GameConsoleManager::PrintWarning(const std::string& msg)
{
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "[WARNING] ";
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << msg << "\n";
}

void GameConsoleManager::PrintError(const std::string& msg)
{
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "[ERROR] ";
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << msg << "\n";
}

void GameConsoleManager::PrintError(const Err& err)
{
	PrintError("Error code: " + std::to_string(err.Code()) + " - '" + err.Message()) + "'";
}

void GameConsoleManager::PrintCritical(const std::string& msg)
{
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "[CRITICAL ERROR] ";
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << msg << "\n";
}

void GameConsoleManager::PrintSimple(const std::string& msg)
{
	std::cout << msg << "\n";
}


HANDLE GameConsoleManager::hConsole_;