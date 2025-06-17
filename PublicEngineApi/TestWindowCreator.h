#pragma once
#include <Windows.h>
#include <dinput.h>
#include <SDL3/SDL.h>
#include "Err.h"

class TestWindowCreator
{
private:
	static SDL_Window* window_;
	static HWND hWindow_;

public:
	static Err StartTestWindow();
	static Err UpdateTestWindow();
	static Err CreateTestWindow();
	static Err Shutdown();
	static LRESULT CALLBACK TestWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
