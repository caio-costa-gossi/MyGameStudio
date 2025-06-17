#pragma once
#include <cstdint>

struct MouseState
{
	uint8_t BtnState;
	long XMov;
	long YMov;
	long ZMov;
};
