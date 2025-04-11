#include "EngineCoreManager.h"
#include "Err.h"
#include <windows.h>
#include "LocalizationManager.h"

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	LocalizationManager::SetLanguage(enums::Language::pt_br);

	Err startup = EngineCoreManager::Startup();
	if (startup.Code() != 0)
	{
		std::cout << "Error: " << startup;
	}

	EngineCoreManager::Shutdown();
}
