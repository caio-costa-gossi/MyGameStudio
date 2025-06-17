#include "TestWindowCreator.h"
#include <iostream>
#include <SDL3/SDL.h>
#include "GameConsoleManager.h"
#include "InputManager.h"

Err TestWindowCreator::CreateTestWindow()
{
	// Init subsystem and create window
	if (!SDL_Init(SDL_INIT_VIDEO))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	window_ = SDL_CreateWindow("Input test window", 640, 480, SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS);

	if (window_ == nullptr)
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	// Get window handle
	const SDL_PropertiesID winProperties = SDL_GetWindowProperties(window_);
	SDL_GetPointerProperty(winProperties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, &window_);

	return error_const::SUCCESS;
}

Err TestWindowCreator::Startup()
{
	Err err = CreateTestWindow();
	if (err.Code())
		return err;

	err = InputManager::Startup(hWindow_, false);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err TestWindowCreator::Run()
{
	while (true)
	{
		Err err = InputManager::Update();
		if (err.Code())
			return err;

		// Cap 20 FPS
		Sleep(50);
	}

	return error_const::SUCCESS;
}

Err TestWindowCreator::Shutdown()
{
	SDL_DestroyWindow(window_);
	InputManager::Shutdown();

	return error_const::SUCCESS;
}


SDL_Window* TestWindowCreator::window_;
HWND TestWindowCreator::hWindow_ = nullptr;