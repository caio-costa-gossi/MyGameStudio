#include "GameObjectManager.h"

#include "InputManager.h"

GameObjectManager& GameObjectManager::Get()
{
	static GameObjectManager instance;
	return instance;
}

Err GameObjectManager::Startup()
{
	// Startup all game objects
	for (GameObjectFactory* factory : factoryList_)
	{
		GameObject* newObject = factory->Get();
		newObject->SetPos({ 1,1 });
		objectList_.push_back(newObject);
	}

	// Register all game objects callbacks

	// Input
	for (GameObject* object : objectList_)
	{
		auto gamepadCallback = [object](const CallbackData* data)
			{
				object->OnGamepadChange(data);
			};
		const Subscription gamepadSub(gamepadCallback, event_class_gamepad);
		InputManager::SubForInputEvent(gamepadSub);

		auto keyboardCallback = [object](const CallbackData* data)
			{
				object->OnKeyboardChange(data);
			};
		const Subscription keyboardSub(keyboardCallback, event_class_keyboard);
		InputManager::SubForInputEvent(keyboardSub);

		auto mouseCallback = [object](const CallbackData* data)
			{
				object->OnMouseChange(data);
			};
		const Subscription mouseSub(mouseCallback, event_class_mouse);
		InputManager::SubForInputEvent(mouseSub);
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