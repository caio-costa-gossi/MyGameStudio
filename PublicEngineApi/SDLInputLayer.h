#pragma once
#include "InputLayer.h"
#include <vector>
#include <SDL3/SDL.h>

struct SDLJoystickInterface
{
	SDL_Joystick* Joystick;
	const char* JoystickName;
};

using Joystick = SDLJoystickInterface;

class SDLInputLayer : public InputLayer
{
private:
	// Devices
	std::vector<Joystick> joysticks_ = std::vector<Joystick>();

	// Input type control
	uint8_t joystickCount_ = 0;
	bool isKeyboardActive_ = true;
	bool isMouseActive_ = true;
	bool isJoystickActive_ = true;

	Err StartupJoysticks();

public:
	Err Startup(HWND hWindow = nullptr) override;
	Err Update() override;
	Err Shutdown() override;

	InputState GetInputStates() override;
};
