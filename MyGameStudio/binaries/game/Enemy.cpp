#include "GameObject.h"
#include "GameConsoleManager.h"

class Enemy : public GameObject
{
public:
	void Update(uint64_t delta) override
	{
		GameConsoleManager::PrintInfo("Hello world from enemy! My coords are: (" + std::to_string(xPos_) + "," + std::to_string(yPos_) + ")");
	}
};

#include "GameObjectMacro.h"
#include "GameObjectManager.h" 
REGISTER_GAME_OBJECT(Enemy)