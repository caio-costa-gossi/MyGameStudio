#pragma once
#include <unordered_map>

#include "InputLayer.h"
#include <vector>
#include <SDL3/SDL.h>

struct SDLGamepadInterface
{
	SDL_Gamepad* Gamepad;
	SDL_GamepadType GamepadType;
	std::string GamepadName;
};

using GamepadInfo = SDLGamepadInterface;

class SDLInputLayer : public InputLayer
{
private:
	// Auxiliary structures
	std::unordered_map<uint8_t, uint8_t> idToIndex_ = std::unordered_map<uint8_t, uint8_t>();

	// Devices
	std::vector<GamepadInfo> gamepads_ = std::vector<GamepadInfo>();

	// Input type control
	uint8_t gamepadCount_ = 0;
	bool isKeyboardActive_ = true;
	bool isMouseActive_ = true;
	bool isGamepadActive_ = true;

	// Engine input structure
	InputState currentState_ = InputState();

	// Methods
	Err StartupGamepads();

	Err UpdateGamepads(const SDL_Event& event);
	Err UpdateKeyboard(const SDL_Event& event);
	Err UpdateMouse(const SDL_Event& event);

	Err UpdateGamepadButton(uint8_t gamepadId, uint8_t buttonId, bool isPressed);
	Err UpdateGamepadHat(uint8_t gamepadId, uint8_t newHatState);
	Err UpdateGamepadAxis(uint8_t gamepadId, uint8_t axisId, int16_t axisValue);

public:
	Err Startup(HWND hWindow = nullptr) override;
	Err Update() override;
	Err Shutdown() override;

	InputState GetInputStates() override;
};
