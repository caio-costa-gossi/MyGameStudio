#include "EventDispatcher.h"

#include "GameConsoleManager.h"

EventDispatcher::EventDispatcher()
{
	eventTimeline_.Start();
}

Err EventDispatcher::FireEvent(Event& event)
{
	for (auto it = subscriptionList_.begin(); it != subscriptionList_.end(); ++it)
	{
		// Check subscription filter
		if (it->GetClassFilter() != event.Class)
			continue;

		// Execute callback
		event.Timestamp = eventTimeline_.GetElapsed();
		CallbackData data = { event };

		Err err = it->ExecuteCallback(data);
		if (err.Code())
			GameConsoleManager::PrintError(err);

		// Check OneShot
		if (it->IsOneShot())
			it = subscriptionList_.erase(it);
	}

	return error_const::SUCCESS;
}

Err EventDispatcher::Subscribe(const Subscription& subscription)
{
	subscriptionList_.push_back(subscription);
	return error_const::SUCCESS;
}