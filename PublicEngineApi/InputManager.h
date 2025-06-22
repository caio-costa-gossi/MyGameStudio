#pragma once
#include "EventDispatcher.h"
#include <Windows.h>
#include "InputLayer.h"
#include "InputState.h"

class InputManager
{
private:
	static InputLayer* inputLayer_;

public:
	static Err Startup(HWND hWindow, int32_t deadzone = 500, bool usingSdl = true);
	static Err Update(const SDL_Event* eventList = nullptr, uint32_t numEvent = 0);
	static Err Shutdown();

	static Err SubForInputEvent(const Subscription& sub);
	static const InputState& GetInputState();
};
