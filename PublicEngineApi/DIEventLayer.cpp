#include "DIEventLayer.h"

#include "DILayerMapping.h"
#include "NumericUtils.h"

Err DIEventLayer::AddEvents(const InputState& currentState, const InputState& nextState, std::vector<Event>* eventList)
{
	currentState_ = currentState;
	nextState_ = nextState;
	eventQueue_ = eventList;

	eventQueue_->clear();

	Err err = BuildGamepadBtnEvent();
	if (err.Code())
		return err;

	err = BuildGamepadAxisEvent();
	if (err.Code())
		return err;

	err = BuildKeyboardEvent();
	if (err.Code())
		return err;

	err = BuildMouseBtnEvent();
	if (err.Code())
		return err;

	err = BuildMouseMoveEvent();
	if (err.Code())
		return err;

	err = BuildMouseWheelEvent();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err DIEventLayer::BuildGamepadBtnEvent()
{
	for (uint8_t joyId = 0; joyId < currentState_.GamepadCount; ++joyId)
	{
		const uint32_t curBtn = currentState_.Gamepads[joyId].State.BtnState;
		const uint32_t nextBtn = nextState_.Gamepads[joyId].State.BtnState;
		const uint32_t btnDiff = curBtn ^ nextBtn;

		if (btnDiff == 0)
			continue;

		for (uint8_t btnId = 0; btnId < gamepad_button_count; ++btnId)
		{
			const uint32_t btnMask = 1 << btnId;

			if ((btnDiff & btnMask) == 0)
				continue;

			// Build event
			const bool isPressed = (nextBtn & btnMask) != 0;
			const GamepadButtonEvent btnEvent = { static_cast<GamepadButton>(btnId), isPressed, joyId };
			Event newEvent = { isPressed ? event_gamepad_button_press : event_gamepad_button_release, event_class_gamepad, 0 };
			newEvent.GamepadButton = btnEvent;
			eventQueue_->emplace_back(newEvent);
		}
	}

	return error_const::SUCCESS;
}

Err DIEventLayer::BuildGamepadAxisEvent()
{
	for (uint8_t joyId = 0; joyId < currentState_.GamepadCount; ++joyId)
	{
		const int32_t* currentAxes = currentState_.Gamepads[joyId].State.AxisState;
		const int32_t* nextAxes = nextState_.Gamepads[joyId].State.AxisState;

		for (uint8_t axisId = 0; axisId < gamepad_axis_count; ++axisId)
		{
			if (currentAxes[axisId] == nextAxes[axisId])
				continue;

			// Build event
			const GamepadAxisEvent axisEvent = { static_cast<GamepadAxis>(axisId), nextAxes[axisId], joyId };
			Event newEvent = { event_gamepad_axis_move, event_class_gamepad, 0 };
			newEvent.GamepadAxis = axisEvent;
			eventQueue_->emplace_back(newEvent);
		}
	}

	return error_const::SUCCESS;
}

Err DIEventLayer::BuildKeyboardEvent()
{
	const bool* currentStates = currentState_.KeyboardState.RawState;
	const bool* nextStates = nextState_.KeyboardState.RawState;

	for (uint16_t rawId = 0; rawId < scancode_key_count; ++rawId)
	{
		if (currentStates[rawId] == nextStates[rawId])
			continue;

		const uint16_t physicalId = gWinScancodeToUsbScancode[rawId];
		const bool isPressed = nextStates[rawId];

		// Build event
		const KeyboardEvent kbEvent = { static_cast<KeyboardKey>(physicalId), static_cast<ScancodeKey>(rawId), isPressed };
		Event newEvent = { isPressed ? event_keyboard_button_press : event_keyboard_button_release, event_class_keyboard, 0 };
		newEvent.Keyboard = kbEvent;
		eventQueue_->emplace_back(newEvent);
	}

	return error_const::SUCCESS;
}

Err DIEventLayer::BuildMouseBtnEvent()
{
	const uint8_t curBtn = currentState_.MouseState.BtnState;
	const uint8_t nextBtn = nextState_.MouseState.BtnState;
	const uint8_t btnDiff = curBtn ^ nextBtn;

	if (btnDiff == 0)
		return error_const::SUCCESS;

	for (uint8_t btnId = 0; btnId < mouse_button_count; ++btnId)
	{
		const uint8_t btnMask = 1 << btnId;

		if ((btnDiff & btnMask) == 0)
			continue;

		// Build event
		const bool isPressed = (nextBtn & btnMask) != 0;
		const MouseButtonEvent btnEvent = { static_cast<MouseButton>(btnId), isPressed };
		Event newEvent = { isPressed ? event_mouse_button_press : event_mouse_button_release, event_class_mouse, 0 };
		newEvent.MouseButton = btnEvent;
		eventQueue_->emplace_back(newEvent);
	}

	return error_const::SUCCESS;
}

Err DIEventLayer::BuildMouseMoveEvent()
{
	const float curXPos = currentState_.MouseState.XPos;
	const float nextXPos = nextState_.MouseState.XPos;
	const float curYPos = currentState_.MouseState.YPos;
	const float nextYPos = nextState_.MouseState.YPos;

	if (NumericUtils::FloatEqual(curXPos, nextXPos, 1e-5f) && NumericUtils::FloatEqual(curYPos, nextYPos, 1e-5f))
		return error_const::SUCCESS;

	// Build event
	const MouseMoveEvent moveEvent = { nextXPos, nextYPos, nextState_.MouseState.XVel, nextState_.MouseState.YVel };
	Event newEvent = { event_mouse_move, event_class_mouse, 0 };
	newEvent.MouseMove = moveEvent;
	eventQueue_->emplace_back(newEvent);

	return error_const::SUCCESS;
}

Err DIEventLayer::BuildMouseWheelEvent()
{
	const float nextWheelX = nextState_.MouseState.WheelXVel;
	const float nextWheelY = nextState_.MouseState.WheelYVel;

	if (NumericUtils::FloatEqual(nextWheelX, 0, 1e-5f) && NumericUtils::FloatEqual(nextWheelY, 0, 1e-5f))
		return error_const::SUCCESS;

	// Build event
	const MouseWheelEvent wheelEvent = { nextWheelX, nextWheelY };
	Event newEvent = { event_mouse_wheel, event_class_mouse, 0 };
	newEvent.MouseWheel = wheelEvent;
	eventQueue_->emplace_back(newEvent);

	return error_const::SUCCESS;
}

InputState DIEventLayer::currentState_;
InputState DIEventLayer::nextState_;
std::vector<Event>* DIEventLayer::eventQueue_ = nullptr;