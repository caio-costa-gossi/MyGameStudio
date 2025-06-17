#pragma once
#include <dinput.h>
#include "EventDispatcher.h"
#include <Windows.h>
#include "InputLayer.h"

struct Device
{
	LPDIRECTINPUTDEVICE8 Data;
	std::vector<std::string> ObjectNames;
};

class DILayer : public InputLayer
{
private:
	// Main DI Interface
	LPDIRECTINPUT8 dInput_ = nullptr;

	// Devices
	std::vector<Device> joysticks_ = std::vector<Device>();
	Device mouse_ = Device();
	Device keyboard_ = Device();

	// States
	std::vector<DIJOYSTATE> joystickStates_ = std::vector<DIJOYSTATE>(4);
	DIMOUSESTATE mouseState_ = DIMOUSESTATE();
	BYTE keyboardState_[256] = {0};

	// Input type control
	uint8_t joystickCount_ = 0;
	bool isKeyboardActive_ = true;
	bool isMouseActive_ = true;
	bool isJoystickActive_ = true;

	// Callback instance access
	static DILayer* instance_;
	static Device* device_;

	EventDispatcher dispatcher_;

	static BOOL EnumDevicesCallback(LPCDIDEVICEINSTANCE instance, LPVOID pContext);
	static BOOL EnumObjectsCallback(LPCDIDEVICEOBJECTINSTANCE object, LPVOID pContext);

public:
	Err Startup(HWND hWindow) override;
	Err Update() override;
	Err Shutdown() override;

	InputState GetInputStates() override;

};