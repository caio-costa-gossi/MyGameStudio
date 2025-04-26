#include "ConsoleManager.h"

#include <windows.h>

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
		LTrimCommand();
		if (*usableCommand_ == '\0') continue;
		ParseCommandAlternative();

		CommandFactory::CreateCommand(mainCommand_).ExecuteCommand(argc_, argn_, argv_);
	}
}

void ConsoleManager::StopConsole()
{
	consoleRunning_ = false;
}

void ConsoleManager::Print(const std::string& message, const enums::ConsoleMessageType type)
{
	switch (type)
	{
	case enums::ConsoleMessageType::info:
		SetConsoleTextAttribute(hWinConsole_, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "[INFO] ";
		SetConsoleTextAttribute(hWinConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;

	case enums::ConsoleMessageType::warning:
		SetConsoleTextAttribute(hWinConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "[WARNING] ";
		SetConsoleTextAttribute(hWinConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;

	case enums::ConsoleMessageType::error:
		SetConsoleTextAttribute(hWinConsole_, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "[ERROR] ";
		SetConsoleTextAttribute(hWinConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;

	case enums::ConsoleMessageType::critical_error:
		SetConsoleTextAttribute(hWinConsole_, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "[CRITICAL ERROR] ";
		SetConsoleTextAttribute(hWinConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}

	std::cout << message << "\n";
}

void ConsoleManager::AllocMem()
{
	argv_ = new char* [16];
	argn_ = new char* [16];
	fullCommand_ = new char[2048];
}

void ConsoleManager::DeleteMem()
{
	delete argv_;
	delete argn_;
	delete fullCommand_;
}


void ConsoleManager::ReceiveCommand()
{
	std::cout << "> ";
	std::cin.getline(fullCommand_, 2048);
}

void ConsoleManager::LTrimCommand()
{
	usableCommand_ = fullCommand_;

	while (*usableCommand_ && std::isspace(static_cast<unsigned char>(*usableCommand_))) {
		++usableCommand_;
	}
}

void ConsoleManager::ParseCommand()
{
	uint8_t argc = 0;

	char* context = nullptr;
	char* token = strtok_s(usableCommand_, " ", &context);

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

void ConsoleManager::ParseCommandAlternative()
{
	uint8_t argc = 0;

	bool readingMainCommand = true;
	bool readingArgs = false;
	bool middleOfArg = false;
	bool nameOfArg = false;
	bool inQuotes = false;

	mainCommand_ = usableCommand_;

	// Deal with quoted command
	if (*usableCommand_ == '"')
	{
		mainCommand_ = &usableCommand_[1];
	}

	for (uint16_t finder = 0; usableCommand_[finder] != '\0'; ++finder)
	{
		if (usableCommand_[finder] == '"')
		{
			inQuotes = !inQuotes;

			// Deal with quoted arg value (the quote would be considered as part of argv otherwise)
			if (readingArgs && !nameOfArg && inQuotes)
			{
				argv_[argc] = &usableCommand_[finder + 1];
			}

			continue;
		}

		if (usableCommand_[finder] == ' ' && !inQuotes && readingMainCommand)
		{
			readingMainCommand = false;
			readingArgs = true;
		}
		else if (readingArgs && middleOfArg && usableCommand_[finder] == ' ' && !inQuotes)
		{
			argc++;
			middleOfArg = false;
		}
		else if (readingArgs && !middleOfArg && usableCommand_[finder] != ' ' && usableCommand_[finder] != '"')
		{
			nameOfArg = true;
			middleOfArg = true;
			argn_[argc] = &usableCommand_[finder];
		}
		else if (readingArgs && nameOfArg && usableCommand_[finder] == '=' && !inQuotes)
		{
			nameOfArg = false;
			argv_[argc] = &usableCommand_[finder + 1];
		}
	}

	const uint16_t commandLen = std::strlen(usableCommand_);
	ReplaceWithTerminator(usableCommand_, commandLen);

	argc_ = argc;
	if (middleOfArg) argc_++;
}

void ConsoleManager::ReplaceWithTerminator(char* string, const uint16_t charCount)
{
	bool inQuotes = false;

	for (uint16_t i = 0; i < charCount; i++)
	{
		if (string[i] == '"')
		{
			inQuotes = !inQuotes;
			string[i] = '\0';
		}

		if ((string[i] == ' ' || string[i] == '=') && !inQuotes)
			string[i] = '\0';
	}
}


HANDLE ConsoleManager::hWinConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);

bool ConsoleManager::consoleRunning_ = true;
char* ConsoleManager::fullCommand_;
char* ConsoleManager::mainCommand_;
char* ConsoleManager::usableCommand_;
char** ConsoleManager::argv_;
char** ConsoleManager::argn_;
uint8_t ConsoleManager::argc_;