#pragma once
#include "EventDispatcher.h"
#include <Windows.h>

class InputManager
{
private:
	static EventDispatcher dispatcher_;

public:
	static Err Startup(HWND hWindow);
	static Err Update();
	static Err Shutdown();

	static Err SubBeforeInput(const Subscription& sub);
};
