#include "InputLayer.h"

InputState InputLayer::GetInputState()
{
	return currentState_;
}

Err InputLayer::Subscribe(const Subscription& subscription)
{
	return eventDispatcher_.Subscribe(subscription);
}
