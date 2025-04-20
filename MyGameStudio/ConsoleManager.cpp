#include "ConsoleManager.h"
#include "CommandFactory.h"

Err ConsoleManager::Startup()
{
	CommandFactory::Startup();
	AllocMem();
	return error_const::SUCCESS;
}

Err ConsoleManager::Shutdown()
{
	CommandFactory::Shutdown();
	DeleteMem();
	return error_const::SUCCESS;
}


void ConsoleManager::RunConsole()
{
	while (consoleRunning_)
	{
		ReceiveCommand();
		ParseCommand();

		CommandFactory::CreateCommand(mainCommand_).ExecuteCommand(argc_, argn_, argv_);
	}
}

void ConsoleManager::StopConsole()
{
	consoleRunning_ = false;
}

void ConsoleManager::AllocMem()
{
	argv_ = new char* [16];
	argn_ = new char* [16];

	for (uint8_t i = 0; i < 16; i++)
	{
		argv_[i] = new char[64];
		argn_[i] = new char[64];
	}

	fullCommand_ = new char[2048];
}

void ConsoleManager::DeleteMem()
{
	for (uint8_t i = 0; i < 16; i++)
	{
		delete argv_[i];
		delete argn_[i];
	}

	delete argv_;
	delete argn_;
	delete fullCommand_;
}


void ConsoleManager::ReceiveCommand()
{
	std::cout << "\n> ";
	std::cin.getline(fullCommand_, 2048);
}

void ConsoleManager::ParseCommand()
{
	uint8_t argc = 0;

	char* context = nullptr;
	char* token = strtok_s(fullCommand_, " ", &context);

	mainCommand_ = token;

	while (token != nullptr)
	{
		if (argc == 0)
		{
			token = strtok_s(nullptr, " ", &context);
			argc++;
			continue;
		}

		char* argContext = nullptr;
		char* argn = strtok_s(token, "=", &argContext);
		char* argv = strtok_s(nullptr, " ", &argContext);

		argn_[argc-1] = argn;
		argv_[argc-1] = argv;

		token = strtok_s(nullptr, " ", &context);

		argc++;
	}

	argc_ = argc - 1;
}

bool ConsoleManager::consoleRunning_ = true;
char* ConsoleManager::fullCommand_;
char* ConsoleManager::mainCommand_;
char** ConsoleManager::argv_;
char** ConsoleManager::argn_;
uint8_t ConsoleManager::argc_;