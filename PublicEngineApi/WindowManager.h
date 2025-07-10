#pragma once
#include <vector>
#include "Err.h"
#include <Windows.h>
#include <SDL3/SDL.h>

class WindowManager
{
private:
	static SDL_Window* window_;
	static HWND hWindow_;

	static std::vector<SDL_Event> inputEventList_;

	static bool isInit_;

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static HWND GetWindowHandle();
	static SDL_Window* GetSdlWindow();
	static SDL_Event* GetEventList(uint32_t& eventCount);

	static bool IsInit();
};
