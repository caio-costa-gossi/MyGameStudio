#include "Event.h"

std::string Event::ToString() const
{
	return
		"==Event==\nType: " + std::to_string(Type) +
		" | Class: " + std::to_string(Class) +
		" | Timestamp: " + std::to_string(Timestamp) +
		"\nGamepadButtonEvent: " + GamepadButton.ToString() +
		"\nGamepadAxisEvent: " + GamepadAxis.ToString() +
		"\nMouseButtonEvent: " + MouseButton.ToString() +
		"\nMouseMoveEvent: " + MouseMove.ToString() +
		"\nMouseWheelEvent: " + MouseWheel.ToString() +
		"\nKeyboardEvent: " + Keyboard.ToString() +
		"\n\n";
}
