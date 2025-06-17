#pragma once
#include "JoystickState.h"
#include "KeyboardState.h"
#include "MouseState.h"

struct InputState
{
	uint8_t JoystickCount;
	JoystickState* JoystickStates;
	KeyboardState KeyboardState;
	MouseState MouseState;
};
