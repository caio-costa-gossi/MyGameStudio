#pragma once
#include <Windows.h>
#include <SDL3/SDL_events.h>
#include "Err.h"
#include "EventDispatcher.h"
#include "InputState.h"

class InputLayer
{
protected:
	InputState currentState_ = InputState();
	EventDispatcher eventDispatcher_;
	std::vector<Event> eventsToFlush_ = std::vector<Event>();

	int32_t deadzone_ = 500;

	uint8_t gamepadCount_ = 0;
	bool isKeyboardActive_ = true;
	bool isMouseActive_ = true;
	bool isGamepadActive_ = true;

	Err FireEvents();

public:
	virtual Err Startup(HWND hWindow = nullptr, int32_t deadzone = 500) = 0;
	virtual Err Update(const SDL_Event* eventList = nullptr, uint32_t numEvent = 0) = 0;
	virtual Err Shutdown() = 0;

	const InputState& GetInputState();
	Err Subscribe(const Subscription& subscription);

	[[nodiscard]]
	bool IsGamepadEnabled() const;

	[[nodiscard]]
	bool IsMouseEnabled() const;

	[[nodiscard]]
	bool IsKeyboardEnabled() const;

	virtual ~InputLayer() = default;
};
