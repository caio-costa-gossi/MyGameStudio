#include "WindowManager.h"

#include "GameConsoleManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "TestDrawer.h"

Err WindowManager::Startup()
{
	// Init subsystem and create window
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	window_ = SDL_CreateWindow("My Game", 640, 480, SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);
	if (window_ == nullptr)
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	SDL_WarpMouseInWindow(window_, 0, 0);
	Err err = UpdateWindowInfo();
	if (err.Code())
		return err;

	// Get window handle
	const SDL_PropertiesID winProperties = SDL_GetWindowProperties(window_);
	hWindow_ = static_cast<HWND>(SDL_GetPointerProperty(winProperties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));

	isInit_ = true;

	return error_const::SUCCESS;
}

Err WindowManager::Update()
{
	Err err = HandleWindowEvents();
	if (err.Code())
		return err;

	err = UpdateWindowFocus();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err WindowManager::Shutdown()
{
	SDL_DestroyWindow(window_);

	return error_const::SUCCESS;
}

Err WindowManager::SetRelativeMouseMode()
{
	SDL_SetWindowRelativeMouseMode(window_, true);
	isMouseRelative_ = true;

	return error_const::SUCCESS;
}

Err WindowManager::UnsetRelativeMouseMode()
{
	SDL_SetWindowRelativeMouseMode(window_, false);
	isMouseRelative_ = false;

	return error_const::SUCCESS;
}

Err WindowManager::HandleWindowEvents()
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
		case SDL_EVENT_WINDOW_RESIZED:
		case SDL_EVENT_WINDOW_MOVED:
			UpdateWindowInfo();
			RenderManager::ResizeViewport(winWidth_, winHeight_);
			break;

		case SDL_EVENT_QUIT:
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			return error_const::GAME_EXIT_REQUEST;

		default:
			break;
		}
	}

	return error_const::SUCCESS;
}

Err WindowManager::UpdateWindowInfo()
{
	SDL_GetWindowSize(window_, &winWidth_, &winHeight_);
	SDL_GetWindowPosition(window_, &winPosX_, &winPosY_);

	return error_const::SUCCESS;
}

Err WindowManager::UpdateWindowFocus()
{
	if (!InputManager::IsInit())
	{
		GameConsoleManager::PrintWarning("Input manager was not initialized. This can cause problems.", enums::ConsoleMessageSender::window);
		return error_const::SUCCESS;
	}

	if (!InputManager::IsMouseEnabled() || !InputManager::IsKeyboardEnabled())
		return error_const::SUCCESS;

	const InputState state = InputManager::GetInputState();

	if ((state.MouseState.BtnState & 1 << mouse_button_left) != 0)
	{
		Err err = SetRelativeMouseMode();
		if (err.Code())
			return err;
	}
	else if (state.KeyboardState.PhysicalKeyState[keyboard_key_escape])
	{
		Err err = UnsetRelativeMouseMode();
		if (err.Code())
			return err;
	}

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

int32_t WindowManager::GetWindowHeight()
{
	return winHeight_;
}

int32_t WindowManager::GetWindowWidth()
{
	return winHeight_;
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

bool WindowManager::IsMouseRelative()
{
	return isMouseRelative_;
}


SDL_Window* WindowManager::window_ = nullptr;
HWND WindowManager::hWindow_ = nullptr;

int32_t WindowManager::winWidth_ = 0;
int32_t WindowManager::winHeight_ = 0;
int32_t WindowManager::winPosX_ = 0;
int32_t WindowManager::winPosY_ = 0;

std::vector<SDL_Event> WindowManager::inputEventList_ = std::vector<SDL_Event>();

bool WindowManager::isInit_ = false;
bool WindowManager::isMouseRelative_ = false;