#include "DILayer.h"
#include <bitset>
#include "DILayerMapping.h"
#include "GameConsoleManager.h"

#undef max()
#define DI_WHEEL_BASE_VEL 120

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

	currentState_.GamepadCount = static_cast<uint8_t>(joysticks_.size());

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

const std::string& DILayer::GetBtnNameByIndex(const uint8_t joystickId, const uint8_t buttonIndex)
{
	return joysticks_[joystickId].ObjectNames[buttonIndex];
}

Err DILayer::Update(const SDL_Event* eventList, uint32_t numEvent)
{
	Err err;

	if (isKeyboardActive_)
	{
		err = UpdateKeyboard();
		if (err.Code())
			GameConsoleManager::PrintError(err.Message());
	}

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

	Err err = UpdateJoystickButton(joystickId);
	if (err.Code())
		return err;

	err = UpdateJoystickHat(joystickId);
	if (err.Code())
		return err;

	err = UpdateJoystickAnalog(joystickId);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err DILayer::UpdateJoystickButton(const uint8_t joystickId)
{
	const BYTE* rawBtnState = joystickStates_[joystickId].rgbButtons;
	uint32_t newButtonState = 0;

	// Update buttons, insert into uint32_t using common mapping
	for (uint8_t buttonId = 0; buttonId < gamepad_button_count; ++buttonId)
		newButtonState += (1 << gGamepadButtonMapping[buttonId]) * (rawBtnState[buttonId] != 0);

	currentState_.Gamepads[joystickId].State.BtnState = newButtonState;
	return error_const::SUCCESS;
}

Err DILayer::UpdateJoystickHat(const uint8_t joystickId)
{
	uint32_t& buttonState = currentState_.Gamepads[joystickId].State.BtnState;

	// Update hat -> Bits 28-31: up,down,left,right
	switch (joystickStates_[joystickId].rgdwPOV[0])
	{
	// D-pad UP
	case 0:
		buttonState += 1 << gGamepadButtonMapping[di_dpad_up];
		break;
	// D-pad UP+RIGHT
	case 4500:
		buttonState += 1 << gGamepadButtonMapping[di_dpad_up];
		buttonState += 1 << gGamepadButtonMapping[di_dpad_right];
		break;
	// D-pad RIGHT
	case 9000:
		buttonState += 1 << gGamepadButtonMapping[di_dpad_right];
		break;
	// D-pad RIGHT+DOWN
	case 13500:
		buttonState += 1 << gGamepadButtonMapping[di_dpad_right];
		buttonState += 1 << gGamepadButtonMapping[di_dpad_down];
		break;
	// D-pad DOWN
	case 18000:
		buttonState += 1 << gGamepadButtonMapping[di_dpad_down];
		break;
	// D-pad DOWN+LEFT
	case 22500:
		buttonState += 1 << gGamepadButtonMapping[di_dpad_down];
		buttonState += 1 << gGamepadButtonMapping[di_dpad_left];
		break;
	// D-pad LEFT
	case 27000:
		buttonState += 1 << gGamepadButtonMapping[di_dpad_left];
		break;
	// D-pad LEFT+UP
	case 31500:
		buttonState += 1 << gGamepadButtonMapping[di_dpad_up];
		buttonState += 1 << gGamepadButtonMapping[di_dpad_left];
		break;
	default:
		break;
	}

	return error_const::SUCCESS;
}

Err DILayer::UpdateJoystickAnalog(const uint8_t joystickId)
{
	int32_t int16Max = std::numeric_limits<int16_t>::max();

	// Most common mapping, not guaranteed
	currentState_.Gamepads[joystickId].State.AxisState[gamepad_axis_leftx] = joystickStates_[joystickId].lX - int16Max;
	currentState_.Gamepads[joystickId].State.AxisState[gamepad_axis_lefty] = joystickStates_[joystickId].lY - int16Max;
	currentState_.Gamepads[joystickId].State.AxisState[gamepad_axis_rightx] = joystickStates_[joystickId].lZ - int16Max;
	currentState_.Gamepads[joystickId].State.AxisState[gamepad_axis_righty] = joystickStates_[joystickId].lRz - int16Max;
	currentState_.Gamepads[joystickId].State.AxisState[gamepad_axis_left_trigger] = joystickStates_[joystickId].lRx - int16Max;
	currentState_.Gamepads[joystickId].State.AxisState[gamepad_axis_right_trigger] = joystickStates_[joystickId].lRy - int16Max;

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

	// Update mouse button state
	uint8_t newBtnState = 0;
	const BYTE* rawBtnState = mouseState_.rgbButtons;

	for (uint8_t btnId = 0; btnId < 4; ++btnId)
		newBtnState += (1 << gMouseButtonMapping[btnId]) * (rawBtnState[btnId] != 0);

	currentState_.MouseState.BtnState = newBtnState;

	// Update mouse vel and position
	Err err = UpdateMouseVelPos(mouseState_.lX, mouseState_.lY, mouseState_.lZ);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err DILayer::UpdateMouseVelPos(const int32_t xVel, const int32_t yVel, const int32_t zVel)
{
	currentState_.MouseState.XVel = static_cast<float>(xVel);
	currentState_.MouseState.YVel = static_cast<float>(yVel);

	currentState_.MouseState.XPos += currentState_.MouseState.XVel;
	currentState_.MouseState.YPos += currentState_.MouseState.YVel;

	// Divide wheel vel by 120 for consistency with SDL3
	currentState_.MouseState.WheelYVel = static_cast<float>(zVel) / DI_WHEEL_BASE_VEL;
	currentState_.MouseState.WheelXVel = 0;

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

	// Create device and push it to the list
	Device newDevice;
	instance_->dInput_->CreateDevice(instance->guidInstance, &newDevice.Data, nullptr);
	instance_->joysticks_.push_back(newDevice);

	// Push device states
	instance_->joystickStates_.emplace_back();
	instance_->currentState_.Gamepads.emplace_back();

	return DIENUM_CONTINUE;
}

BOOL DILayer::EnumObjectsCallback(const LPCDIDEVICEOBJECTINSTANCE object, LPVOID pContext)
{
	if (device_ == nullptr)
		return DIENUM_CONTINUE;

	/*if (!(object->dwType & DIDFT_BUTTON))
		return DIENUM_CONTINUE;*/

	char deviceName[256];
	size_t converted;
	wcstombs_s(&converted, deviceName, object->tszName, 256);
	std::cout << " Name: " << deviceName << "\n";

	device_->ObjectNames.emplace_back(deviceName);
	return DIENUM_CONTINUE;
}


DILayer* DILayer::instance_ = nullptr;
Device* DILayer::device_ = nullptr;