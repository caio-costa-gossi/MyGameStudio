#pragma once
#include "Keyboard.h"

struct KeyboardEvent
{
	KeyboardKey KeyboardKey;
	ScancodeKey RawScancodeKey;
	bool Pressed;

	[[nodiscard]]
	std::string ToString() const
	{
		return
			"\n\tKeyboardKey: " + std::to_string(KeyboardKey) +
			"\n\tScancodeKey: " + std::to_string(RawScancodeKey) +
			"\n\tPressed: " + std::to_string(Pressed) + 
			"\n";
	}
};
