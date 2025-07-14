#include "GameConsoleManager.h"
#include <iostream>
#include "NumericUtils.h"

Err GameConsoleManager::Startup(const char* minLevelArg, const char* disableChannelArg)
{
	hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
	ParseArgs(minLevelArg, disableChannelArg, minLevel_, channelMask_);

	PrintInfo("Game console manager started with verbosity = " + std::to_string(static_cast<uint8_t>(minLevel_)) + " and channelMask = " + std::to_string(channelMask_), enums::ConsoleMessageSender::console);

	return error_const::SUCCESS;
}

void GameConsoleManager::PrintInfo(const std::string& msg, const enums::ConsoleMessageSender sender)
{
	if (minLevel_ > enums::ConsoleMessageType::info)
		return;

	if ((channelMask_ & 1 << static_cast<uint8_t>(sender)) != 0)
		return;

	SetConsoleTextAttribute(hConsole_, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "[INFO]";
	std::cout << "[" + std::string(enums::MessageSenderToString(sender)) + "] ";
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << msg << "\n";
}

void GameConsoleManager::PrintWarning(const std::string& msg, const enums::ConsoleMessageSender sender)
{
	if (minLevel_ > enums::ConsoleMessageType::warning)
		return;

	if ((channelMask_ & 1 << static_cast<uint8_t>(sender)) != 0)
		return;

	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "[WARNING]";
	std::cout << "[" + std::string(enums::MessageSenderToString(sender)) + "] ";
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << msg << "\n";
}

void GameConsoleManager::PrintError(const std::string& msg, const enums::ConsoleMessageSender sender)
{
	if (minLevel_ > enums::ConsoleMessageType::error)
		return;

	if ((channelMask_ & 1 << static_cast<uint8_t>(sender)) != 0)
		return;

	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "[ERROR]";
	std::cout << "[" + std::string(enums::MessageSenderToString(sender)) + "] ";
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << msg << "\n";
}

void GameConsoleManager::PrintError(const Err& err, const enums::ConsoleMessageSender sender)
{
	if (minLevel_ > enums::ConsoleMessageType::error)
		return;

	if ((channelMask_ & 1 << static_cast<uint8_t>(sender)) != 0)
		return;

	PrintError("Error code: " + std::to_string(err.Code()) + " - '" + err.Message() + "'", sender);
}

void GameConsoleManager::PrintCritical(const std::string& msg, const enums::ConsoleMessageSender sender)
{
	if (minLevel_ > enums::ConsoleMessageType::critical_error)
		return;

	if ((channelMask_ & 1 << static_cast<uint8_t>(sender)) != 0)
		return;

	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "[CRITICAL ERROR]";
	std::cout << "[" + std::string(enums::MessageSenderToString(sender)) + "] ";
	SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << msg << "\n";
}

void GameConsoleManager::PrintSimple(const std::string& msg, const enums::ConsoleMessageSender sender)
{
	std::cout << msg << "\n";
}

Err GameConsoleManager::ParseArgs(const char* minLevelArg, const char* channelMaskArg, enums::ConsoleMessageType& minLevel, uint32_t& disableChannelMask)
{
	uint8_t minLevelInt;
	Err err = NumericUtils::StringToUInt8(minLevelArg, minLevelInt);
	if (err.Code())
		return err;

	minLevel = static_cast<enums::ConsoleMessageType>(minLevelInt);

	err = NumericUtils::StringToUInt32(channelMaskArg, disableChannelMask);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}


HANDLE GameConsoleManager::hConsole_;
enums::ConsoleMessageType GameConsoleManager::minLevel_ = enums::ConsoleMessageType::info;
uint32_t GameConsoleManager::channelMask_ = 0;