#include "GameObjectManager.h"

GameObjectManager& GameObjectManager::Get()
{
	static GameObjectManager instance;
	return instance;
}

Err GameObjectManager::Startup()
{
	for (GameObjectFactory* factory : factoryList_)
	{
		GameObject* newObject = factory->Get();
		newObject->SetPos({ 1,1 });
		objectList_.push_back(newObject);
	}

	return error_const::SUCCESS;
}

Err GameObjectManager::Shutdown()
{
	return error_const::SUCCESS;
}

void GameObjectManager::Update(const uint64_t delta) const
{
	for (GameObject* object : objectList_)
	{
		object->Update(delta);
	}
}

Err GameObjectManager::AddFactory(GameObjectFactory* factory)
{
	factoryList_.push_back(factory);
	return error_const::SUCCESS;
}