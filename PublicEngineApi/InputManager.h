#pragma once
#include "EventDispatcher.h"

class InputManager
{
private:
	static EventDispatcher dispatcher_;

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err SubBeforeInput(const Subscription& sub);
};
