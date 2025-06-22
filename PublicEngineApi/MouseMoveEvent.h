#pragma once

struct MouseMoveEvent
{
	float XPos;
	float YPos;

	float XVel;
	float YVel;

	[[nodiscard]]
	std::string ToString() const
	{
		return
			"\n\tXPos: " + std::to_string(XPos) +
			"\n\tYPos: " + std::to_string(YPos) +
			"\n\tXVel: " + std::to_string(XVel) +
			"\n\tYVel: " + std::to_string(YVel) +
			"\n";
	}
};