#pragma once
#include <cstdint>

enum MouseButton : uint8_t
{
	mouse_button_invalid = 0,
	mouse_button_left,
	mouse_button_middle,
	mouse_button_right,
	mouse_button_x1,
	mouse_button_x2,
	mouse_button_misc1,
	mouse_button_misc2,
	mouse_button_count
};

struct MouseState
{
	uint8_t BtnState = 0;

	float XPos = 0;
	float YPos = 0;

	float XVel = 0;
	float YVel = 0;

	float WheelXVel = 0;
	float WheelYVel = 0;
};
