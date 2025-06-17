#pragma once
#include <Windows.h>
#include "Err.h"
#include "InputState.h"

class InputLayer
{
public:
	virtual Err Startup(HWND hWindow = nullptr) = 0;
	virtual Err Update() = 0;
	virtual Err Shutdown() = 0;

	virtual InputState GetInputStates() = 0;

	virtual ~InputLayer() = default;
};
