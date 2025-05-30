#pragma once
#include "Err.h"

class MasterLoopManager
{
private:
	static bool loopRunning_;

	static Err RunGame();

public:
	static Err Startup();
	static Err Run();
	static Err Shutdown();

	static Err Stop();
};
