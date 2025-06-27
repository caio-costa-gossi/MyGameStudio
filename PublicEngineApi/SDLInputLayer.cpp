#include "SDLInputLayer.h"
#include "GameConsoleManager.h"
#include "NumericUtils.h"
#include <bitset>

#undef max()

Err SDLInputLayer::Startup(HWND hWindow, const int32_t deadzone)
{
	deadzone_ = deadzone;

	// SDL Event subsystem must already have been started
	Err err = StartupGamepads();
	if (err.Code())
		return err;

	currentState_.GamepadCount = gamepadCount_;

	return error_const::SUCCESS;
}

Err SDLInputLayer::Shutdown()
{
	for (const GamepadInfo& gamepad : gamepads_)
	{
		SDL_CloseGamepad(gamepad.Gamepad);
	}

	SDL_Quit();
	return error_const::SUCCESS;
}

Err SDLInputLayer::Update(const SDL_Event* eventList, const uint32_t numEvent)
{
	Err err;

	// Reset mouse vel
	currentState_.MouseState.XVel = 0;
	currentState_.MouseState.YVel = 0;
	currentState_.MouseState.WheelXVel = 0;
	currentState_.MouseState.WheelYVel = 0;

	if (eventList == nullptr)
		return error_const::INPUT_SDL_EVENT_NULL;

	// Update currentState_
	for (uint32_t i = 0; i < numEvent; ++i)
	{
		SDL_Event event = eventList[i];

		if (isGamepadActive_)
		{
			err = UpdateGamepads(event);
			if (err.Code())
				GameConsoleManager::PrintError(err.Message());
		}

		if (isKeyboardActive_)
		{
			err = UpdateKeyboard(event);
			if (err.Code())
				GameConsoleManager::PrintError(err.Message());
		}

		if (isMouseActive_)
		{
			err = UpdateMouse(event);
			if (err.Code())
				GameConsoleManager::PrintError(err.Message());
		}
	}

	// Dispatch events of this frame
	if (!eventsToFlush_.empty())
	{
		err = FireEvents();
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err SDLInputLayer::StartupGamepads()
{
	if (!SDL_Init(SDL_INIT_GAMEPAD))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	int gamepadCount;
	SDL_JoystickID* gamepadList = SDL_GetGamepads(&gamepadCount);
	gamepadCount_ = static_cast<uint8_t>(gamepadCount);

	// Check gamepads
	if (gamepadCount <= 0)
	{
		isGamepadActive_ = false;
		GameConsoleManager::PrintInfo("No gamepads detected. Disabling gamepad input...");
		return error_const::SUCCESS;
	}

	// Open gamepads
	for (uint8_t i = 0; i < gamepadCount; ++i)
	{
		if (gamepadList[i] == '\0')
			break;

		SDL_Gamepad* newSdlGamepad = SDL_OpenGamepad(gamepadList[i]);

		if (newSdlGamepad == nullptr)
		{
			GameConsoleManager::PrintInfo("Failure to open gamepad " + std::to_string(i) + ": " + SDL_GetError());
			continue;
		}

		Gamepad newGamepad = { GamepadState(), SDL_GetGamepadName(newSdlGamepad) };
			
		// Maintain internal SDL and engine structures
		gamepads_.push_back({ newSdlGamepad, SDL_GetGamepadType(newSdlGamepad), newGamepad.Name });
		currentState_.Gamepads.push_back(newGamepad);

		// Translate ID to index
		idToIndex_[static_cast<uint8_t>(gamepadList[i])] = i;

		GameConsoleManager::PrintInfo("Added " + std::string(newGamepad.Name));
	}

	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateGamepads(const SDL_Event& event)
{
	Err err = error_const::SUCCESS;

	switch (event.type)
	{
	case SDL_EVENT_GAMEPAD_BUTTON_UP:
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		err = UpdateGamepadButton(static_cast<uint8_t>(event.jbutton.which), event.jbutton.button, event.jbutton.down);
		break;

	case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		err = UpdateGamepadAxis(static_cast<uint8_t>(event.jaxis.which), event.jaxis.axis, event.jaxis.value);
		break;

	default:
		break;
	}

	return err;
}

Err SDLInputLayer::UpdateGamepadButton(const uint8_t gamepadId, const uint8_t buttonId, const bool isPressed)
{
	// Update state
	uint32_t& buttonState = currentState_.Gamepads[idToIndex_[gamepadId]].State.BtnState;
	const uint32_t mask = NumericUtils::Bitmask(buttonId);

	buttonState = (buttonState & ~mask) | (isPressed * mask);

	// Build event
	const GamepadButtonEvent btnEvent = { static_cast<GamepadButton>(buttonId), isPressed, idToIndex_[gamepadId] };
	Event newEvent = {isPressed ? event_gamepad_button_press : event_gamepad_button_release, event_class_gamepad, 0 };
	newEvent.GamepadButton = btnEvent;
	eventsToFlush_.emplace_back(newEvent);

	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateGamepadAxis(const uint8_t gamepadId, const uint8_t axisId, const int16_t axisValue)
{
	int16_t valueAfterDeadzone = axisValue;
	if (std::abs(axisValue) <= deadzone_)
		valueAfterDeadzone = 0;

	// Only change values if new value is different from old value
	if (valueAfterDeadzone == currentState_.Gamepads[idToIndex_[gamepadId]].State.AxisState[axisId])
		return error_const::SUCCESS;

	// Update state
	currentState_.Gamepads[idToIndex_[gamepadId]].State.AxisState[axisId] = valueAfterDeadzone;

	// Build event
	const GamepadAxisEvent axisEvent = { static_cast<GamepadAxis>(axisId), valueAfterDeadzone, idToIndex_[gamepadId] };
	Event newEvent = { event_gamepad_axis_move, event_class_gamepad, 0 };
	newEvent.GamepadAxis = axisEvent;
	eventsToFlush_.emplace_back(newEvent);

	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateKeyboard(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP:
	{
		if (event.key.raw < scancode_key_count)
			currentState_.KeyboardState.RawState[event.key.raw] = event.key.down;

		currentState_.KeyboardState.PhysicalKeyState[event.key.scancode] = event.key.down;

		const KeyboardEvent kbEvent = { static_cast<KeyboardKey>(event.key.scancode), static_cast<ScancodeKey>(event.key.raw), event.key.down };
		Event newEvent = { event.key.down ? event_keyboard_button_press : event_keyboard_button_release, event_class_keyboard, 0 };
		newEvent.Keyboard = kbEvent;
		eventsToFlush_.emplace_back(newEvent);

		break;
	}

	default:
		break;
	}

	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateMouse(const SDL_Event& event)
{
	Err err = error_const::SUCCESS;

	switch (event.type)
	{
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
		err = UpdateMouseButton(event.button.button, event.button.down);
		break;

	case SDL_EVENT_MOUSE_MOTION:
		err = UpdateMouseMotion(event.motion);
		break;

	case SDL_EVENT_MOUSE_WHEEL:
		err = UpdateMouseWheel(event.wheel);
		break;

	default:
		break;
	}

	return err;
}

Err SDLInputLayer::UpdateMouseButton(const uint8_t buttonIndex, const bool isPressed)
{
	// Update state
	uint8_t& buttonState = currentState_.MouseState.BtnState;
	const uint8_t mask = static_cast<uint8_t>(NumericUtils::Bitmask(buttonIndex));
	buttonState = (buttonState & ~mask) | (isPressed * mask);

	// Build event
	const MouseButtonEvent btnEvent = { static_cast<MouseButton>(buttonIndex), isPressed };
	Event newEvent = { isPressed ? event_mouse_button_press : event_mouse_button_release, event_class_mouse, 0 };
	newEvent.MouseButton = btnEvent;
	eventsToFlush_.emplace_back(newEvent);

	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateMouseMotion(const SDL_MouseMotionEvent& event)
{
	// Update state
	currentState_.MouseState.XPos = event.x;
	currentState_.MouseState.YPos = event.y;
	currentState_.MouseState.XVel = event.xrel;
	currentState_.MouseState.YVel = event.yrel;

	// Build event
	const MouseMoveEvent moveEvent = { event.x, event.y, event.xrel, event.yrel };
	Event newEvent = { event_mouse_move, event_class_mouse, 0 };
	newEvent.MouseMove = moveEvent;
	eventsToFlush_.emplace_back(newEvent);

	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateMouseWheel(const SDL_MouseWheelEvent& event)
{
	// Update state
	currentState_.MouseState.WheelXVel = event.x;
	currentState_.MouseState.WheelYVel = event.y;

	// Build event
	const MouseWheelEvent wheelEvent = { event.x, event.y };
	Event newEvent = { event_mouse_wheel, event_class_mouse, 0 };
	newEvent.MouseWheel = wheelEvent;
	eventsToFlush_.emplace_back(newEvent);

	return error_const::SUCCESS;
}