#pragma once
#include <cstdint>

#include "GamepadAxisEvent.h"
#include "GamepadButtonEvent.h"
#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "MouseMoveEvent.h"
#include "MouseWheelEvent.h"

enum EventType : uint32_t
{
	event_invalid = 0,
	event_generic = 1,

	// Gamepad
	event_gamepad_range_start = 10,
	event_gamepad_button_press = 11,
	event_gamepad_button_release = 12,
	event_gamepad_axis_move = 13,
	event_gamepad_range_end = 39,

	// Keyboard
	event_keyboard_range_start = 40,
	event_keyboard_button_press = 41,
	event_keyboard_button_release = 42,
	event_keyboard_range_end = 69,

	// Mouse
	event_mouse_range_start = 70,
	event_mouse_button_press = 71,
	event_mouse_button_release = 72,
	event_mouse_move = 73,
	event_mouse_wheel = 74,
	event_mouse_range_end = 99
};

enum EventClass : uint8_t
{
	event_class_invalid = 0,
	event_class_generic = 1,
	event_class_gamepad = 2,
	event_class_mouse = 3,
	event_class_keyboard = 4
};

struct Event
{
	EventType Type;
	EventClass Class;
	int64_t Timestamp;

	// Event type-specific information
	GamepadButtonEvent GamepadButton;
	GamepadAxisEvent GamepadAxis;
	MouseButtonEvent MouseButton;
	MouseMoveEvent MouseMove;
	MouseWheelEvent MouseWheel;
	KeyboardEvent Keyboard;

	// Aux methods
	[[nodiscard]]
	std::string ToString() const;
};

