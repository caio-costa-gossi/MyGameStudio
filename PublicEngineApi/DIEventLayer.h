#pragma once
#include <vector>

#include "Err.h"
#include "Event.h"
#include "InputState.h"

class DIEventLayer
{
private:
	static Err BuildGamepadBtnEvent();
	static Err BuildGamepadAxisEvent(int32_t deadzone);
	static Err BuildKeyboardEvent();
	static Err BuildMouseBtnEvent();
	static Err BuildMouseMoveEvent();
	static Err BuildMouseWheelEvent();

	static InputState currentState_;
	static InputState nextState_;
	static std::vector<Event>* eventQueue_;

public:
	static Err AddEvents(const InputState& currentState, const InputState& nextState, std::vector<Event>* eventList, int32_t deadzone);
	
};
