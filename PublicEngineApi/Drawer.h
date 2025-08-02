#pragma once

#include "CoordinateGizmo.h"

class Drawer
{
private:
	static CoordinateGizmo coordGizmo_;

public:
	static Err Init();
	static void Draw();
};