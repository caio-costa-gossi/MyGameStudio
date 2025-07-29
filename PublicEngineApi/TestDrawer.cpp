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

#include "LightingManager.h"

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

	err = LightingManager::Startup();
	if (err.Code())
		return err;

	err = RenderManager::Startup();
	if (err.Code())
		return err;

	time_.Start();
	err = LightingManager::AddLightSource({ 1.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 1.0f, lightSrcId_);
	if (err.Code())
		return err;

	//LightingManager::SetAmbientLight(0.4f, { 1.0f, 1.0f, 1.0f });

	return error_const::SUCCESS;
}

Err TestDrawer::Run()
{
	uint64_t modelSize;
	const uint8_t* mesh1BinData = AssetRuntimeManager::LoadAsset(60, modelSize);
	Model model = Serialization::DesserializeModel(mesh1BinData, modelSize);

	const uint8_t* mesh2BinData = AssetRuntimeManager::LoadAsset(62, modelSize);
	Model model2 = Serialization::DesserializeModel(mesh2BinData, modelSize);

	const uint8_t* mesh3BinData = AssetRuntimeManager::LoadAsset(108, modelSize);
	Model model3 = Serialization::DesserializeModel(mesh3BinData, modelSize);

	//const Vec3F worldPos[5] = { {0.0f, 0.0f, 0.0f}, {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
	//	{-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f} };

	const Vec3F worldPos[3] = { {0.0f, 0.0f, 5.0f}, {2.0f, 0.0f, 0.0f}, {6.0f, 0.0f, 0.0f} };

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

		err = CameraManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::camera);

		UpdateLightSource();

		err = LightingManager::DrawLightSources();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);

		//Transform transform(worldPos[i], static_cast<float>(time_.GetElapsed()) / 50, {0.5f,1,0}, {1,1,1});
		//Transform transform({ 0,0,0 }, 0, { 0,0,1 }, { 0.1f,0.1f,0.1f });

		RenderRequest request = { &model,Transform(worldPos[0], 0, {0,0,1}, {1.0f,1.0f,1.0f})};
		err = RenderManager::RequestRender(request);
		if (err.Code())
			return err;

		RenderRequest request2 = { &model2, Transform(worldPos[1], 0, {0,0,1}, {1.0f,1.0f,1.0f}) };
		err = RenderManager::RequestRender(request2);
		if (err.Code())
			return err;

		RenderRequest request3 = { &model3, Transform(worldPos[2], 0, {0,0,1}, {0.01f,0.01f,0.01f}) };
		err = RenderManager::RequestRender(request3);
		if (err.Code())
			return err;

		BillboardRenderRequest billboardRequest = { {{-2,0,0}, {1.0f,1.0f}, 145} };
		err = RenderManager::RequestBillboardRender(billboardRequest);
		if (err.Code())
			return err;

		BillboardRenderRequest billboardRequest2 = { {{-3,0,0}, {1.0f,1.0f}, 146} };
		err = RenderManager::RequestBillboardRender(billboardRequest2);
		if (err.Code())
			return err;

		BillboardRenderRequest billboardRequest3 = { {{-4,0,0}, {1.0f,1.0f}, 147} };
		err = RenderManager::RequestBillboardRender(billboardRequest3);
		if (err.Code())
			return err;

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

void TestDrawer::UpdateLightSource()
{
	const InputState& state = InputManager::GetInputState();
	LightSource* source = LightingManager::GetLightSource(lightSrcId_);
	if (source == nullptr)
	{
		GameConsoleManager::PrintError("Light source not found!");
		return;
	}

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_z])
		source->Pos.Y += 0.01f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_x])
		source->Pos.Y -= 0.01f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_left])
		source->Pos.Z += 0.01f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_right])
		source->Pos.Z -= 0.01f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_up])
		source->Pos.X += 0.01f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_down])
		source->Pos.X -= 0.01f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_kp_plus])
		source->Intensity += 0.005f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_kp_minus])
		if (source->Intensity >= 0.005f) source->Intensity -= 0.005f;

	// Red
	if (state.KeyboardState.PhysicalKeyState[keyboard_key_u])
		if (source->Color.R <= 0.95f) source->Color.R += 0.005f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_i])
		if (source->Color.R >= 0.05f) source->Color.R -= 0.005f;

	// Green
	if (state.KeyboardState.PhysicalKeyState[keyboard_key_j])
		if (source->Color.G <= 0.95f) source->Color.G += 0.005f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_k])
		if (source->Color.G >= 0.05f) source->Color.G -= 0.005f;

	// Blue
	if (state.KeyboardState.PhysicalKeyState[keyboard_key_n])
		if (source->Color.B <= 0.95f) source->Color.B += 0.005f;

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_m])
		if (source->Color.B >= 0.05f) source->Color.B -= 0.005f;
}



bool TestDrawer::running_ = false;

float TestDrawer::rotation_ = -55.0f;
Vec3F TestDrawer::position_ = {0,0,0};
Vec3F TestDrawer::scale_ = {1,1,1};

Timeline TestDrawer::time_(timeline::MILLISECOND);

uint32_t TestDrawer::lightSrcId_ = 0;