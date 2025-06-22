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

	uint32_t subscriptionId_;
	EventClass classFilter_ = event_class_generic;

public:
	Subscription() = default;
	explicit Subscription(const std::function<void(const CallbackData*)>& callback, const EventClass& classFilter = event_class_generic, uint32_t subscriptionId = 0, uint8_t priority = 1, bool oneShot = false);

	bool operator< (const Subscription& other) const;

	[[nodiscard]]
	bool IsOneShot() const;

	[[nodiscard]]
	uint32_t GetSubscriptionId() const;

	[[nodiscard]]
	const EventClass& GetClassFilter() const;

	[[nodiscard]]
	Err ExecuteCallback() const;
};
