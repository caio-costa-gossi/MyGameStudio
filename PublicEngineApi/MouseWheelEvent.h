#pragma once

struct MouseWheelEvent
{
	float WheelXVel;
	float WheelYVel;

	[[nodiscard]]
	std::string ToString() const
	{
		return
			"\n\tWheelXVel: " + std::to_string(WheelXVel) +
			"\n\tWheelYVel: " + std::to_string(WheelYVel) +
			"\n";
	}
};