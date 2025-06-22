#pragma once
#include "Gamepad.h"

struct GamepadButtonEvent
{
	GamepadButton Button;
	bool Pressed;
	uint8_t ControllerId;
};
