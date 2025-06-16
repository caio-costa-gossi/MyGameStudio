#include "SDLInputLayer.h"
#include "GameConsoleManager.h"

Err SDLInputLayer::Startup(HWND hWindow)
{
	Err err = StartupJoysticks();
	if (err.Code())
		return err;

	err = StartupKeyboardMouse();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err SDLInputLayer::Shutdown()
{
	return error_const::SUCCESS;
}

Err SDLInputLayer::Update()
{
	return error_const::SUCCESS;
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



uint8_t SDLInputLayer::joystickCount_ = 0;
auto SDLInputLayer::joysticks_ = std::vector<Joystick>();