#pragma once
#define SDL_PROP_WINDOW_WIND32_HWND_POINTER

#include <Windows.h>
#include <SDL3/SDL.h>
#include <vector>
#include "Err.h"

class TestDrawer
{
private:
	static bool running_;
	static SDL_GLContext glContext_;

	static Err InitRenderer();

public:
	static Err Startup();
	static Err Run();
	static Err Shutdown();
};
