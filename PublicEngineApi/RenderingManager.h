#pragma once
#include "EventDispatcher.h"

class RenderingManager
{
private:
	static EventDispatcher dispatcher_;

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err SubBeforeRendering(const Subscription& sub);
};