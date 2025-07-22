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
	/*Vertex* vertices = new Vertex[8];

	vertices[0] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }; // bottom, low left
	vertices[1] = { {0.5f, -0.5f, -0.5f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }; // bottom, low right
	vertices[2] = { {0.5f,  0.5f, -0.5f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }; // bottom, up right
	vertices[3] = { {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }; // bottom, up left

	vertices[4] = { {-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }; // top, low left
	vertices[5] = { {0.5f, -0.5f,  0.5f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }; // top, low right
	vertices[6] = { {0.5f,  0.5f,  0.5f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }; // top, up right
	vertices[7] = { {-0.5f,  0.5f,  0.5f},{1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }; // top, up left

	uint32_t* indices = new uint32_t[36];

	indices[0] = 4;
	indices[1] = 5;
	indices[2] = 6;
	indices[3] = 4;
	indices[4] = 6;
	indices[5] = 7;
	indices[6] = 0;
	indices[7] = 2;
	indices[8] = 1;
	indices[9] = 0;
	indices[10] = 3;
	indices[11] = 2;
	indices[12] = 0;
	indices[13] = 4;
	indices[14] = 7;
	indices[15] = 0;
	indices[16] = 7;
	indices[17] = 3;
	indices[18] = 1;
	indices[19] = 2;
	indices[20] = 6;
	indices[21] = 1;
	indices[22] = 6;
	indices[23] = 5;
	indices[24] = 0;
	indices[25] = 1;
	indices[26] = 5;
	indices[27] = 0;
	indices[28] = 5;
	indices[29] = 4;
	indices[30] = 3;
	indices[31] = 7;
	indices[32] = 6;
	indices[33] = 3;
	indices[34] = 6;
	indices[35] = 2;

	testMesh_ = { 1, std::unique_ptr<Vertex[]>(vertices), 8, std::unique_ptr<uint32_t[]>(indices), 36, 47 };*/

	uint64_t meshSize;
	const uint8_t* meshBinaryData = AssetRuntimeManager::LoadAsset(49, meshSize);
	testMesh_ = Serialization::DesserializeMesh(meshBinaryData, meshSize);

	GameConsoleManager::PrintInfo("Mesh vertices: ");
	for (uint32_t i = 0; i < testMesh_.VertexCount; ++i)
	{
		Vertex vertex = testMesh_.VertexList.get()[i];
		GameConsoleManager::PrintInfo(std::to_string(vertex.Pos.X) + "," + std::to_string(vertex.Pos.Y) + "," + std::to_string(vertex.Pos.Z));
	}

	for (uint32_t i = 0; i < testMesh_.IndexCount; ++i)
	{
		uint32_t index = testMesh_.IndexList.get()[i];
		GameConsoleManager::PrintInfo(std::to_string(index));
	}

	/*const Vec3F worldPos[5] = { {0.0f, 0.0f, 0.0f}, {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f} };*/

	const Vec3F worldPos[1] = { {0.0f, 0.0f, 0.0f} };

	CameraInstance camera(enums::perspective, 0.1f, 10000);
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
			Transform model(worldPos[i], static_cast<float>(time_.GetElapsed()) / 50, {0.5f,1,0}, {1,1,1});

			RenderRequest request = { &testMesh_, model };
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
		camera.Move(camera.GetPos() + camera.GetDirection() * 1.0f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_s])
		camera.Move(camera.GetPos() + camera.GetDirection() * -1.0f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_a])
		camera.Move(camera.GetPos() + camera.GetRight() * -1.0f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_d])
		camera.Move(camera.GetPos() + camera.GetRight() * 1.0f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_space])
		camera.Move(camera.GetPos() + Vec3F(0, 1.0f, 0));

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_lshift])
		camera.Move(camera.GetPos() + Vec3F(0, -1.0f, 0));

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
Mesh TestDrawer::testMesh_;

float TestDrawer::rotation_ = -55.0f;
Vec3F TestDrawer::position_ = {0,0,0};
Vec3F TestDrawer::scale_ = {1,1,1};

Timeline TestDrawer::time_(timeline::MILLISECOND);
bool TestDrawer::focus_ = false;