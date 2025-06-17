#include "TestWindowCreator.h"
#include <iostream>
#include <SDL3/SDL.h>

#include "GameConsoleManager.h"
#include "InputManager.h"

Err TestWindowCreator::StartTestWindow()
{
	Err err = CreateTestWindow();
	if (err.Code())
		return err;

	err = InputManager::Startup(hWindow_);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err TestWindowCreator::UpdateTestWindow()
{
	MSG msg = {};

	while (true) 
	{
		PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)
			break;

		Err err = InputManager::Update();
		if (err.Code())
			return err;

		TranslateMessage(&msg);
		DispatchMessageA(&msg);

		// Cap 20 FPS
		Sleep(50);
	}

	return error_const::SUCCESS;
}

LRESULT TestWindowCreator::TestWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

Err TestWindowCreator::CreateTestWindow()
{
	// Init subsystem and create window
	if (SDL_Init(SDL_INIT_VIDEO))
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	window_ = SDL_CreateWindow("Input test window", 640, 480, SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS);

	if (window_ == nullptr)
		return Err(SDL_GetError(), error_const::SDL_ERROR_CODE);

	// Set HW properties
	SDL_SysWM

	const auto className = "TestWindowClass";

	WNDCLASSA wc = {};
	wc.lpfnWndProc = TestWindowProc;
	wc.hInstance = GetModuleHandleA(nullptr);
	wc.lpszClassName = reinterpret_cast<LPCSTR>(className);

	RegisterClassA(&wc);

	// Create the window
	hWindow_ = CreateWindowExA(
		0,
		className,
		"DirectInput Test Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		nullptr, nullptr, GetModuleHandleA(nullptr), nullptr
	);

	if (hWindow_ == nullptr) {
		std::cerr << "Failed to create window.\n";
		return error_const::GENERIC_EXCEPTION;
	}

	ShowWindow(hWindow_, SW_SHOW);
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