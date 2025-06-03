#pragma once
#include "CallbackData.h"

class GameObject
{
private:


public:
	virtual ~GameObject();

	virtual void OnCreation(const CallbackData* data) = 0;
	virtual void Update(uint64_t delta) = 0;
	virtual void OnDestruction(const CallbackData* data) = 0;
};
