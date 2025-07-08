#include "InputManager.h"
#include <windows.h>
#include "DILayer.h"
#include "GameConsoleManager.h"
#include "NumericUtils.h"
#include "SDLInputLayer.h"
#include "WindowManager.h"

Err InputManager::Startup(const HWND hWindow, const char* deadzoneArg, const char* usingSdlArg)
{
	if (!WindowManager::IsInit())
		return error_const::WINDOW_MANAGER_NOT_STARTED;

	// Parse string args
	int32_t deadzone;
	bool useSdl;
	Err err = ParseInputArgs(usingSdlArg, deadzoneArg, useSdl, deadzone);
	if (err.Code())
		return err;

	usingSdl_ = useSdl;

	// Define input layer
	if (useSdl)
		inputLayer_ = new SDLInputLayer();
	else
		inputLayer_ = new DILayer();

	// Startup instance
	err = inputLayer_->Startup(hWindow, deadzone);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::Update()
{
	uint32_t numEvent = 0;
	const SDL_Event* eventList = nullptr;

	if (usingSdl_)
		eventList = WindowManager::GetEventList(numEvent);

	Err err = inputLayer_->Update(eventList, numEvent);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::Shutdown()
{
	Err err = inputLayer_->Shutdown();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::SubForInputEvent(const Subscription& sub)
{
	inputLayer_->Subscribe(sub);
	return error_const::SUCCESS;
}

const InputState& InputManager::GetInputState()
{
	return inputLayer_->GetInputState();
}

Err InputManager::ParseInputArgs(const char* useSdlArg, const char* deadzoneArg, bool& useSdl, int32_t& deadzone)
{
	int32_t useSdlInt;

	Err err = NumericUtils::StringToInt(useSdlArg, useSdlInt);
	if (err.Code())
		GameConsoleManager::PrintError("Invalid parameter value passed to 'useSdl'. Defaulting to 0.");

	useSdl = static_cast<bool>(useSdlInt);

	err = NumericUtils::StringToInt(deadzoneArg, deadzone);
	if (err.Code())
		GameConsoleManager::PrintError("Invalid parameter value passed to 'deadzone'. Defaulting to 0.");

	return error_const::SUCCESS;
}


InputLayer* InputManager::inputLayer_ = nullptr;
bool InputManager::usingSdl_ = false;