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

	Err FireEvents();

public:
	virtual Err Startup(HWND hWindow = nullptr) = 0;
	virtual Err Update(const SDL_Event* eventList = nullptr, uint32_t numEvent = 0) = 0;
	virtual Err Shutdown() = 0;

	InputState GetInputState();
	Err Subscribe(const Subscription& subscription);

	virtual ~InputLayer() = default;
};
