#include "CoordinateGizmo.h"

#include <glad/glad.h>

#include "BillboardDrawer.h"
#include "BillboardRenderRequest.h"
#include "Camera.h"
#include "CameraManager.h"
#include "GameConsoleManager.h"
#include "ModelDrawer.h"
#include "Shader.h"
#include "Transform.h"
#include "VaoFactory.h"
#include "Vertex.h"
#include "WindowManager.h"

void CoordinateGizmo::InitGizmo()
{
	InitUniformIds();
	BuildVao();
	BuildNodeObjects();
	BuildCamera();
	BuildTextures();
}

void CoordinateGizmo::InitUniformIds()
{
	Shader& modelShader = ModelDrawer::GetShader();
	Shader& billboardShader = BillboardDrawer::GetShader();

	modelShader.Use();

	// Light
	uAmbientColor_ = modelShader.GetUniformId("ambientColor");
	uAmbientIntensity_ = modelShader.GetUniformId("ambientIntensity");

	uDirLightCount_ = modelShader.GetUniformId("dirLightsCount");
	uPointLightCount_ = modelShader.GetUniformId("pointLightsCount");
	uSpotlightCount_ = modelShader.GetUniformId("spotlightsCount");

	uUseVertexColor_ = modelShader.GetUniformId("useVertexColor");

	// Transforms
	uModel_ = modelShader.GetUniformId("model");
	uProjection_ = modelShader.GetUniformId("projection");
	uView_ = modelShader.GetUniformId("view");

	// Billboard
	billboardShader.Use();

	uBboardCenterWorld_ = billboardShader.GetUniformId("centerWorld");
	uBboardScale_ = billboardShader.GetUniformId("scale");
	uBboardView_ = billboardShader.GetUniformId("view");
	uBboardProjection_ = billboardShader.GetUniformId("projection");
}

void CoordinateGizmo::BuildVao()
{
	// Gizmo Attribute Object
	glGenVertexArrays(1, &vaoId_);
	glBindVertexArray(vaoId_);

	// Setup Vertex Buffer Object
	Vertex gizmoVertices[6] = {
		{ {0.0f, 0.0f, 0.0f}, { }, {1.0f, 0.0f, 0.0f, 1.0f}},
		{ {0.9f, 0.0f, 0.0f}, { }, {1.0f, 0.0f, 0.0f, 1.0f}},
		{ {0.0f, 0.0f, 0.0f}, { }, {0.0f, 1.0f, 0.0f, 1.0f}},
		{ {0.0f, 0.9f, 0.0f}, { }, {0.0f, 1.0f, 0.0f, 1.0f}},
		{ {0.0f, 0.0f, 0.0f}, { }, {0.0f, 0.0f, 1.0f, 1.0f}},
		{ {0.0f, 0.0f, 0.9f}, { }, {0.0f, 0.0f, 1.0f, 1.0f}}
	};

	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(6 * sizeof(Vertex)), gizmoVertices, GL_STATIC_DRAW);

	// Define vertex attribute layout
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Color
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind VAO & VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CoordinateGizmo::BuildNodeObjects()
{
	xNode_.RenderQuery = { { { 0.9f, -0.2f, 0.0f }, { 0.3f, 0.3f }, 0 }, 0 };
	yNode_.RenderQuery = { { { 0.0f, 0.8f, 0.0f }, { 0.3f, 0.3f }, 0 }, 0 };
	zNode_.RenderQuery = { { { 0.0f, -0.2f, 0.9f }, { 0.3f, 0.3f }, 0 }, 0 };

	VaoFactory::NewBillboardAttribObject(xNode_.RenderQuery.BillboardVao);
	VaoFactory::NewBillboardAttribObject(yNode_.RenderQuery.BillboardVao);
	VaoFactory::NewBillboardAttribObject(zNode_.RenderQuery.BillboardVao);
}

void CoordinateGizmo::BuildCamera()
{
	CameraManager::CreatePerspective(camId_, enums::free_cam, 600, 600, 90, 0.01f, 100.0f);
}

void CoordinateGizmo::BuildTextures()
{
	Err err = xNode_.NodeTexture.Init("internal_engine_assets/X.png", xNode_.RenderQuery.BillboardData.BillboardImageId);
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);

	yNode_.NodeTexture.Init("internal_engine_assets/Y.png", yNode_.RenderQuery.BillboardData.BillboardImageId);
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);

	zNode_.NodeTexture.Init("internal_engine_assets/Z.png", zNode_.RenderQuery.BillboardData.BillboardImageId);
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);
}

void CoordinateGizmo::UpdateGizmoCam() const
{
	Camera* gizmoCam = CameraManager::GetCamera(camId_);
	if (gizmoCam == nullptr)
		return;

	gizmoCam->ChangePitch(-CameraManager::GetMainCamera()->GetPitch());
	gizmoCam->ChangeYaw(-CameraManager::GetMainCamera()->GetYaw());
}

void CoordinateGizmo::Draw()
{
	glViewport(0, 2 * WindowManager::GetWindowHeight() / 3, WindowManager::GetWindowWidth() / 3, WindowManager::GetWindowHeight() / 3);

	UpdateGizmoCam();
	DrawAxes();
	DrawNodes();
}

void CoordinateGizmo::DrawAxes() const
{
	Shader& modelShader = ModelDrawer::GetShader();

	modelShader.Use();

	Transform transform;
	transform.Translate({ 0.0f, -0.1f, 0.0f });

	const Camera* gizmoCam = CameraManager::GetCamera(camId_);
	if (gizmoCam == nullptr)
		return;

	// Lighting
	Shader::SetUniform(uAmbientColor_, 1.0f, 1.0f, 1.0f);
	Shader::SetUniform(uAmbientIntensity_, 1.0f);

	Shader::SetUniform(uDirLightCount_, 0);
	Shader::SetUniform(uPointLightCount_, 0);
	Shader::SetUniform(uSpotlightCount_, 0);

	Shader::SetUniform(uUseVertexColor_, true);

	// Transforms
	Shader::SetUniform(uModel_, enums::MatrixDim::m4x4, transform.GetData(), false);
	Shader::SetUniform(uProjection_, enums::MatrixDim::m4x4, transform.GetData(), false);
	Shader::SetUniform(uView_, enums::MatrixDim::m4x4, gizmoCam->GetView().GetData(), false);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vaoId_);
	glLineWidth(5.0f);

	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

	glLineWidth(1.0f);
}

void CoordinateGizmo::DrawNodes()
{
	Shader& billboardShader = BillboardDrawer::GetShader();
	billboardShader.Use();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Emplace to sort the nodes by distance
	const Camera* gizmoCam = CameraManager::GetCamera(camId_);
	if (gizmoCam == nullptr)
		return;

	EmplaceNodes(gizmoCam);

	while (!nodeRenderQuery_.empty())
	{
		const RenderNode& node = nodeRenderQuery_.front();

		// Prepare node billboard uniforms
		Shader::SetUniform(uBboardCenterWorld_, node.RenderQuery.BillboardData.WorldPos.X, node.RenderQuery.BillboardData.WorldPos.Y, node.RenderQuery.BillboardData.WorldPos.Z);
		Shader::SetUniform(uBboardScale_, node.RenderQuery.BillboardData.Scale.X, node.RenderQuery.BillboardData.Scale.Y);
		Shader::SetUniform(uBboardView_, enums::MatrixDim::m4x4, gizmoCam->GetView().GetData(), false);
		Shader::SetUniform(uBboardProjection_, enums::MatrixDim::m4x4, Transform().GetData(), false);

		// Draw
		node.NodeTexture.Use(0);
		glBindVertexArray(node.RenderQuery.BillboardVao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		nodeRenderQuery_.pop();
	}
}

void CoordinateGizmo::EmplaceNodes(const Camera* camera)
{
	if (camera->GetYaw() > 90)
	{
		nodeRenderQuery_.emplace(xNode_);
		nodeRenderQuery_.emplace(yNode_);
		nodeRenderQuery_.emplace(zNode_);
	}
	else
	{
		nodeRenderQuery_.emplace(zNode_);
		nodeRenderQuery_.emplace(yNode_);
		nodeRenderQuery_.emplace(xNode_);
	}
}
