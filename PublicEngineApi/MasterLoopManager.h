#pragma once
#include "Err.h"
#include "Timeline.h"

class MasterLoopManager
{
private:
	static bool loopRunning_;
	static Timeline mainGameTimeline_;

	static Err UpdateGame();

public:
	static Err Startup();
	static Err Run();
	static Err Shutdown();

	static Err Stop();
};
