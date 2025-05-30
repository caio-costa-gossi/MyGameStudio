#pragma once
#include "Err.h"

class GameDebugger
{
private:


public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();
};
