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
			GameConsoleManager::PrintError(err);
	}

	eventsToFlush_.clear();

	return error_const::SUCCESS;
}