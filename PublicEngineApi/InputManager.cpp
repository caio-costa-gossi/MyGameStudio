#include "InputManager.h"
#include <windows.h>
#include <dinput.h>

#include "GameConsoleManager.h"

Err InputManager::Startup()
{
	const HINSTANCE hModule = GetModuleHandle(nullptr);

	// Init interface
	if (FAILED(DirectInput8Create(hModule, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID*>(dInput_), nullptr)))
		return error_const::INPUT_MOD_INIT_FAIL;

	// Enumerate joysticks and create
	if (FAILED(dInput_->EnumDevices(DI8DEVCLASS_ALL, EnumDevicesCallback, nullptr, DIEDFL_ALLDEVICES)))
		return error_const::INPUT_ENUM_FAIL;

	joystickCount_ = static_cast<uint8_t>(joysticks_.size());

	// Create mouse & keyboard
	if (FAILED(dInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, nullptr)))
		return error_const::INPUT_CREATE_FAIL;

	if (FAILED(dInput_->CreateDevice(GUID_SysMouse, &mouse_, nullptr)))
		return error_const::INPUT_CREATE_FAIL;

	// Set data format
	for (const Device device : joysticks_)
	{
		if (FAILED(device->SetDataFormat(&c_dfDIJoystick)))
			return error_const::INPUT_SET_FORMAT_FAIL;
	}

	if (FAILED(mouse_->SetDataFormat(&c_dfDIMouse)))
		return error_const::INPUT_SET_FORMAT_FAIL;

	if (FAILED(keyboard_->SetDataFormat(&c_dfDIKeyboard)))
		return error_const::INPUT_SET_FORMAT_FAIL;

	// Set cooperative level
	HWND hConsole = GetConsoleWindow();

	for (const Device device: joysticks_)
	{
		if (FAILED(device->SetCooperativeLevel(hConsole, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			return error_const::INPUT_SET_CL_FAIL;
	}

	if (FAILED(mouse_->SetCooperativeLevel(hConsole, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		return error_const::INPUT_SET_CL_FAIL;

	if (FAILED(keyboard_->SetCooperativeLevel(hConsole, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		return error_const::INPUT_SET_CL_FAIL;

	// Acquire devices
	for (const Device device : joysticks_)
	{
		if (FAILED(device->Acquire()))
			return error_const::INPUT_DEV_ACQUIRE_FAIL;
	}

	if (FAILED(mouse_->Acquire()))
		return error_const::INPUT_DEV_ACQUIRE_FAIL;

	if (FAILED(keyboard_->Acquire()))
		return error_const::INPUT_DEV_ACQUIRE_FAIL;

	return error_const::SUCCESS;
}

Err InputManager::Update()
{
	if (isKeyboardActive_)
	{
		if (keyboard_->Poll() != DI_OK)
			GameConsoleManager::PrintError("Error polling from keyboard");
		else
		{
			keyboard_->GetDeviceState(sizeof(keyboardState_), &keyboardState_);
		}
	}

	if (isMouseActive_)
	{
		if (mouse_->Poll() != DI_OK)
			GameConsoleManager::PrintError("Error polling from mouse");
		else
		{
			mouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
		}
	}

	if (isJoystickActive_)
	{
		for (uint8_t i = 0; i < joystickCount_; ++i)
		{
			if (joysticks_[i]->Poll() != DI_OK)
				GameConsoleManager::PrintError("Error polling from joystick " + std::to_string(i));
			else
			{
				joysticks_[i]->GetDeviceState(sizeof(joystickStates_[i]), &joystickStates_[i]);
			}
		}
	}

	return error_const::SUCCESS;
}

Err InputManager::Shutdown()
{
	for (Device device : joysticks_)
	{
		if (device) device->Unacquire();
		if (device) device->Release();
	}

	if (keyboard_) keyboard_->Unacquire();
	if (keyboard_) keyboard_->Release();
	if (mouse_) mouse_->Unacquire();
	if (mouse_) mouse_->Release();

	if (dInput_) dInput_->Release();

	return error_const::SUCCESS;
}

Err InputManager::SubBeforeInput(const Subscription& sub)
{
	return error_const::SUCCESS;
}

BOOL InputManager::EnumDevicesCallback(const LPCDIDEVICEINSTANCE instance, LPVOID value)
{
	deviceInstances_.push_back(instance);

	Device newDevice;
	dInput_->CreateDevice(instance->guidInstance, &newDevice, nullptr);
	joysticks_.push_back(newDevice);

	return DIENUM_CONTINUE;
}

LPDIRECTINPUT8 InputManager::dInput_ = nullptr;

auto InputManager::deviceInstances_ = std::vector<LPCDIDEVICEINSTANCE>();
auto InputManager::joysticks_ = std::vector<Device>();
Device InputManager::mouse_;
Device InputManager::keyboard_;

auto InputManager::joystickStates_ = std::vector<DIJOYSTATE>(4);
DIMOUSESTATE InputManager::mouseState_;
BYTE InputManager::keyboardState_[256] = {0};

uint8_t InputManager::joystickCount_ = 0;
bool InputManager::isKeyboardActive_ = true;
bool InputManager::isMouseActive_ = true;
bool InputManager::isJoystickActive_ = true;