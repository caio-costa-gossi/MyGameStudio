#include "TestDrawer.h"

#include "AssetRuntimeManager.h"
#include "CameraInstance.h"
#include "CameraManager.h"
#include "GameConsoleManager.h"
#include "InputManager.h"
#include "NumericUtils.h"
#include "WindowManager.h"
#include "RenderManager.h"
#include "Serialization.h"
#include "Transform.h"
#include "ZipFile.h"

#include <fstream>

#undef min

Err TestDrawer::Startup()
{
	Err err = GameConsoleManager::Startup("0", "0");
	if (err.Code())
		return err;

	err = WindowManager::Startup();
	if (err.Code())
		return err;

	err = InputManager::Startup(WindowManager::GetWindowHandle(), "2500", "1");
	if (err.Code())
		return err;

	err = CameraManager::Startup();
	if (err.Code())
		return err;

	err = RenderManager::Startup();
	if (err.Code())
		return err;

	time_.Start();

	return error_const::SUCCESS;
}

Err TestDrawer::Run()
{
	uint64_t modelSize;
	const uint8_t* meshBinaryData = AssetRuntimeManager::LoadAsset(66, modelSize);
	Model model = Serialization::DesserializeModel(meshBinaryData, modelSize);

	//const Vec3F worldPos[5] = { {0.0f, 0.0f, 0.0f}, {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
	//	{-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f} };

	const Vec3F worldPos[1] = { {0.0f, 0.0f, 0.0f} };

	CameraInstance camera(enums::perspective, 0.1f, 1000.0f, 800.0f, 400.0f);
	camera.Use();

	running_ = true;

	while (running_)
	{
		Err err = WindowManager::Update();
		if (err.Code() == error_const::EXIT_REQUEST_CODE)
			running_ = false;
		else if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::window);

		err = InputManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

		err = MoveCameraPos(camera);
		if (err.Code())
			return err;

		err = MoveCameraDirection(camera);
		if (err.Code())
			return err;

		err = LockUnlock();
		if (err.Code())
			return err;

		for (int i = 0; i < 5; ++i)
		{
			//Transform transform(worldPos[i], static_cast<float>(time_.GetElapsed()) / 50, {0.5f,1,0}, {1,1,1});
			Transform transform(worldPos[i], 0, { 0,0,1 }, { 1.0f,1.0f,1.0f });

			RenderRequest request = { &model, transform };
			err = RenderManager::RequestRender(request);
			if (err.Code())
				return err;
		}

		err = RenderManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);
	}

	return error_const::SUCCESS;
}

Err TestDrawer::Shutdown()
{
	Err err = WindowManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

	err = InputManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

	err = RenderManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);

	return error_const::SUCCESS;
}

Err TestDrawer::MoveCameraPos(CameraInstance& camera)
{
	if (!focus_)
		return error_const::SUCCESS;

	const InputState state = InputManager::GetInputState();

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_w])
		camera.Move(camera.GetPos() + camera.GetDirection() * 0.01f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_s])
		camera.Move(camera.GetPos() + camera.GetDirection() * -0.01f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_a])
		camera.Move(camera.GetPos() + camera.GetRight() * -0.01f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_d])
		camera.Move(camera.GetPos() + camera.GetRight() * 0.01f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_space])
		camera.Move(camera.GetPos() + Vec3F(0, 0.01f, 0));

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_lshift])
		camera.Move(camera.GetPos() + Vec3F(0, -0.01f, 0));

	return error_const::SUCCESS;
}

Err TestDrawer::MoveCameraDirection(CameraInstance& camera)
{
	if (!focus_)
		return error_const::SUCCESS;

	const InputState state = InputManager::GetInputState();

	camera.ChangeYaw(camera.GetYaw() + state.MouseState.XVel / 5.0f);

	float newPitch = camera.GetPitch() - state.MouseState.YVel / 5.0f;
	newPitch = std::min(newPitch, 89.0f);
	newPitch = std::max(newPitch, -89.0f);
	camera.ChangePitch(newPitch);

	return error_const::SUCCESS;
}

Err TestDrawer::LockUnlock()
{
	const InputState state = InputManager::GetInputState();

	if ((state.MouseState.BtnState & 1 << mouse_button_left) != 0)
	{
		Err err = WindowManager::SetRelativeMouseMode();
		if (err.Code())
			return err;

		focus_ = true;
	}
	else if (state.KeyboardState.PhysicalKeyState[keyboard_key_escape])
	{
		Err err = WindowManager::UnsetRelativeMouseMode();
		if (err.Code())
			return err;

		focus_ = false;
	}

	return error_const::SUCCESS;
}


bool TestDrawer::running_ = false;

float TestDrawer::rotation_ = -55.0f;
Vec3F TestDrawer::position_ = {0,0,0};
Vec3F TestDrawer::scale_ = {1,1,1};

Timeline TestDrawer::time_(timeline::MILLISECOND);
bool TestDrawer::focus_ = false;