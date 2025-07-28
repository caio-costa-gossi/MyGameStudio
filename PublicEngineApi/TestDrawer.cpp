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
	const uint8_t* mesh1BinData = AssetRuntimeManager::LoadAsset(60, modelSize);
	Model model = Serialization::DesserializeModel(mesh1BinData, modelSize);

	const uint8_t* mesh2BinData = AssetRuntimeManager::LoadAsset(62, modelSize);
	Model model2 = Serialization::DesserializeModel(mesh2BinData, modelSize);

	const uint8_t* mesh3BinData = AssetRuntimeManager::LoadAsset(108, modelSize);
	Model model3 = Serialization::DesserializeModel(mesh3BinData, modelSize);

	//const Vec3F worldPos[5] = { {0.0f, 0.0f, 0.0f}, {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
	//	{-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f} };

	const Vec3F worldPos[3] = { {0.0f, 0.0f, 0.0f}, {2.0f, 0.0f, 0.0f}, {6.0f, 0.0f, 0.0f} };

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

		BillboardRenderRequest billboardRequest = { {{-2,0,0}, {1.0f,1.0f}, 69} };
		err = RenderManager::RequestBillboardRender(billboardRequest);
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


bool TestDrawer::running_ = false;

float TestDrawer::rotation_ = -55.0f;
Vec3F TestDrawer::position_ = {0,0,0};
Vec3F TestDrawer::scale_ = {1,1,1};

Timeline TestDrawer::time_(timeline::MILLISECOND);