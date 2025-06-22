#include "Subscription.h"

Subscription::Subscription(const std::function<void(const CallbackData*)>& callback, const EventClass& classFilter, const uint32_t subscriptionId, const uint8_t priority, const bool oneShot) :
	callback_(callback), classFilter_(classFilter), subscriptionId_(subscriptionId), priority_(priority), oneShot_(oneShot)
{
	
}

bool Subscription::operator<(const Subscription& other) const
{
	return priority_ < other.priority_;
}

bool Subscription::IsOneShot() const
{
	return oneShot_;
}

uint32_t Subscription::GetSubscriptionId() const
{
	return subscriptionId_;
}

const EventClass& Subscription::GetClassFilter() const
{
	return classFilter_;
}

Err Subscription::ExecuteCallback(const CallbackData& data) const
{
	callback_(&data);
	return error_const::SUCCESS;
}
