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

	//ConsoleManager::RunConsole();

	uint64_t counter = 0;
	Timeline time(timeline::SECOND);
	time.Start();
	while (true)
	{
		const uint64_t elapsed = time.GetElapsed();

		std::cout << "Time elapsed: " << elapsed << "\n";

		char input;
		std::cin >> input;

		if (input == 'q')
			break;

		if (input == 'p')
		{
			Err err = time.Pause();

			if (err.Code())
				ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
			else
				std::cout << "Paused!" << "\n";
		}

		if (input == 'r')
		{
			Err err = time.Resume();

			if (err.Code())
				ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
			else
				std::cout << "Resumed!" << "\n";
		}

		if (input == 's')
		{
			Err err = time.Reset();

			if (err.Code())
				ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
			else
				std::cout << "Reset!" << "\n";
		}

		if (input == 'd')
		{
			Err error = time.UpdateLastTime();
			if (error.Code())
				ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);

			const uint64_t delta = time.GetDelta();

			std::cout << "TimeDelta: " << delta << "\n";
		}

		counter++;
	}

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
	}
}
