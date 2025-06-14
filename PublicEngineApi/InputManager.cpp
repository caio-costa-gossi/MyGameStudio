#include "InputManager.h"
#include <windows.h>
#include <dinput.h>
#include <thread>

#include "GameConsoleManager.h"

Err InputManager::Startup()
{
	const HINSTANCE hModule = GetModuleHandle(nullptr);

	// Init interface
	if (FAILED(DirectInput8Create(hModule, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID*>(&dInput_), nullptr)))
		return error_const::INPUT_MOD_INIT_FAIL;

	// Enumerate joysticks and create
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

	// Create test window & set cooperative level
	HWND hWindow = nullptr;
	std::thread winThread(CreateTestWindow, std::ref(hWindow));

	while (hWindow == nullptr)
	{
		Sleep(1);
	}

	for (const Device device: joysticks_)
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
		if (FAILED(device->Acquire()))
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
	Device newDevice;
	dInput_->CreateDevice(instance->guidInstance, &newDevice, nullptr);
	joysticks_.push_back(newDevice);

	return DIENUM_CONTINUE;
}

void InputManager::CreateTestWindow(HWND& hNewWindow)
{
	const auto className = "TestWindowClass";

	WNDCLASSA wc = {};
	wc.lpfnWndProc = TestWindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = reinterpret_cast<LPCSTR>(className);

	RegisterClassA(&wc);

	// Create the window
	hNewWindow = CreateWindowExA(
		0,
		className,
		"DirectInput Test Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		nullptr, nullptr, GetModuleHandle(nullptr), nullptr
	);

	if (hNewWindow == nullptr) {
		std::cerr << "Failed to create window.\n";
		return;
	}

	ShowWindow(hNewWindow, SW_SHOW);

	// Run the message loop
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT InputManager::TestWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


LPDIRECTINPUT8 InputManager::dInput_ = nullptr;

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