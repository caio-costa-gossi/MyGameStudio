#include "EngineCoreManager.h"
#include "Err.h"
#include "HashTable.h"

int main()
{
	/*Err startup = EngineCoreManager::Startup();

	if (startup.Code() != 0)
	{
		std::cout << "Error: " << startup;
	}*/

	auto myHashTable = containers::HashTable<std::string>();

	myHashTable["key1"] = "Appasdasdsales";
	myHashTable["key2"] = "Oranges";

	std::string s = myHashTable["key2"];

	std::cout << s;
}
