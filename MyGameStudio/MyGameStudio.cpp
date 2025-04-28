#include "AssetPipeline.h"
#include "ConsoleManager.h"
#include "EngineCoreManager.h"
#include "Err.h"

int main()
{
	Err err = EngineCoreManager::Startup();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}

	err = EngineCoreManager::Config();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}

	//ConsoleManager::RunConsole();
	uint8_t* buffer = new uint8_t[128];
	AssetPipeline::GetAssetMetadata("assets/test.txt", buffer, 128);
	delete[] buffer;

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}
}
