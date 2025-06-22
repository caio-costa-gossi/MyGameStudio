#pragma once
#include "Gamepad.h"

struct GamepadAxisEvent
{
	GamepadAxis Axis;
	int32_t Value;
	uint8_t ControllerId;

	[[nodiscard]]
	std::string ToString() const
	{
		return
			"\n\tAxis: " + std::to_string(Axis) +
			"\n\tValue: " + std::to_string(Value) +
			"\n\tControllerId: " + std::to_string(ControllerId) +
			"\n";
	}
};
