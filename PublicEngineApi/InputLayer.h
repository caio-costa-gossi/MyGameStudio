#pragma once
#include <Windows.h>
#include <SDL3/SDL_events.h>
#include "Err.h"
#include "InputState.h"

class InputLayer
{
public:
	virtual Err Startup(HWND hWindow = nullptr) = 0;
	virtual Err Update(const SDL_Event* eventList = nullptr, uint32_t numEvent = 0) = 0;
	virtual Err Shutdown() = 0;

	virtual InputState GetInputStates() = 0;

	virtual ~InputLayer() = default;
};
