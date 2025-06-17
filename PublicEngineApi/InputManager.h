#pragma once
#include "EventDispatcher.h"
#include <Windows.h>
#include "InputLayer.h"
#include "InputState.h"

class InputManager
{
private:
	static InputLayer* inputLayer_;

	static EventDispatcher dispatcher_;

	static InputState inputState_;

public:
	static Err Startup(HWND hWindow, bool usingSdl = true);
	static Err Update();
	static Err Shutdown();

	static Err SubBeforeInput(const Subscription& sub);
};
