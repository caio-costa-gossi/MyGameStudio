#pragma once
#include "Err.h"

class TestDrawer
{
private:
	static bool running_;

public:
	static Err Startup();
	static Err Run();
	static Err Shutdown();
};
