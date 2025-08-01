#pragma once
#include "EventDispatcher.h"
#include <Windows.h>
#include "InputLayer.h"
#include "InputState.h"

class InputManager
{
private:
	static InputLayer* inputLayer_;
	static bool usingSdl_;
	static bool isInit_;

	static Err ParseInputArgs(const char* useSdlArg, const char* deadzoneArg, bool& useSdl, int32_t& deadzone);

public:
	static Err Startup(HWND hWindow, const char* deadzoneArg, const char* usingSdlArg);
	static Err Update();
	static Err Shutdown();

	static bool IsInit();
	static bool IsGamepadEnabled();
	static bool IsMouseEnabled();
	static bool IsKeyboardEnabled();

	static Err SubForInputEvent(const Subscription& sub);
	static const InputState& GetInputState();
};
