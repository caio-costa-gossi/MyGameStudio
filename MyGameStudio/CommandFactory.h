#pragma once
#include <unordered_map>

#include "Command.h"
#include "Err.h"

class CommandFactory
{
private:
	static std::unordered_map<std::string, Command*> commandMap_;

public:
	static Err Startup();
	static Err Shutdown();
	static Command& CreateCommand(const char* command);
};
