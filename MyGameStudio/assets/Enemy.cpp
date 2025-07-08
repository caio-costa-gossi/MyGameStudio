#include "GameObject.h"
#include "GameConsoleManager.h"
#include "InputManager.h"

class Enemy : public GameObject
{
private:
	Vec2I64 speed_ = {0,0};

public:
	void Update(uint64_t delta) override
	{
		//GameConsoleManager::PrintInfo("EnemyPos: (" + std::to_string(pos_.X) + "," + std::to_string(pos_.Y) + ")");
		pos_ += speed_ * (delta >> 10);
	}

	void OnGamepadChange(const CallbackData* data) override
	{
		Event event = data->Event;

		if (event.Type == event_gamepad_button_release)
			speed_ = {0,0};

		else if (event.Type == event_gamepad_button_press)
		{
			switch (event.GamepadButton.Button)
			{
			case gamepad_button_dpad_up:
				speed_ += {0,-1};
				break;
			case gamepad_button_dpad_down:
				speed_ += {0,1};
				break;
			case gamepad_button_dpad_left:
				speed_ += {-1,0};
				break;
			case gamepad_button_dpad_right:
				speed_ += {1,0};
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