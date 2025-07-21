#include "MeshFactory.h"

#include "ConsoleManager.h"

Mesh MeshFactory::CreateMesh(const tinygltf::Model& model)
{
	for (const tinygltf::Mesh& mesh : model.meshes)
	{
		for (const tinygltf::Primitive& primitive : mesh.primitives)
		{
			if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
			{
				ConsoleManager::PrintWarning("Mesh mode is not TRIANGLES. Skipping primitive...");
				continue;
			}

			std::unique_ptr<Vertex[]> vertexData;
			uint32_t vertexCount;

			Err err = GetVertices(model, primitive, vertexData, vertexCount);
			if (err.Code())
			{
				ConsoleManager::PrintWarning(err.Message());
				continue;
			}

			//const tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
		}
	}

	return Mesh();
}

Err MeshFactory::GetVertices(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::unique_ptr<Vertex[]>& vertices, uint32_t& count)
{
	if (primitive.attributes.find("POSITION") == primitive.attributes.end())
	{
		ConsoleManager::PrintWarning("POSITION attribute not found. Skipping primitive...");
		return error_const::IMPORT_INVALID_PRIMITIVE;
	}

	const tinygltf::Accessor vertexAccessor = model.accessors[primitive.attributes.at("POSITION")];
	if (vertexAccessor.type != TINYGLTF_TYPE_VEC3 || vertexAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
	{
		ConsoleManager::PrintWarning("Component type is not float or data type is not vec3. Skipping primitive...");
		return error_const::IMPORT_INVALID_PRIMITIVE;
	}

	count = static_cast<uint32_t>(vertexAccessor.count);
	vertices = std::unique_ptr<Vertex[]>(new Vertex[vertexAccessor.count]);

	const tinygltf::BufferView vertexBufferView = model.bufferViews[vertexAccessor.bufferView];
	const tinygltf::Buffer vertexBuffer = model.buffers[vertexBufferView.buffer];

	const float* vertexData = reinterpret_cast<float*>(vertexBuffer.data[vertexBufferView.byteOffset]);
	for (uint32_t i = 0; i < vertexAccessor.count; ++i)
	{
		Vertex newVertex;

		newVertex.Color = { 255,255,255,255 };
		newVertex.Pos.X = vertexBuffer.data[i * 3 + 1];
		newVertex.Pos.Y = vertexBuffer.data[i * 3 + 2];
		newVertex.Pos.Z = vertexBuffer.data[i * 3 + 3];

		vertices[i] = newVertex;
	}

	return error_const::SUCCESS;
}

