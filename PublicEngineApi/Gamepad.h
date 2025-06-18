#pragma once
#include <cstdint>

enum GamepadButton : int8_t
{
	gamepad_button_invalid = -1,
	gamepad_button_south,
	gamepad_button_east,
	gamepad_button_west,
	gamepad_button_north,
	gamepad_button_back,
	gamepad_button_guide,
	gamepad_button_start,
	gamepad_button_left_stick,
	gamepad_button_right_stick,
	gamepad_button_left_shoulder,
	gamepad_button_right_shoulder,
	gamepad_button_touchpad,
	gamepad_button_misc1,
	gamepad_button_right_paddle1,
	gamepad_button_left_paddle1,
	gamepad_button_right_paddle2,
	gamepad_button_left_paddle2,
	gamepad_button_misc2,
	gamepad_button_misc3,
	gamepad_button_misc4,
	gamepad_button_misc5,
	gamepad_button_misc6,
	gamepad_button_misc7,
	gamepad_button_misc8,
	gamepad_button_dpad_up,
	gamepad_button_dpad_right,
	gamepad_button_dpad_down,
	gamepad_button_dpad_left,
	gamepad_button_misc9,
	gamepad_button_misc10,
	gamepad_button_misc11,
	gamepad_button_misc12,
};

struct GamepadState
{
	uint32_t BtnState = 0;
	int16_t AxisState[6] = { 0 };
};

struct Gamepad
{
	GamepadState State;
	std::string Name;
};