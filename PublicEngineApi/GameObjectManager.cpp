#include "GameObjectManager.h"

Err GameObjectManager::Startup()
{
	return error_const::SUCCESS;
}

Err GameObjectManager::Shutdown()
{
	return error_const::SUCCESS;
}

Err GameObjectManager::Update()
{
	for (GameObject* object : objectList_)
	{
		object->Update(nullptr);
	}

	return error_const::SUCCESS;
}

Err GameObjectManager::AddObject(GameObject* object)
{
	objectList_.push_back(object);
	return error_const::SUCCESS;
}


auto GameObjectManager::objectList_ = std::vector<GameObject*>();