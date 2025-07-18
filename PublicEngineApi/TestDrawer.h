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

	static Mesh testMesh_;

	static Timeline time_;

public:
	static Err Startup();
	static Err Run();
	static Err Shutdown();
};
