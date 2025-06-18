#include "DILayer.h"

#include "DILayerMapping.h"
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
	if (FAILED(dInput_->CreateDevice(GUID_SysKeyboard, &keyboard_.Data, nullptr)))
		return error_const::INPUT_CREATE_FAIL;

	if (FAILED(dInput_->CreateDevice(GUID_SysMouse, &mouse_.Data, nullptr)))
		return error_const::INPUT_CREATE_FAIL;

	// Enumerate device objects for joysticks
	for (Device& joystick : joysticks_)
	{
		device_ = &joystick;

		if (FAILED(joystick.Data->EnumObjects(EnumObjectsCallback, nullptr, DIDFT_ALL)))
			GameConsoleManager::PrintError("Failed to enumerate objects");
	}

	// Set data format
	for (const Device device : joysticks_)
	{
		if (FAILED(device.Data->SetDataFormat(&c_dfDIJoystick)))
			return error_const::INPUT_SET_FORMAT_FAIL;
	}

	if (FAILED(mouse_.Data->SetDataFormat(&c_dfDIMouse)))
		return error_const::INPUT_SET_FORMAT_FAIL;

	if (FAILED(keyboard_.Data->SetDataFormat(&c_dfDIKeyboard)))
		return error_const::INPUT_SET_FORMAT_FAIL;

	// Set cooperative level
	for (const Device device : joysticks_)
	{
		if (FAILED(device.Data->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
			return error_const::INPUT_SET_CL_FAIL;
	}

	if (FAILED(mouse_.Data->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return error_const::INPUT_SET_CL_FAIL;

	if (FAILED(keyboard_.Data->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return error_const::INPUT_SET_CL_FAIL;

	// Acquire devices
	for (const Device device : joysticks_)
	{
		HRESULT result = device.Data->Acquire();

		if (FAILED(result))
		{
			GameConsoleManager::PrintError(error_const::INPUT_JOYSTICK_ACQUIRE_FAIL.Message());
			isJoystickActive_ = false;
			break;
		}
	}

	if (FAILED(mouse_.Data->Acquire()))
	{
		GameConsoleManager::PrintError(error_const::INPUT_MOUSE_ACQUIRE_FAIL.Message());
		isMouseActive_ = false;
	}

	if (FAILED(keyboard_.Data->Acquire()))
	{
		GameConsoleManager::PrintError(error_const::INPUT_KEYBOARD_ACQUIRE_FAIL.Message());
		isKeyboardActive_ = false;
	}

	return error_const::SUCCESS;
}

Err DILayer::Update()
{
	Err err;

	if (isKeyboardActive_)
	{
		err = UpdateKeyboard();
		if (err.Code())
			GameConsoleManager::PrintError(err.Message());
	}

	/*for (int i = 0; i < 256; ++i)
	{*/
		if (keyboardState_[30] != 0)
			std::cout << "1";
		else
			std::cout << "0";
	//}
	std::cout << "\n";

	if (isMouseActive_)
	{
		err = UpdateMouse();
		if (err.Code())
			GameConsoleManager::PrintError(err.Message());
	}

	if (isJoystickActive_)
	{
		for (uint8_t i = 0; i < joystickCount_; ++i)
		{
			err = UpdateJoystick(i);
			if (err.Code())
				GameConsoleManager::PrintError(err.Message());
		}
	}

	return error_const::SUCCESS;
}

Err DILayer::UpdateJoystick(const uint8_t joystickId)
{
	joysticks_[joystickId].Data->Poll();

	if (joysticks_[joystickId].Data->GetDeviceState(sizeof(joystickStates_[joystickId]), &joystickStates_[joystickId]) != DI_OK)
	{
		joysticks_[joystickId].Data->Acquire();
		return error_const::INPUT_JOYSTICK_POLL_FAIL;
	}

	return error_const::SUCCESS;
}

Err DILayer::UpdateKeyboard()
{
	keyboard_.Data->Poll();

	if (keyboard_.Data->GetDeviceState(sizeof(keyboardState_), &keyboardState_) != DI_OK)
	{
		keyboard_.Data->Acquire();
		return error_const::INPUT_KEYBOARD_POLL_FAIL;
	}

	for (uint16_t key = 0; key < keyboard_di_count; ++key)
		currentState_.KeyboardState.BtnState[gKeyboardMapping[static_cast<uint8_t>(key)]] = keyboardState_[key] != 0;

	return error_const::SUCCESS;
}

Err DILayer::UpdateMouse()
{
	mouse_.Data->Poll();

	if (mouse_.Data->GetDeviceState(sizeof(mouseState_), &mouseState_) != DI_OK)
	{
		mouse_.Data->Acquire();
		return error_const::INPUT_MOUSE_POLL_FAIL;
	}

	return error_const::SUCCESS;
}

Err DILayer::Shutdown()
{
	for (Device device : joysticks_)
	{
		if (device.Data) device.Data->Unacquire();
		if (device.Data) device.Data->Release();
	}

	if (keyboard_.Data) keyboard_.Data->Unacquire();
	if (keyboard_.Data) keyboard_.Data->Release();
	if (mouse_.Data) mouse_.Data->Unacquire();
	if (mouse_.Data) mouse_.Data->Release();

	if (dInput_) dInput_->Release();

	return error_const::SUCCESS;
}

InputState DILayer::GetInputStates()
{
	return currentState_;
}

BOOL DILayer::EnumDevicesCallback(const LPCDIDEVICEINSTANCE instance, LPVOID pContext)
{
	if (instance_ == nullptr)
		return DIENUM_STOP;

	Device newDevice;
	instance_->dInput_->CreateDevice(instance->guidInstance, &newDevice.Data, nullptr);
	instance_->joysticks_.push_back(newDevice);

	return DIENUM_CONTINUE;
}

BOOL DILayer::EnumObjectsCallback(const LPCDIDEVICEOBJECTINSTANCE object, LPVOID pContext)
{
	if (device_ == nullptr)
		return DIENUM_CONTINUE;

	if (!(object->dwType & DIDFT_BUTTON))
		return DIENUM_CONTINUE;

	char deviceName[256];
	size_t converted;
	wcstombs_s(&converted, deviceName, object->tszName, 256);
	std::cout << " Name: " << deviceName << "\n";

	device_->ObjectNames.emplace_back(deviceName);
	return DIENUM_CONTINUE;
}


DILayer* DILayer::instance_ = nullptr;
Device* DILayer::device_ = nullptr;