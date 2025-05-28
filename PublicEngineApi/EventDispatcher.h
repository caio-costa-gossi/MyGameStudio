#pragma once
#include <set>
#include "Err.h"
#include "Subscription.h"

class EventDispatcher
{
private:
	std::set<Subscription> subscriptionSet_;

public:
	Err FireEvent();
	Err Subscribe(const Subscription& subscription);
};
