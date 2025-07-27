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

	// Methods
	Err StartupGamepads();

	Err UpdateGamepads(const SDL_Event& event);
	Err UpdateKeyboard(const SDL_Event& event);
	Err UpdateMouse(const SDL_Event& event);

	Err UpdateGamepadButton(uint8_t gamepadId, uint8_t buttonId, bool isPressed);
	Err UpdateGamepadAxis(uint8_t gamepadId, uint8_t axisId, int16_t axisValue);
	Err UpdateMouseButton(uint8_t buttonIndex, bool isPressed);
	Err UpdateMouseMotion(const SDL_MouseMotionEvent& event);
	Err UpdateMouseWheel(const SDL_MouseWheelEvent& event);

public:
	Err Startup(HWND hWindow = nullptr, int32_t deadzone = 500) override;
	Err Update(const SDL_Event* eventList = nullptr, uint32_t numEvent = 0) override;
	Err Shutdown() override;
};
