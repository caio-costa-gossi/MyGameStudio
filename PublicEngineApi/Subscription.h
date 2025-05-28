#pragma once
#include <functional>
#include "CallbackData.h"
#include "Err.h"

class Subscription
{
private:
	std::function<void(const CallbackData*)> callback_;
	uint8_t priority_ = 1;
	bool oneShot_;

	std::string subscriptionName_;

public:
	Subscription() = default;
	Subscription(const std::function<void(const CallbackData*)>& callback, const std::string& name = "undefined", uint8_t priority = 1, const bool oneShot = false);

	bool operator< (const Subscription& other) const;
	bool IsOneShot() const;
	Err ExecuteCallback() const;
};
