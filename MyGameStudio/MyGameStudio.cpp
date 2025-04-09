#include "CsvParser.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "HashTable.h"
#include "StringId.h"
#include <windows.h>
#include <chrono>

#include "LocalizationManager.h"

int main()
{
	const auto beginTime = std::chrono::high_resolution_clock::now();

	SetConsoleOutputCP(CP_UTF8);
	LocalizationManager::SetLanguage(enums::Language::pt_br);

	Err startup = EngineCoreManager::Startup();

	if (startup.Code() != 0)
	{
		std::cout << "Error: " << startup;
	}

	const char* string = LocalizationManager::GetLocalizedString("fala_3");

	if (string != nullptr)
		std::cout << string << std::endl;

	const auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = endTime - beginTime;

	std::cout << "Execution time: " << duration.count() << " seconds\n";
}
