#pragma once
#include <vector>
#include "Err.h"
#include "GameObject.h"
#include "GameObjectFactory.h"

class GameObjectManager
{
private:
	std::vector<GameObject*> objectList_ = std::vector<GameObject*>();
	std::vector<GameObjectFactory*> factoryList_ = std::vector<GameObjectFactory*>();

	GameObjectManager() = default;

public:
	static GameObjectManager& Get();

	Err Startup();
	Err Update(uint64_t delta) const;
	Err Shutdown();

	Err AddFactory(GameObjectFactory* factory);
};
