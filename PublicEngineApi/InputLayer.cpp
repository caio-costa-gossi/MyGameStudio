#include "InputLayer.h"

#include "GameConsoleManager.h"

const InputState& InputLayer::GetInputState()
{
	return currentState_;
}

Err InputLayer::Subscribe(const Subscription& subscription)
{
	return eventDispatcher_.Subscribe(subscription);
}

Err InputLayer::FireEvents()
{
	for (Event& event : eventsToFlush_)
	{
		Err err = eventDispatcher_.FireEvent(event);
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);
	}

	eventsToFlush_.clear();

	return error_const::SUCCESS;
}

bool InputLayer::IsGamepadEnabled() const
{
	return isGamepadActive_;
}

bool InputLayer::IsMouseEnabled() const
{
	return isMouseActive_;
}

bool InputLayer::IsKeyboardEnabled() const
{
	return isKeyboardActive_;
}
