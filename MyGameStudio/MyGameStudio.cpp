#include "CsvParser.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "HashTable.h"
#include "StringId.h"

int main()
{
	/*Err startup = EngineCoreManager::Startup();

	if (startup.Code() != 0)
	{
		std::cout << "Error: " << startup;
	}*/

	CsvParser* parser = new CsvParser("D:\\Documents\\Programs\\MyGameStudio\\MyGameStudio\\MyGameStudio\\x64\\Debug\\test.csv", 3);

	const char* result = parser->GetValue("pt-br", 2);
	std::cout << parser->GetValue("pt-br", 2);
}
