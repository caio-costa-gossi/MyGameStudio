#pragma once
#include "EventDispatcher.h"

class PhysicsManager
{
private:
	static EventDispatcher dispatcher_;

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err SubBeforePhysics(const Subscription& sub);
};