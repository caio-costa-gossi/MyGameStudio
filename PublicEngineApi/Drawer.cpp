#include "Drawer.h"
#include <glad/glad.h>

#include "BillboardDrawer.h"
#include "GameConsoleManager.h"
#include "LightingManager.h"
#include "ModelDrawer.h"

Err Drawer::Init()
{
	Err err = ModelDrawer::Init();
	if (err.Code())
		return err;

	err = BillboardDrawer::Init();
	if (err.Code())
		return err;

	coordGizmo_.InitGizmo();

	return error_const::SUCCESS;
}

void Drawer::Draw()
{
	// Reset color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set lighting sources
	Err err = LightingManager::SetLightUniforms();
	if (err.Code())
		GameConsoleManager::PrintError("Error setting lighting uniforms: " + err.Message(), enums::ConsoleMessageSender::render);

	// Draw 3d meshes with standard shader
	err = ModelDrawer::Draw();
	if (err.Code())
		GameConsoleManager::PrintError("Error drawing models: " + err.Message(), enums::ConsoleMessageSender::render);

	// Draw billboards with billboard shader
	err = BillboardDrawer::Draw();
	if (err.Code())
		GameConsoleManager::PrintError("Error drawing billboards: " + err.Message(), enums::ConsoleMessageSender::render);

	coordGizmo_.Draw();
}


CoordinateGizmo Drawer::coordGizmo_;
