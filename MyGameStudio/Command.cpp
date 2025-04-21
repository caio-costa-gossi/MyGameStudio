#include "Command.h"

bool Command::ValidateArgs(const uint8_t argc, char** argn, char** argv, const uint8_t expectedArgc, int intArgs[], float floatArgs[])
{
	if (argc != expectedArgc) return false;

	for (uint8_t i = 0; i < expectedArgc; ++i)
	{
		if (argn[i] == nullptr || argv[i] == nullptr) return false;

		if (intArgs != nullptr && intArgs[i] == 1)
		{
			try
			{
				intArgs[i] = std::stoi(argv[i]);
			}
			catch (std::exception& e)
			{
				return false;
			}
		}

		if (floatArgs != nullptr && floatArgs[i] == 1.0f)
		{
			try
			{
				floatArgs[i] = std::stof(argv[i]);
			}
			catch (std::exception& e)
			{
				return false;
			}
		}
	}

	return true;
}

void Command::PrintUsage(const char* commandName)
{
	const std::string message =
		static_cast<std::string>(LocalizationManager::GetLocalizedString(string_const::G_INVALID_PARAMETER)) + 
		"\n" + static_cast<std::string>(LocalizationManager::GetLocalizedString(commandName)) + "\n";

	std::cout << AsciiDrawer::DrawBox(25, message.c_str());
}

const char* Command::GetArg(const uint8_t argc, char** argn, char** argv, const char* argName)
{
	for (uint8_t i = 0; i < argc; ++i)
	{
		if (std::strcmp(argn[i], argName) == 0)
			return argv[i];
	}

	return nullptr;
}
