#include "GameObject.h"
#include "GameConsoleManager.h"

class Enemy : public GameObject
{
public:
	void Update(uint64_t delta) override
	{
		//GameConsoleManager::PrintInfo("Hello world from enemy! Delta: " + std::to_string(delta));
	}

	void OnGamepadChange(const CallbackData* data) override
	{
		GameConsoleManager::PrintInfo("Gamepad changed!");
	}

	void OnKeyboardChange(const CallbackData* data) override
	{
		GameConsoleManager::PrintInfo("Keyboard changed!");
	}

	void OnMouseChange(const CallbackData* data) override
	{
		GameConsoleManager::PrintInfo("Mouse changed!");
	}
};