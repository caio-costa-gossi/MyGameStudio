#pragma once
#include "Mouse.h"

struct MouseButtonEvent
{
	MouseButton Button;
	bool Pressed;

	[[nodiscard]]
	std::string ToString() const
	{
		return
			"\n\tButton: " + std::to_string(Button) +
			"\n\tPressed: " + std::to_string(Pressed) +
			"\n";
	}
};
