#pragma once
#include <vector>

#include "Gamepad.h"
#include "Keyboard.h"
#include "Mouse.h"

struct InputState
{
	uint8_t JoystickCount;
	std::vector<Gamepad> Gamepads;
	KeyboardState KeyboardState;
	MouseState MouseState;
};
