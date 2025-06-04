#include "GameObject.h"
#include "GameConsoleManager.h"

class World : public GameObject
{
public:
	void Update(uint64_t delta) override
	{
		GameConsoleManager::PrintInfo("Hello world from world! Delta: " + std::to_string(delta));
	}
};
#include "GameObjectMacro.h"
#include "GameObjectManager.h"
REGISTER_GAME_OBJECT(World)