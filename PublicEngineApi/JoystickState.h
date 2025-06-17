#pragma once
#include <cstdint>

struct JoystickState
{
	uint32_t BtnState;
	float LTrigger;
	float RTrigger;
	float LAnalogX;
	float LAnalogY;
	float RAnalogX;
	float RAnalogY;
};
