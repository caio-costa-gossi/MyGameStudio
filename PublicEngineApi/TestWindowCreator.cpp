#include "TestWindowCreator.h"
#include <iostream>
#include <SDL3/SDL.h>
#include "GameConsoleManager.h"
#include "InputManager.h"

Err TestWindowCreator::CreateTestWindow()
{
	// Init subsystem and create window
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	window_ = SDL_CreateWindow("Input test window", 640, 480, SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_RESIZABLE);
	SDL_WarpMouseInWindow(window_, 0, 0);

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

	err = InputManager::Startup(hWindow_, true);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err TestWindowCreator::Run()
{
	bool testWindowRunning = true;

	while (testWindowRunning)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			// Collect input events
			if (event.type >= 0x300 && event.type < 0x700)
			{
				inputEventList_.push_back(event);
				continue;
			}

			// Handle other events
			switch (event.type)
			{

			case SDL_EVENT_QUIT:
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				testWindowRunning = false;
				break;
			default:
				break;
			}
		}

		Err err = InputManager::Update(inputEventList_.data(), static_cast<uint32_t>(inputEventList_.size()));
		if (err.Code())
			return err;

		inputEventList_.clear();

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
std::vector<SDL_Event> TestWindowCreator::inputEventList_ = std::vector<SDL_Event>();