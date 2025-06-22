#include "EventDispatcher.h"

EventDispatcher::EventDispatcher()
{
	eventTimeline_.Start();
}

Err EventDispatcher::FireEvent(Event& event)
{
	for (auto it = subscriptionSet_.begin(); it != subscriptionSet_.end(); ++it)
	{
		// Check subscription filter
		if (it->GetClassFilter() != event.Class)
			continue;

		// Execute callback
		event.Timestamp = eventTimeline_.GetElapsed();
		CallbackData data = { event };

		Err err = it->ExecuteCallback(data);
		if (err.Code())
			return err;

		// Check OneShot
		if (it->IsOneShot())
			it = subscriptionSet_.erase(it);
	}

	return error_const::SUCCESS;
}

Err EventDispatcher::Subscribe(const Subscription& subscription)
{
	subscriptionSet_.insert(subscription);
	return error_const::SUCCESS;
}