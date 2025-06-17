#pragma once
#include <cstdint>

struct MouseState
{
	uint8_t BtnState = 0;
	long XMov = 0;
	long YMov = 0;
	long ZMov = 0;
};
