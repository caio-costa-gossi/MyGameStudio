#pragma once
#include <vector>
#include "Err.h"
#include "GameObject.h"

class GameObjectManager
{
private:
	static std::vector<GameObject*> objectList_;

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err AddObject(GameObject* object);
};
