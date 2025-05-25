#include "AssetDatabase.h"
#include "AssetPipeline.h"
#include "AssetRuntimeManager.h"
#include "ConsoleManager.h"
#include "DataStream.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "ImageProcessor.h"
#include "MeshProcessor.h"
#include "SystemPathHelper.h"
#include "Timeline.h"

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

	ConsoleManager::RunConsole();

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}
}
