#pragma once
#include <set>
#include "Err.h"
#include "Subscription.h"
#include "Timeline.h"

class EventDispatcher
{
private:
	std::set<Subscription> subscriptionSet_;
	Timeline eventTimeline_ = Timeline(timeline::NANOSECOND);

public:
	EventDispatcher();
	Err FireEvent(Event& event);
	Err Subscribe(const Subscription& subscription);
};
