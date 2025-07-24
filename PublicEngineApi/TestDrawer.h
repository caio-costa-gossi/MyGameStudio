#pragma once
#include "CameraInstance.h"
#include "Err.h"
#include "Mesh.h"
#include "Timeline.h"

class TestDrawer
{
private:
	static bool running_;

	static float rotation_;
	static Vec3F position_;
	static Vec3F scale_;

	static Timeline time_;
	static bool focus_;

	static Err MoveCameraPos(CameraInstance& camera);
	static Err MoveCameraDirection(CameraInstance& camera);
	static Err LockUnlock();

public:
	static Err Startup();
	static Err Run();
	static Err Shutdown();
};
