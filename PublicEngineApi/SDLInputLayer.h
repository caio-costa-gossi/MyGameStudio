#pragma once
#include <vector>
#include <Windows.h>
#include <SDL3/SDL.h>
#include "Err.h"

struct SDLJoystickInterface
{
	SDL_Joystick* Joystick;
	const char* JoystickName;
};

using Joystick = SDLJoystickInterface;

class SDLInputLayer
{
private:
	// Devices
	static std::vector<Joystick> joysticks_;

	// Input type control
	static uint8_t joystickCount_;
	static bool isKeyboardActive_;
	static bool isMouseActive_;
	static bool isJoystickActive_;

	static Err StartupJoysticks();
	static Err StartupKeyboardMouse();

public:
	static Err Startup(HWND hWindow);
	static Err Update();
	static Err Shutdown();
};
