#include "CommandFactory.h"

Err CommandFactory::Startup()
{
	commandMap_["help"] = new HelpCommand();
	commandMap_["quit"] = new QuitCommand();
	commandMap_["chlang"] = new ChangeLangCommand();
	commandMap_["$not_found"] = new NotFoundCommand();

	return error_const::SUCCESS;
}

Err CommandFactory::Shutdown()
{
	for (std::pair<std::string, Command*> par : commandMap_)
	{
		delete par.second;
	}

	commandMap_.clear();

	return error_const::SUCCESS;
}

Command& CommandFactory::CreateCommand(const char* command)
{
	if (commandMap_[command] == nullptr)
		return *commandMap_["$not_found"];

	return *commandMap_[command];
}

auto CommandFactory::commandMap_ = std::unordered_map<std::string, Command*>();