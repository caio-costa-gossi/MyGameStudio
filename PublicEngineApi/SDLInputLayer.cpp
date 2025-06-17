#include "SDLInputLayer.h"
#include "GameConsoleManager.h"

Err SDLInputLayer::Startup(HWND hWindow)
{
	if (!SDL_Init(SDL_INIT_EVENTS))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	Err err = StartupGamepads();
	if (err.Code())
		return err;

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

	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
		/*case SDL_EVENT_JOYSTICK_AXIS_MOTION:
			std::cout << "Axis " << static_cast<int>(event.jaxis.axis)
				<< " moved to " << event.jaxis.value << "\n";
			break;*/

		case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
			std::cout << "Button " << static_cast<int>(event.jbutton.button)
				<< " pressed.\n";
			break;

		case SDL_EVENT_JOYSTICK_BUTTON_UP:
			std::cout << "Button " << static_cast<int>(event.jbutton.button)
				<< " released.\n";
			break;

		case SDL_EVENT_JOYSTICK_HAT_MOTION:
			std::cout << "Hat " << static_cast<int>(event.jhat.hat)
				<< " changed to " << static_cast<int>(event.jhat.value) << "\n";
			break;
		case SDL_EVENT_KEY_DOWN:
			std::cout << "Key down!\n";
			break;
		case SDL_EVENT_KEY_UP:
			std::cout << "Key up!\n";
			break;
		case SDL_EVENT_MOUSE_MOTION:
			std::cout << "Mouse moved!\n";
			break;
		case SDL_EVENT_MOUSE_ADDED:
			std::cout << "Mouse added!\n";
			break;
		}
	}

	return error_const::SUCCESS;
}

InputState SDLInputLayer::GetInputStates()
{
	return {};
}

Err SDLInputLayer::StartupGamepads()
{
	if (!SDL_Init(SDL_INIT_JOYSTICK))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	int gamepadCount;
	SDL_JoystickID* gamepadList = SDL_GetGamepads(&gamepadCount);
	gamepadCount_ = static_cast<uint8_t>(gamepadCount);

	// Start gamepads
	if (gamepadCount <= 0)
	{
		isGamepadActive_ = false;
		GameConsoleManager::PrintInfo("No gamepads detected. Disabling gamepad input...");
		return error_const::SUCCESS;
	}

	// Open gamepads
	for (int i = 0; i < gamepadCount; ++i)
	{
		if (gamepadList[i] == '\0')
			break;

		SDL_Gamepad* newGamepad = SDL_OpenGamepad(gamepadList[i]);

		if (newGamepad == nullptr)
		{
			GameConsoleManager::PrintInfo("Failure to open gamepad " + std::to_string(i) + ": " + SDL_GetError());
			continue;
		}
			
		gamepads_.push_back({ newGamepad, SDL_GetGamepadName(newGamepad) });
		GameConsoleManager::PrintInfo("Added " + std::string(SDL_GetGamepadName(newGamepad)));
	}

	return error_const::SUCCESS;
}