#include "MeshFactory.h"

#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

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

			std::unique_ptr<uint32_t[]> indexData;
			uint32_t indexCount;

			err = GetIndices(model, primitive, indexData, indexCount);
			if (err.Code())
			{
				ConsoleManager::PrintWarning(err.Message());
				continue;
			}

			Mesh newMesh;
			newMesh.VertexCount = vertexCount;
			newMesh.VertexList = std::move(vertexData);
			newMesh.IndexCount = indexCount;
			newMesh.IndexList = std::move(indexData);

			return newMesh;
		}
	}

	ConsoleManager::PrintError("No suitable primitives found in mesh. Mesh will be invalid.");
	return { };
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

	const float* vertexData = reinterpret_cast<float*>(vertexBuffer.data[vertexAccessor.byteOffset + vertexBufferView.byteOffset]);
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

Err MeshFactory::GetIndices(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::unique_ptr<uint32_t[]>& indices, uint32_t& count)
{
	const tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

	if (indexAccessor.type != TINYGLTF_TYPE_SCALAR)
	{
		ConsoleManager::PrintWarning("Primitive index data type is invalid. Skipping...");
		return error_const::IMPORT_INVALID_PRIMITIVE;
	}

	count = static_cast<uint32_t>(indexAccessor.count);
	indices = std::unique_ptr<uint32_t[]>(new uint32_t[indexAccessor.count]);

	const tinygltf::BufferView indexBufferView = model.bufferViews[indexAccessor.bufferView];
	const tinygltf::Buffer indexBuffer = model.buffers[indexBufferView.buffer];
	const uint32_t byteOffset = static_cast<uint32_t>(indexAccessor.byteOffset + indexBufferView.byteOffset);

	const uint8_t* dataPtr = &indexBuffer.data[byteOffset];

	for (size_t i = 0; i < indexAccessor.count; ++i)
	{
		switch (indexAccessor.componentType)
		{
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			indices[i] = *(dataPtr + i);
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			indices[i] = *(reinterpret_cast<const uint16_t*>(dataPtr + i * sizeof(uint16_t)));
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			indices[i] = *(reinterpret_cast<const uint32_t*>(dataPtr + i * sizeof(uint32_t)));
			break;
		default:
			ConsoleManager::PrintWarning("Unsupported index component type: " + std::to_string(indexAccessor.componentType));
			return {};
		}
	}

	return error_const::SUCCESS;
}
