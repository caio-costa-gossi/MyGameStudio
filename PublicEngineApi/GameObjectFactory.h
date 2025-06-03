#pragma once
#include "GameObject.h"

class GameObjectFactory
{
public:
	virtual GameObject* Get() = 0;
	virtual ~GameObjectFactory() = default;
};
