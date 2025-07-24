#include "VertexIndexExtractor.h"

#include <tiny_gltf.h>

#include "ConsoleManager.h"

VertexIndexExtractor::VertexIndexExtractor(tinygltf::Model model) :
	model_(std::move(model)) { }

Err VertexIndexExtractor::ExtractVerticesIndices(std::unique_ptr<Vertex[]>& vertices, uint32_t& vertexCount, std::unique_ptr<uint32_t[]>& indices, uint32_t& indexCount)
{
	// Get vertex count
	Err err = CountVerticesIndices();
	if (err.Code())
		return err;

	// Prepare vertex & index buffer
	vertices = std::make_unique<Vertex[]>(totalVertexCount_);
	vertexList_ = vertices.get();

	indices = std::make_unique<uint32_t[]>(totalIndexCount_);
	indexList_ = indices.get();

	// Start extraction
	err = ExtractAllVerticesIndices();
	if (err.Code())
		return err;

	vertexCount = totalVertexCount_;
	indexCount = totalIndexCount_;

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::CountVerticesIndices()
{
	const tinygltf::Scene& scene = model_.scenes[model_.defaultScene];

	for (const uint32_t nodeId : scene.nodes)
	{
		Err err = CountVerticesIndicesNode(model_.nodes[nodeId]);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::CountVerticesIndicesNode(const tinygltf::Node& node)
{
	// Calculate node vertex count
	if (node.mesh < 0)
	{
		ConsoleManager::PrintWarning("Node mesh index < 0");
		return error_const::SUCCESS;
	}

	const tinygltf::Mesh& mesh = model_.meshes[node.mesh];

	for (const tinygltf::Primitive& primitive : mesh.primitives)
	{
		if (primitive.attributes.find("POSITION") == primitive.attributes.end())
		{
			ConsoleManager::PrintWarning("Primitive does not contain POSITION data. Skipping...");
			continue;
		}

		const tinygltf::Accessor& vertexAccessor = model_.accessors[primitive.attributes.at("POSITION")];
		const tinygltf::Accessor& indexAccessor = model_.accessors[primitive.indices];

		totalVertexCount_ += vertexAccessor.count;
		totalIndexCount_ += indexAccessor.count;
	}

	// Go through children
	for (const uint32_t childId : node.children)
	{
		Err err = CountVerticesIndicesNode(model_.nodes[childId]);
		if (err.Code())
			ConsoleManager::PrintWarning("Error processing node vertex count: " + err.Message());
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::ExtractAllVerticesIndices()
{
	const tinygltf::Scene& scene = model_.scenes[model_.defaultScene];

	for (const uint32_t nodeId : scene.nodes)
	{
		Err err = ExtractVerticesIndicesNode(model_.nodes[nodeId]);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::ExtractVerticesIndicesNode(const tinygltf::Node& node)
{
	// Stack transform matrix
	Err err = StackNodeTransform(node);
	if (err.Code())
		return err;

	// Construct final transform for node
	Transform transform;
	std::stack<Transform> temp = transforms_;
	while (!temp.empty())
	{
		transform = transform * temp.top();
		temp.pop();
	}

	err = CopyVerticesIndicesBuffer(model_.meshes[node.mesh], transform);
	if (err.Code())
		return err;

	// Go through children
	for (const uint32_t childId : node.children)
	{
		err = ExtractVerticesIndicesNode(model_.nodes[childId]);
		if (err.Code())
			ConsoleManager::PrintWarning("Error processing node vertex count: " + err.Message());
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::StackNodeTransform(const tinygltf::Node& node)
{
	if (!node.matrix.empty() && node.matrix.size() != 16)
		return error_const::IMPORT_INVALID_TRANSFORM;

	if (node.matrix.empty())
		return error_const::SUCCESS;

	float matrixVal[16] {};
	for (uint8_t i = 0; i < 16; ++i)
	{
		matrixVal[i] = static_cast<float>(node.matrix[i]);
	}

	auto transform = Transform(matrixVal);
	transforms_.emplace(transform);

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::CopyVerticesIndicesBuffer(const tinygltf::Mesh& mesh, const Transform& transform)
{
	for (const tinygltf::Primitive& primitive : mesh.primitives)
	{
		if (primitive.attributes.find("POSITION") == primitive.attributes.end())
		{
			ConsoleManager::PrintWarning("POSITION attribute not found. Skipping primitive...");
			continue;
		}

		const tinygltf::Accessor vertexAccessor = model_.accessors[primitive.attributes.at("POSITION")];
		if (vertexAccessor.type != TINYGLTF_TYPE_VEC3 || vertexAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
		{
			ConsoleManager::PrintWarning("Component type is not float or data type is not vec3. Skipping primitive...");
			continue;
		}

		const uint32_t verticesInPrimitive = static_cast<uint32_t>(vertexAccessor.count);
		if (verticesInPrimitive + vCounter_ > totalVertexCount_)
		{
			ConsoleManager::PrintWarning("VertexCount from primitive will exceed allocated space. Skipping...");
			continue;
		}

		const tinygltf::BufferView vertexBufferView = model_.bufferViews[vertexAccessor.bufferView];
		const tinygltf::Buffer vertexBuffer = model_.buffers[vertexBufferView.buffer];

		const size_t offset = vertexAccessor.byteOffset + vertexBufferView.byteOffset;
		const size_t stride = vertexBufferView.byteStride ? vertexBufferView.byteStride : 3 * sizeof(float);

		for (uint32_t i = 0; i < vertexAccessor.count; ++i)
		{
			const float* vertexData = reinterpret_cast<const float*>(vertexBuffer.data.data() + offset + stride * i);

			Vertex newVertex;

			newVertex.Color = { 255,255,255,255 };
			newVertex.Pos.X = vertexData[0];
			newVertex.Pos.Y = vertexData[1];
			newVertex.Pos.Z = vertexData[2];

			// Apply transform & add value to vertexList
			newVertex.Pos = transform * newVertex.Pos;

			vertexList_[vCounter_ + i] = newVertex;
		}

		// Add indices based on vCounter_
		Err err = ExtractIndices(primitive);
		if (err.Code())
			return err;

		vCounter_ += verticesInPrimitive;
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::ExtractIndices(const tinygltf::Primitive& primitive)
{
	const tinygltf::Accessor indexAccessor = model_.accessors[primitive.indices];

	if (indexAccessor.type != TINYGLTF_TYPE_SCALAR)
	{
		ConsoleManager::PrintWarning("Primitive index data type is invalid. Skipping...");
		return error_const::SUCCESS;
	}

	const uint64_t count = static_cast<uint32_t>(indexAccessor.count);
	if (iCounter_ + count > totalIndexCount_)
	{
		ConsoleManager::PrintWarning("Primitive indices will overflow indices buffer. Skipping...");
		return error_const::SUCCESS;
	}

	const tinygltf::BufferView indexBufferView = model_.bufferViews[indexAccessor.bufferView];
	const tinygltf::Buffer indexBuffer = model_.buffers[indexBufferView.buffer];
	const uint32_t byteOffset = static_cast<uint32_t>(indexAccessor.byteOffset + indexBufferView.byteOffset);

	const uint8_t* dataPtr = &indexBuffer.data[byteOffset];

	// Go through indices, with indexOffset on the indexList and vertexOffset on the index value
	for (size_t i = 0; i < indexAccessor.count; ++i)
	{
		switch (indexAccessor.componentType)
		{
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			indexList_[iCounter_ + i] = vCounter_ + *(dataPtr + i);
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			indexList_[iCounter_ + i] = vCounter_ + *(reinterpret_cast<const uint16_t*>(dataPtr + i * sizeof(uint16_t)));
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			indexList_[iCounter_ + i] = vCounter_ + *(reinterpret_cast<const uint32_t*>(dataPtr + i * sizeof(uint32_t)));
			break;
		default:
			ConsoleManager::PrintWarning("Unsupported index component type: " + std::to_string(indexAccessor.componentType));
			return error_const::SUCCESS;
		}
	}

	iCounter_ += count;

	return error_const::SUCCESS;
}
