#pragma once
#include <cstdint>
#include <vector>

#include "Err.h"
#include "Event.h"

class DIEventLayer
{
private:


public:
	static Err AddJoystickBtnEvent(uint8_t joystickId, uint32_t oldBtnState, uint32_t newBtnState, std::vector<Event>& eventList);
	static Err AddJoystickAxisEvent(uint8_t joystickId, uint32_t oldBtnState, uint32_t newBtnState, std::vector<Event>& eventList);
};
