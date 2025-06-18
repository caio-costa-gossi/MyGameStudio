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
	gamepad_button_dpad_up,
	gamepad_button_dpad_down,
	gamepad_button_dpad_left,
	gamepad_button_dpad_right,
	gamepad_button_misc1,
	gamepad_button_right_paddle1,
	gamepad_button_left_paddle1,
	gamepad_button_right_paddle2,
	gamepad_button_left_paddle2,
	gamepad_button_touchpad,
	gamepad_button_misc2,
	gamepad_button_misc3,
	gamepad_button_misc4,
	gamepad_button_misc5,
	gamepad_button_misc6
};

struct GamepadState
{
	uint32_t BtnState = 0;
	uint32_t HatState = 0;
	float LTrigger = 0;
	float RTrigger = 0;
	float LAnalogX = 0;
	float LAnalogY = 0;
	float RAnalogX = 0;
	float RAnalogY = 0;
};

struct Gamepad
{
	GamepadState State;
	std::string Name;
};