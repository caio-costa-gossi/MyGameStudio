#pragma once
#define SDL_PROP_WINDOW_WIND32_HWND_POINTER

#include <Windows.h>
#include <dinput.h>
#include <SDL3/SDL.h>
#include "Err.h"

class TestWindowCreator
{
private:
	static SDL_Window* window_;
	static HWND hWindow_;

	static Err CreateTestWindow();

public:
	static Err Startup();
	static Err Run();
	static Err Shutdown();
};
