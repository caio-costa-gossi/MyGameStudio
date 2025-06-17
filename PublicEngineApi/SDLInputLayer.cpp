#include "SDLInputLayer.h"
#include "GameConsoleManager.h"

Err SDLInputLayer::Startup(HWND hWindow)
{
	if (!SDL_Init(SDL_INIT_EVENTS))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	Err err = StartupJoysticks();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err SDLInputLayer::Shutdown()
{
	for (const Joystick& joystick : joysticks_)
	{
		SDL_CloseJoystick(joystick.Joystick);
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

Err SDLInputLayer::StartupJoysticks()
{
	if (!SDL_Init(SDL_INIT_JOYSTICK))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	int joystickCount;
	SDL_JoystickID* joystickList = SDL_GetJoysticks(&joystickCount);

	// Start joysticks
	if (joystickCount <= 0)
	{
		isJoystickActive_ = false;
		GameConsoleManager::PrintInfo("No joysticks detected. Disabling joystick input...");
		return error_const::SUCCESS;
	}

	// Open joysticks
	for (int i = 0; i < joystickCount; ++i)
	{
		if (joystickList[i] == '\0')
			break;

		SDL_Joystick* newJoystick = SDL_OpenJoystick(joystickList[i]);

		if (newJoystick == nullptr)
		{
			GameConsoleManager::PrintInfo("Failure to open joystick " + std::to_string(i) + ": " + SDL_GetError());
			continue;
		}
			
		joysticks_.push_back({ newJoystick, SDL_GetJoystickName(newJoystick) });
		GameConsoleManager::PrintInfo("Added " + std::string(SDL_GetJoystickName(newJoystick)));
	}

	return error_const::SUCCESS;
}