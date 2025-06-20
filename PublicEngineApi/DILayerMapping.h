#pragma once
#include <unordered_map>

static std::unordered_map<uint8_t, uint8_t> gKeyboardMapping =
{
	{0,keyboard_key_unknown},
	{1,2},
	{2,0},
	{3,5}
};

// Most common mapping, but not guaranteed
static std::unordered_map<uint8_t, uint8_t> gGamepadButtonMapping =
{
	{0,gamepad_button_west},
	{1,gamepad_button_south},
	{2,gamepad_button_east},
	{3,gamepad_button_north},
	{4,gamepad_button_left_shoulder},
	{5,gamepad_button_right_shoulder},
	{6,gamepad_button_misc1},
	{7,gamepad_button_misc2},
	{8,gamepad_button_back},
	{9,gamepad_button_start},
	{10,gamepad_button_left_stick},
	{11,gamepad_button_right_stick},
	{12,gamepad_button_guide},
	{13,gamepad_button_touchpad},
	{14,gamepad_button_right_paddle1},
	{15,gamepad_button_left_paddle1},
	{16,gamepad_button_right_paddle2},
	{17,gamepad_button_left_paddle2},
	{18,gamepad_button_misc3},
	{19,gamepad_button_misc4},
	{20,gamepad_button_misc5},
	{21,gamepad_button_misc6},
	{22,gamepad_button_misc7},
	{23,gamepad_button_misc8},
	{24,gamepad_button_misc9},
	{25,gamepad_button_misc10},
	{26,gamepad_button_misc11},
	{27,gamepad_button_misc12},
	{28,gamepad_button_dpad_up},
	{29,gamepad_button_dpad_down},
	{30,gamepad_button_dpad_left},
	{31,gamepad_button_dpad_right}
};

static std::unordered_map<uint8_t, uint8_t> gMouseButtonMapping =
{
	{0,mouse_button_left},
	{1,mouse_button_right},
	{2,mouse_button_middle},
	{3,mouse_button_x1}
};