#include "CsvParser.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "HashTable.h"
#include "StringId.h"
#include <windows.h>

int main()
{
	SetConsoleOutputCP(CP_UTF8);

	/*Err startup = EngineCoreManager::Startup();

	if (startup.Code() != 0)
	{
		std::cout << "Error: " << startup;
	}*/

	CsvParser* parser = new CsvParser("D:\\Documents\\Programs\\MyGameStudio\\MyGameStudio\\MyGameStudio\\x64\\Debug\\test.csv", 3);

	std::cout << parser->GetValue("en-us", 0);
}
