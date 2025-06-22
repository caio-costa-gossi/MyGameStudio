#pragma once
#include "Gamepad.h"

struct GamepadButtonEvent
{
	GamepadButton Button;
	bool Pressed;
	uint8_t ControllerId;

	[[nodiscard]]
	std::string ToString() const
	{
		return
			"\n\tButton: " + std::to_string(Button) +
			"\n\tPressed: " + std::to_string(Pressed) +
			"\n\tControllerId: " + std::to_string(ControllerId) +
			"\n";
	}
};
