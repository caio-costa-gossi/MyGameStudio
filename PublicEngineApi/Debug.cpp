#include "Debug.h"
#include "GameConsoleManager.h"

void Debug::Log(const char* message)
{
	GameConsoleManager::PrintInfo(message, enums::ConsoleMessageSender::game);
}

void Debug::LogError(const char* message)
{
	GameConsoleManager::PrintError(message, enums::ConsoleMessageSender::game);
}

void Debug::Log(const std::string& message)
{
	GameConsoleManager::PrintInfo(message, enums::ConsoleMessageSender::game);
}

void Debug::LogError(const std::string& message)
{
	GameConsoleManager::PrintError(message, enums::ConsoleMessageSender::game);
}