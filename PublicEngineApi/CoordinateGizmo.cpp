#include "CoordinateGizmo.h"

#include <glad/glad.h>

#include "Camera.h"
#include "CameraManager.h"
#include "Shader.h"
#include "Transform.h"
#include "Vertex.h"
#include "WindowManager.h"

void CoordinateGizmo::InitGizmo()
{
	BuildVao();
	BuildCamera();
}

void CoordinateGizmo::BuildVao()
{
	// Gizmo Attribute Object
	glGenVertexArrays(1, &vaoId_);
	glBindVertexArray(vaoId_);

	// Setup Vertex Buffer Object
	Vertex gizmoVertices[6] = {
		{ {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
		{ {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{ {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{ {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
		{ {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
	};

	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(6 * sizeof(Vertex)), gizmoVertices, GL_STATIC_DRAW);

	// Define vertex attribute layout
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TextCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind VAO & VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CoordinateGizmo::BuildCamera()
{
	CameraManager::CreatePerspective(camId_, enums::free_cam, 600, 600, 60, 0.01f, 100);
}

void CoordinateGizmo::Draw(const Shader& shader)
{
	Transform transform;
	transform.Translate({ 0.0f, -0.1f, 0.0f });

	Camera* gizmoCam = CameraManager::GetCamera(camId_);
	gizmoCam->ChangePitch(CameraManager::GetMainCamera()->GetPitch());
	gizmoCam->ChangeYaw(CameraManager::GetMainCamera()->GetYaw());

	shader.SetUniform("model", enums::MatrixDim::m4x4, transform.GetData(), false);
	shader.SetUniform("projection", enums::MatrixDim::m4x4, transform.GetData(), false);
	shader.SetUniform("view", enums::MatrixDim::m4x4, gizmoCam->GetView().GetData(), false);
	shader.SetUniform("light", 1.0f, 1.0f, 1.0f);

	glViewport(0, 0, WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight());
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vaoId_);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);
}
