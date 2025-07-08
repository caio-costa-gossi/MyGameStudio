#pragma once
#include <string>
#include <Windows.h>

#include "Enums.h"
#include "Err.h"


class GameConsoleManager
{
private:
	static HANDLE hConsole_;
	static uint32_t channelMask_;
	static enums::ConsoleMessageType minLevel_;

	static Err ParseArgs(const char* minLevelArg, const char* channelMaskArg, enums::ConsoleMessageType& minLevel, uint32_t& disableChannelMask);

public:
	static Err Startup(const char* minLevelArg, const char* disableChannelArg);

	static void PrintInfo(const std::string& msg, enums::ConsoleMessageSender sender = enums::ConsoleMessageSender::undefined);
	static void PrintWarning(const std::string& msg, enums::ConsoleMessageSender sender = enums::ConsoleMessageSender::undefined);
	static void PrintError(const std::string& msg, enums::ConsoleMessageSender sender = enums::ConsoleMessageSender::undefined);
	static void PrintError(const Err& err, enums::ConsoleMessageSender sender = enums::ConsoleMessageSender::undefined);
	static void PrintCritical(const std::string& msg, enums::ConsoleMessageSender sender = enums::ConsoleMessageSender::undefined);
	static void PrintSimple(const std::string& msg, enums::ConsoleMessageSender sender = enums::ConsoleMessageSender::undefined);
};
