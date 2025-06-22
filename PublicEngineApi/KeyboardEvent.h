#pragma once
#include "Keyboard.h"

struct KeyboardEvent
{
	KeyboardKey KeyboardKey;
	ScancodeKey RawScancodeKey;
	bool Pressed;
};
