#pragma once
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

	static uint32_t bulbAssetId_;

public:
	static Err Startup();
	static Err Run();
	static Err Shutdown();
};
