#pragma once
#include "EventDispatcher.h"

class AnimationManager
{
private:
	static EventDispatcher dispatcher_;

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err SubBeforeAnimation(const Subscription& sub);
};
