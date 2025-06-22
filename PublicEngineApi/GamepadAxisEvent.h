#pragma once
#include "Gamepad.h"

struct GamepadAxisEvent
{
	GamepadAxis Axis;
	int32_t Value;
	uint8_t ControllerId;
};
