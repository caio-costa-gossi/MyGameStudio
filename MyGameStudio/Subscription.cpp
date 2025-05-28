#include "Subscription.h"

Subscription::Subscription(const std::function<void(const CallbackData*)>& callback, const std::string& name, const uint8_t priority, const bool oneShot) :
	callback_(callback), subscriptionName_(name), priority_(priority), oneShot_(oneShot)
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

Err Subscription::ExecuteCallback() const
{
	callback_(nullptr);
	return error_const::SUCCESS;
}
