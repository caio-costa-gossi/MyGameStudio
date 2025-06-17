#include "DILayer.h"
#include "GameConsoleManager.h"

Err DILayer::Startup(HWND hWindow)
{
	const HINSTANCE hModule = GetModuleHandle(nullptr);

	// Init interface
	if (FAILED(DirectInput8Create(hModule, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID*>(&dInput_), nullptr)))
		return error_const::INPUT_MOD_INIT_FAIL;

	// Enumerate joysticks and create
	instance_ = this;
	if (FAILED(dInput_->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, nullptr, DIEDFL_ALLDEVICES)))
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
	for (const Device device : joysticks_)
	{
		if (FAILED(device->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
			return error_const::INPUT_SET_CL_FAIL;
	}

	if (FAILED(mouse_->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return error_const::INPUT_SET_CL_FAIL;

	if (FAILED(keyboard_->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return error_const::INPUT_SET_CL_FAIL;

	// Acquire devices
	for (const Device device : joysticks_)
	{
		HRESULT result = device->Acquire();

		if (FAILED(result))
		{
			GameConsoleManager::PrintError(error_const::INPUT_JOYSTICK_ACQUIRE_FAIL.Message());
			isJoystickActive_ = false;
			break;
		}
	}

	if (FAILED(mouse_->Acquire()))
	{
		GameConsoleManager::PrintError(error_const::INPUT_MOUSE_ACQUIRE_FAIL.Message());
		isMouseActive_ = false;
	}

	if (FAILED(keyboard_->Acquire()))
	{
		GameConsoleManager::PrintError(error_const::INPUT_KEYBOARD_ACQUIRE_FAIL.Message());
		isKeyboardActive_ = false;
	}

	return error_const::SUCCESS;
}

Err DILayer::Update()
{
	HRESULT pollResult, stateResult;

	if (isKeyboardActive_)
	{
		keyboard_->Poll();

		if (keyboard_->GetDeviceState(sizeof(keyboardState_), &keyboardState_) != DI_OK)
		{
			GameConsoleManager::PrintError("Error polling from keyboard. Trying to reacquire...");
			keyboard_->Acquire();
		}
	}

	if (isMouseActive_)
	{
		mouse_->Poll();
		if (mouse_->GetDeviceState(sizeof(mouseState_), &mouseState_) != DI_OK)
		{
			GameConsoleManager::PrintError("Error polling from mouse. Trying to reacquire...");
			mouse_->Acquire();
		}

		GameConsoleManager::PrintInfo("(" + std::to_string(mouseState_.lX) + "," + std::to_string(mouseState_.lY) + "," + std::to_string(mouseState_.lZ) + ")");
	}

	if (isJoystickActive_)
	{
		for (uint8_t i = 0; i < joystickCount_; ++i)
		{
			joysticks_[i]->Poll();
			if (joysticks_[i]->GetDeviceState(sizeof(joystickStates_[i]), &joystickStates_[i]) != DI_OK)
			{
				GameConsoleManager::PrintError("Error polling from joystick " + std::to_string(i) + ". Trying to reacquire...");
				joysticks_[i]->Acquire();
			}
		}
	}

	return error_const::SUCCESS;
}

Err DILayer::Shutdown()
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

InputState DILayer::GetInputStates()
{
	return {};
}

BOOL DILayer::EnumDevicesCallback(const LPCDIDEVICEINSTANCE instance, LPVOID value)
{
	if (instance_ == nullptr)
		return DIENUM_STOP;

	Device newDevice;
	instance_->dInput_->CreateDevice(instance->guidInstance, &newDevice, nullptr);
	instance_->joysticks_.push_back(newDevice);

	return DIENUM_CONTINUE;
}


DILayer* DILayer::instance_ = nullptr;