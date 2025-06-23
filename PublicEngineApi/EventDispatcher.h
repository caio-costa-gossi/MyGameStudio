#pragma once
#include <vector>
#include "Err.h"
#include "Subscription.h"
#include "Timeline.h"

class EventDispatcher
{
private:
	std::vector<Subscription> subscriptionList_;
	Timeline eventTimeline_ = Timeline(timeline::NANOSECOND);

public:
	EventDispatcher();
	Err FireEvent(Event& event);
	Err Subscribe(const Subscription& subscription);
};
