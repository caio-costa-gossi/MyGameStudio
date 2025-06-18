#include "SDLInputLayer.h"
#include "GameConsoleManager.h"
#include "NumericUtils.h"

#undef max()

Err SDLInputLayer::Startup(HWND hWindow)
{
	if (!SDL_Init(SDL_INIT_EVENTS))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

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

Err SDLInputLayer::Update()
{
	SDL_Event event;
	Err err;

	// Update currentState_
	while (SDL_PollEvent(&event))
	{
		if (isGamepadActive_)
		{
			err = UpdateGamepads(event);
			if (err.Code())
				GameConsoleManager::PrintError(err.Message());
		}
	}

	GameConsoleManager::PrintInfo(std::to_string(currentState_.Gamepads[0].State.AxisState[gamepad_axis_rightx]));

	return error_const::SUCCESS;
}

InputState SDLInputLayer::GetInputStates()
{
	return currentState_;
}

Err SDLInputLayer::StartupGamepads()
{
	if (!SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD))
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
	case SDL_EVENT_JOYSTICK_BUTTON_UP:
	case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
		err = UpdateGamepadButton(static_cast<uint8_t>(event.jbutton.which), event.jbutton.button, event.jbutton.down);
		break;

	case SDL_EVENT_JOYSTICK_HAT_MOTION:
		err = UpdateGamepadHat(static_cast<uint8_t>(event.jhat.which), event.jhat.value);
		break;

	case SDL_EVENT_JOYSTICK_AXIS_MOTION:
		err = UpdateGamepadAxis(static_cast<uint8_t>(event.jaxis.which), event.jaxis.axis, event.jaxis.value);
		break;

	default:
		break;
	}

	return err;
}

Err SDLInputLayer::UpdateGamepadButton(const uint8_t gamepadId, const uint8_t buttonId, const bool isPressed)
{
	uint32_t& buttonState = currentState_.Gamepads[idToIndex_[gamepadId]].State.BtnState;
	const uint32_t mask = NumericUtils::Bitmask(buttonId);

	buttonState = (buttonState & ~mask) | (isPressed * mask);
	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateGamepadHat(const uint8_t gamepadId, const uint8_t newHatState)
{
	// Merge bits 24-32 from Hat reading with the buttonState
	uint32_t& buttonState = currentState_.Gamepads[idToIndex_[gamepadId]].State.BtnState;
	buttonState = (buttonState & 0x00FFFFFF) | (static_cast<uint32_t>(newHatState) << 24);
	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateGamepadAxis(const uint8_t gamepadId, const uint8_t axisId, const int16_t axisValue)
{
	const int16_t rectifiedAxisValue = 
		(axisId == gamepad_axis_right_trigger || axisId == gamepad_axis_left_trigger) ? (axisValue + std::numeric_limits<int16_t>::max()) / 2 : axisValue;

	currentState_.Gamepads[idToIndex_[gamepadId]].State.AxisState[axisId] = rectifiedAxisValue;
	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateKeyboard(const SDL_Event& event)
{
	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateMouse(const SDL_Event& event)
{
	return error_const::SUCCESS;
}
