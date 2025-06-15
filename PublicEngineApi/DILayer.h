#pragma once
#include <dinput.h>
#include "EventDispatcher.h"
#include <Windows.h>

using Device = LPDIRECTINPUTDEVICE8;

class DILayer
{
private:
	// Main DI Interface
	static LPDIRECTINPUT8 dInput_;

	// Devices
	static std::vector<Device> joysticks_;
	static Device mouse_;
	static Device keyboard_;

	// States
	static std::vector<DIJOYSTATE> joystickStates_;
	static DIMOUSESTATE mouseState_;
	static BYTE keyboardState_[256];

	// Input type control
	static uint8_t joystickCount_;
	static bool isKeyboardActive_;
	static bool isMouseActive_;
	static bool isJoystickActive_;

	static EventDispatcher dispatcher_;

	static BOOL EnumDevicesCallback(LPCDIDEVICEINSTANCE instance, LPVOID value);

public:
	static Err Startup(HWND hWindow);
	static Err Update();
	static Err Shutdown();

};