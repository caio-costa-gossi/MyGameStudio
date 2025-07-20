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

	static int32_t winWidth_;
	static int32_t winHeight_;
	static int32_t winPosX_;
	static int32_t winPosY_;

	static std::vector<SDL_Event> inputEventList_;

	static bool isInit_;
	static bool isMouseRelative_;

	static Err UpdateWindowInfo();

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err SetRelativeMouseMode();
	static Err UnsetRelativeMouseMode();

	static HWND GetWindowHandle();
	static SDL_Window* GetSdlWindow();
	static int32_t GetWindowHeight();
	static int32_t GetWindowWidth();

	static SDL_Event* GetEventList(uint32_t& eventCount);

	static bool IsInit();
	static bool IsMouseRelative();
};
