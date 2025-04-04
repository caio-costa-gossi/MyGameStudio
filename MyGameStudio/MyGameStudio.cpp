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

	std::cout << "Hello world!"_sid.stringText.data() << " " << "Hello world!"_sid.stringHash << std::endl;
}
