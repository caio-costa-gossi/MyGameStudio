#pragma once
#include <unordered_map>

#include "AsciiDrawer.h"
#include "ConsoleManager.h"
#include "Err.h"
#include "LocalizationManager.h"

class Command
{
protected:
	static bool ValidateArgs(uint8_t argc, char** argn, char** argv, uint8_t expectedArgc, int intArgs[] = nullptr, float floatArgs[] = nullptr);

public:
	virtual Err ExecuteCommand(uint8_t argc, char** argn, char** argv) = 0;
	virtual ~Command() = default;
};

class NotFoundCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		std::cout << "\nCommand not found...\n";
		return error_const::SUCCESS;
	}

	~NotFoundCommand() override = default;
};

class HelpCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		//ValidateArgs(argc, argn, argv, 1);

		std::cout << AsciiDrawer::DrawBox(20, LocalizationManager::GetLocalizedString(string_const::G_HELP_COMMAND));
		return error_const::SUCCESS;
	}

	~HelpCommand() override = default;
};

class QuitCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		std::cout << "\n" << LocalizationManager::GetLocalizedString(string_const::G_QUIT_CONSOLE) << "\n";
		ConsoleManager::StopConsole();
		return error_const::SUCCESS;
	}

	~QuitCommand() override = default;
};