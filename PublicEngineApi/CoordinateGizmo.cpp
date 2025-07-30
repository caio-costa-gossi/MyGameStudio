#include "CoordinateGizmo.h"

#include <glad/glad.h>

#include "BillboardRenderRequest.h"
#include "Camera.h"
#include "CameraManager.h"
#include "GameConsoleManager.h"
#include "Shader.h"
#include "Transform.h"
#include "VaoFactory.h"
#include "Vertex.h"
#include "WindowManager.h"

void CoordinateGizmo::InitGizmo()
{
	BuildVao();
	BuildNodeObjects();
	BuildCamera();
	BuildTextures();
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
	gizmoCam->ChangePitch(-CameraManager::GetMainCamera()->GetPitch());
	gizmoCam->ChangeYaw(-CameraManager::GetMainCamera()->GetYaw());
}

void CoordinateGizmo::Draw(const Shader& regularShader, const Shader& billboardShader)
{
	glViewport(0, 2 * WindowManager::GetWindowHeight() / 3, WindowManager::GetWindowWidth() / 3, WindowManager::GetWindowHeight() / 3);

	UpdateGizmoCam();
	DrawAxes(regularShader);
	DrawNodes(billboardShader);
}

void CoordinateGizmo::DrawAxes(const Shader& regularShader) const
{
	regularShader.Use();

	Transform transform;
	transform.Translate({ 0.0f, -0.1f, 0.0f });

	const Camera* gizmoCam = CameraManager::GetCamera(camId_);

	// Lighting
	regularShader.SetUniform("ambientColor", 1.0f, 1.0f, 1.0f);
	regularShader.SetUniform("ambientFactor", 1.0f);

	regularShader.SetUniform("lightPos", 0.0f, 0.0f, 0.0f);
	regularShader.SetUniform("lightColor", 1.0f, 1.0f, 1.0f);
	regularShader.SetUniform("lightStrength", 0.0f);

	regularShader.SetUniform("useVertexColor", true);

	// Transforms
	regularShader.SetUniform("model", enums::MatrixDim::m4x4, transform.GetData(), false);
	regularShader.SetUniform("projection", enums::MatrixDim::m4x4, transform.GetData(), false);
	regularShader.SetUniform("view", enums::MatrixDim::m4x4, gizmoCam->GetView().GetData(), false);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vaoId_);
	glLineWidth(5.0f);

	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

	glLineWidth(1.0f);
}

void CoordinateGizmo::DrawNodes(const Shader& billboardShader)
{
	billboardShader.Use();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Emplace to sort the nodes by distance
	const Camera* gizmoCam = CameraManager::GetCamera(camId_);
	EmplaceNodes(gizmoCam);

	while (!nodeRenderQuery_.empty())
	{
		const RenderNode& node = nodeRenderQuery_.front();

		// Prepare node billboard uniforms
		billboardShader.SetUniform("centerWorld", node.RenderQuery.BillboardData.WorldPos.X, node.RenderQuery.BillboardData.WorldPos.Y, node.RenderQuery.BillboardData.WorldPos.Z);
		billboardShader.SetUniform("scale", node.RenderQuery.BillboardData.Scale.X, node.RenderQuery.BillboardData.Scale.Y);
		billboardShader.SetUniform("view", enums::MatrixDim::m4x4, gizmoCam->GetView().GetData(), false);
		billboardShader.SetUniform("projection", enums::MatrixDim::m4x4, Transform().GetData(), false);

		// Draw
		node.NodeTexture.Use();
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
