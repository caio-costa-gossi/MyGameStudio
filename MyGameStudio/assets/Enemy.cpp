#include "GameObject.h"
#include "GameConsoleManager.h"

class Enemy : public GameObject
{
private:
	Vec2I64 speed_ = Vec2I64(0,0);

public:
	void Update(uint64_t delta) override
	{
		GameConsoleManager::PrintInfo("EnemyPos: (" + std::to_string(pos_.X) + "," + std::to_string(pos_.Y) + ")");

		pos_.X += speed_.X * (delta >> 10);
		pos_.Y += speed_.Y * (delta >> 10);
	}

	void OnGamepadChange(const CallbackData* data) override
	{
		Event event = data->Event;

		if (event.Type == event_gamepad_button_release)
			speed_ = Vec2I64(0,0);

		else if (event.Type == event_gamepad_button_press)
		{
			switch (event.GamepadButton.Button)
			{
			case gamepad_button_dpad_up:
				speed_ = Vec2I64(0,-1);
				break;
			case gamepad_button_dpad_down:
				speed_ = Vec2I64(0,1);
				break;
			case gamepad_button_dpad_left:
				speed_ = Vec2I64(-1,0);
				break;
			case gamepad_button_dpad_right:
				speed_ = Vec2I64(1,0);
				break;
			default:
				break;
			}
		}
	}

	void OnKeyboardChange(const CallbackData* data) override
	{
		
	}

	void OnMouseChange(const CallbackData* data) override
	{
		
	}
};