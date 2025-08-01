#pragma once
#include <dinput.h>
#include "EventDispatcher.h"
#include <Windows.h>
#include "InputLayer.h"

enum DpadBit : uint8_t
{
	di_dpad_up = 28,
	di_dpad_down,
	di_dpad_left,
	di_dpad_right
};

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
	std::vector<DIJOYSTATE> joystickStates_ = std::vector<DIJOYSTATE>();
	DIMOUSESTATE mouseState_ = DIMOUSESTATE();
	BYTE keyboardState_[keyboard_di_count] = {0};

	// Aux structure for event handling
	InputState nextState_ = InputState();

	// Main private methods
	Err UpdateJoystick(uint8_t joystickId);
	Err UpdateKeyboard();
	Err UpdateMouse();

	// Aux update methods
	Err UpdateJoystickButton(uint8_t joystickId);
	Err UpdateJoystickHat(uint8_t joystickId);
	Err UpdateJoystickAnalog(uint8_t joystickId);
	Err UpdateMouseVelPos(int32_t xVel, int32_t yVel, int32_t zVel);

	static uint16_t ScancodeToKeycode(uint16_t scancode);

	// Callback instance access
	static DILayer* instance_;
	static Device* device_;

	// Callbacks for DI
	static BOOL EnumDevicesCallback(LPCDIDEVICEINSTANCE instance, LPVOID pContext);
	static BOOL EnumObjectsCallback(LPCDIDEVICEOBJECTINSTANCE object, LPVOID pContext);

public:
	Err Startup(HWND hWindow, int32_t deadzone = 500) override;
	Err Update(const SDL_Event* eventList = nullptr, uint32_t numEvent = 0) override;
	Err Shutdown() override;

	const std::string& GetBtnNameByIndex(uint8_t joystickId, uint8_t buttonIndex);
};