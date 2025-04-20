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
