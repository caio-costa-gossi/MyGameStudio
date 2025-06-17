#pragma once
#include "InputLayer.h"
#include <vector>
#include <SDL3/SDL.h>

struct SDLGamepadInterface
{
	SDL_Gamepad* Gamepad;
	SDL_GamepadType GamepadType;
	const char* GamepadName;
};

using GamepadInfo = SDLGamepadInterface;

class SDLInputLayer : public InputLayer
{
private:
	// Devices
	std::vector<GamepadInfo> gamepads_ = std::vector<GamepadInfo>();

	// Input type control
	uint8_t gamepadCount_ = 0;
	bool isKeyboardActive_ = true;
	bool isMouseActive_ = true;
	bool isGamepadActive_ = true;

	// Engine input structure
	InputState currentState_;

	Err StartupGamepads();

public:
	Err Startup(HWND hWindow = nullptr) override;
	Err Update() override;
	Err Shutdown() override;

	InputState GetInputStates() override;
};
