#include "EventDispatcher.h"

Err EventDispatcher::FireEvent(const Event& event)
{
	for (auto it = subscriptionSet_.begin(); it != subscriptionSet_.end(); ++it)
	{
		if (it->GetClassFilter() != event.Class)
			continue;

		Err err = it->ExecuteCallback();
		if (err.Code())
			return err;

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