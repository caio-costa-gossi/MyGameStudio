#pragma once
#define SDL_PROP_WINDOW_WIND32_HWND_POINTER

#include <Windows.h>
#include <dinput.h>
#include <SDL3/SDL.h>
#include <vector>
#include "Err.h"

class TestWindowCreator
{
private:
	static SDL_Window* window_;
	static HWND hWindow_;
	static std::vector<SDL_Event> inputEventList_;

	static Err CreateTestWindow();

public:
	static Err Startup(bool useSdl = true, int32_t deadzone = 2500);
	static Err Run();
	static Err Update();
	static Err Shutdown();
};
