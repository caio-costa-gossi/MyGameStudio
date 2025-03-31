#include "EngineCoreManager.h"
#include "Err.h"

int main()
{
	Err startup = EngineCoreManager::Startup();

	if (startup.Code() != 0)
	{
		std::cout << "Error: " << startup;
	}
}
