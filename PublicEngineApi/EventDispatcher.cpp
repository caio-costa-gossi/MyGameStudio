#include "EventDispatcher.h"

Err EventDispatcher::FireEvent()
{
	for (auto it = subscriptionSet_.begin(); it != subscriptionSet_.end(); )
	{
		Err err = it->ExecuteCallback();

		if (it->IsOneShot())
			it = subscriptionSet_.erase(it);
		else
			++it;
	}

	return error_const::SUCCESS;
}

Err EventDispatcher::Subscribe(const Subscription& subscription)
{
	subscriptionSet_.insert(subscription);
	return error_const::SUCCESS;
}