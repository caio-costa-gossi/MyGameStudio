#pragma once
#include "Counter.h"
#include "Err.h"
#include "Timeline.h"

class MasterLoopManager
{
private:
	static bool loopRunning_;
	static bool debug_;
	static Timeline mainGameTimeline_;

	static Counter debugFrameCounter_;

	static Err UpdateGame();

public:
	static Err Startup(int argc, char** argv);
	static Err Run();
	static Err Shutdown();

	static Err Stop();
};
