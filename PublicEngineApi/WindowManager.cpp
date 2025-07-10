#include "WindowManager.h"

Err WindowManager::Startup()
{
	// Init subsystem and create window
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	window_ = SDL_CreateWindow("My Game", 640, 480, SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_WarpMouseInWindow(window_, 0, 0);

	if (window_ == nullptr)
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	// Get window handle
	const SDL_PropertiesID winProperties = SDL_GetWindowProperties(window_);
	hWindow_ = static_cast<HWND>(SDL_GetPointerProperty(winProperties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));

	isInit_ = true;

	return error_const::SUCCESS;
}

Err WindowManager::Update()
{
	inputEventList_.clear();

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
			return error_const::GAME_EXIT_REQUEST;

		default:
			break;
		}
	}

	return error_const::SUCCESS;
}

Err WindowManager::Shutdown()
{
	SDL_DestroyWindow(window_);

	return error_const::SUCCESS;
}

SDL_Window* WindowManager::GetSdlWindow()
{
	return window_;
}

HWND WindowManager::GetWindowHandle()
{
	return hWindow_;
}

SDL_Event* WindowManager::GetEventList(uint32_t& eventCount)
{
	eventCount = static_cast<uint32_t>(inputEventList_.size());
	return inputEventList_.data();
}

bool WindowManager::IsInit()
{
	return isInit_;
}



SDL_Window* WindowManager::window_ = nullptr;
HWND WindowManager::hWindow_ = nullptr;
std::vector<SDL_Event> WindowManager::inputEventList_ = std::vector<SDL_Event>();
bool WindowManager::isInit_ = false;