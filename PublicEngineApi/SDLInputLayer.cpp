#include "SDLInputLayer.h"
#include "GameConsoleManager.h"
#include "NumericUtils.h"
#include <bitset>

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

	// Reset mouse vel
	currentState_.MouseState.XVel = 0;
	currentState_.MouseState.YVel = 0;
	currentState_.MouseState.WheelXVel = 0;
	currentState_.MouseState.WheelYVel = 0;

	// Update currentState_
	while (SDL_PollEvent(&event))
	{
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

	std::string xPos = std::to_string(currentState_.MouseState.XPos);
	std::string yPos = std::to_string(currentState_.MouseState.YPos);
	std::string xVel = std::to_string(currentState_.MouseState.XVel);
	std::string yVel = std::to_string(currentState_.MouseState.YVel);
	std::string xWheel = std::to_string(currentState_.MouseState.WheelXVel);
	std::string yWheel = std::to_string(currentState_.MouseState.WheelYVel);
	std::string mouseBtn = std::bitset<8>(currentState_.MouseState.BtnState).to_string();
	GameConsoleManager::PrintInfo("(" + xPos + "," + yPos + "); (" + xVel + "," + yVel + "); (" + xWheel + "," + yWheel + "); - " + mouseBtn);

	return error_const::SUCCESS;
}

InputState SDLInputLayer::GetInputStates()
{
	return currentState_;
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
	uint32_t& buttonState = currentState_.Gamepads[idToIndex_[gamepadId]].State.BtnState;
	const uint32_t mask = NumericUtils::Bitmask(buttonId);

	buttonState = (buttonState & ~mask) | (isPressed * mask);
	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateGamepadAxis(const uint8_t gamepadId, const uint8_t axisId, const int16_t axisValue)
{
	currentState_.Gamepads[idToIndex_[gamepadId]].State.AxisState[axisId] = axisValue;
	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateKeyboard(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP:
		currentState_.KeyboardState.BtnState[event.key.scancode] = event.key.down;
		break;

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
	uint8_t& buttonState = currentState_.MouseState.BtnState;
	const uint8_t mask = static_cast<uint8_t>(NumericUtils::Bitmask(buttonIndex));

	buttonState = (buttonState & ~mask) | (isPressed * mask);
	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateMouseMotion(const SDL_MouseMotionEvent& event)
{
	currentState_.MouseState.XPos = event.x;
	currentState_.MouseState.YPos = event.y;
	currentState_.MouseState.XVel = event.xrel;
	currentState_.MouseState.YVel = event.yrel;

	return error_const::SUCCESS;
}

Err SDLInputLayer::UpdateMouseWheel(const SDL_MouseWheelEvent& event)
{
	currentState_.MouseState.WheelXVel = event.x;
	currentState_.MouseState.WheelYVel = event.y;

	return error_const::SUCCESS;
}
