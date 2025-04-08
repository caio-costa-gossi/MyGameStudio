#include "CsvParser.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "HashTable.h"
#include "StringId.h"
#include <windows.h>

#include "LocalizationManager.h"

int main()
{
	SetConsoleOutputCP(CP_UTF8);

	Err startup = EngineCoreManager::Startup();

	if (startup.Code() != 0)
	{
		std::cout << "Error: " << startup;
	}

	const char* string = LocalizationManager::GetLocalizedString("fala_3", "fr-fr");

	if (string != nullptr)
		std::cout << string;
}
