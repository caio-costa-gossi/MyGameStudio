#pragma once
#include <Windows.h>
#include <dinput.h>
#include "Err.h"

class TestWindowCreator
{
private:
	static HWND hWindow_;

public:
	static Err StartTestWindow();
	static Err UpdateTestWindow();
	static Err CreateTestWindow();
	static LRESULT CALLBACK TestWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
