#pragma once
#include <vector>

#include "Gamepad.h"
#include "Keyboard.h"
#include "Mouse.h"

struct InputState
{
	uint8_t GamepadCount = 0;
	std::vector<Gamepad> Gamepads;
	KeyboardState KeyboardState;
	MouseState MouseState;
};
