#include "VertexIndexExtractor.h"

#include <tiny_gltf.h>
#include <glm/gtc/quaternion.hpp>

#include "ConsoleManager.h"
#include "TextureExtractor.h"

VertexIndexExtractor::VertexIndexExtractor(tinygltf::Model model) :
	model_(std::move(model)) { }

Err VertexIndexExtractor::ExtractVerticesIndices(std::unique_ptr<Mesh[]>& meshList, uint32_t& meshCount, const Asset& modelMetadata)
{
	// Get mesh count
	ConsoleManager::PrintInfo("Counting meshes...");
	Err err = CountMeshes();
	if (err.Code())
		return err;

	meshList = std::make_unique<Mesh[]>(totalMeshCount_);
	meshList_ = meshList.get();
	meshCount = totalMeshCount_;

	// Get vertex count
	ConsoleManager::PrintInfo("Counting vertices and indices for each mesh...");
	err = CountVerticesIndices();
	if (err.Code())
		return err;

	// Init for each mesh
	ConsoleManager::PrintInfo("Initializing meshes...");
	err = InitMeshes();
	if (err.Code())
		return err;

	// Start extraction of vertices and indices
	err = ExtractAllVerticesIndices();
	if (err.Code())
		return err;

	// Import textures as assets
	err = TextureExtractor::ProcessTextureImages(model_, meshList_, meshInfo_, modelMetadata);
	if (err.Code())
		return err;

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
		const int32_t primitiveTexId = GetPrimitiveMaterialId(primitive);

		meshInfo_[primitiveTexId].TotalVertexCount += static_cast<uint32_t>(vertexAccessor.count);
		meshInfo_[primitiveTexId].TotalIndexCount += static_cast<uint32_t>(indexAccessor.count);
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

Err VertexIndexExtractor::CountMeshes()
{
	for (const tinygltf::Mesh& mesh : model_.meshes) 
	{
		for (const tinygltf::Primitive& primitive : mesh.primitives) 
		{
			const int32_t primitiveMatId = GetPrimitiveMaterialId(primitive);

			if (meshInfo_.find(primitiveMatId) != meshInfo_.end())
				continue;

			meshInfo_[primitiveMatId] = {
				nullptr,
				nullptr,
				0,
				0,
				0,
				0,
				{ },
				totalMeshCount_
			};

			totalMeshCount_++;
		}
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::ExtractAllVerticesIndices()
{
	const tinygltf::Scene& scene = model_.scenes[model_.defaultScene];

	for (const uint32_t nodeId : scene.nodes)
	{
		ConsoleManager::PrintInfo("Processing node " + std::to_string(nodeId) + "...");

		Err err = ExtractVerticesIndicesNode(model_.nodes[nodeId]);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::ExtractVerticesIndicesNode(const tinygltf::Node& node)
{
	if (node.mesh < 0)
	{
		ConsoleManager::PrintWarning("Node mesh index < 0");
		return error_const::SUCCESS;
	}

	// Stack transform matrix
	Err err = StackNodeTransform(node);
	if (err.Code())
		return err;

	// Construct final transform for node
	Transform transform;
	std::stack<Transform> temp = transforms_;
	while (!temp.empty())
	{
		transform = temp.top() * transform;
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

	transforms_.pop();

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::StackNodeTransform(const tinygltf::Node& node)
{
	Transform newTransform;

	if (node.translation.size() == 3)
	{
		const Vec3F translation = {
			static_cast<float>(node.translation[0]),
			static_cast<float>(node.translation[1]),
			static_cast<float>(node.translation[2])
		};

		newTransform.Translate(translation);
	}

	if (node.rotation.size() == 4)
	{
		const glm::quat rotation = {
			static_cast<float>(node.rotation[3]),
			static_cast<float>(node.rotation[0]),
			static_cast<float>(node.rotation[1]),
			static_cast<float>(node.rotation[2])
		};

		const glm::vec3 axis = glm::axis(rotation);
		const float angle = glm::degrees(glm::angle(rotation));

		newTransform.Rotate(angle, axis);
	}

	if (node.scale.size() == 3)
	{
		const Vec3F scale = {
			static_cast<float>(node.scale[0]),
			static_cast<float>(node.scale[1]),
			static_cast<float>(node.scale[2])
		};

		newTransform.Scale(scale);
	}

	transforms_.emplace(newTransform);

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::CopyVerticesIndicesBuffer(const tinygltf::Mesh& mesh, const Transform& transform)
{
	for (const tinygltf::Primitive& primitive : mesh.primitives)
	{
		// Get correct mesh aux info & properties
		const int32_t primitiveMatId = GetPrimitiveMaterialId(primitive);
		MeshAuxInfo& info = meshInfo_[primitiveMatId];

		// Check if primitive has POSITION and if accessor is in correct format
		if (primitive.attributes.find("POSITION") == primitive.attributes.end())
		{
			ConsoleManager::PrintWarning("POSITION attribute not found. Skipping primitive...");
			continue;
		}

		const tinygltf::Accessor& vertexAccessor = model_.accessors[primitive.attributes.at("POSITION")];
		if (vertexAccessor.type != TINYGLTF_TYPE_VEC3 || vertexAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
		{
			ConsoleManager::PrintWarning("Component type is not float or data type is not vec3. Skipping primitive...");
			continue;
		}

		const uint32_t verticesInPrimitive = static_cast<uint32_t>(vertexAccessor.count);
		if (verticesInPrimitive + info.VCounter > info.TotalVertexCount)
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

			// Apply transform & add value to info.VertexList (mesh specific vertex list)
			newVertex.Pos = transform * newVertex.Pos;

			info.VertexList[info.VCounter + i] = newVertex;
		}

		// Add indices based on info.VCounter (mesh specific VCounter)
		Err err = ExtractIndices(primitive, info);
		if (err.Code())
			return err;

		err = TextureExtractor::ProcessPrimitiveTexCoords(model_, primitive, info, verticesInPrimitive);
		if (err.Code())
			return err;

		info.VCounter += verticesInPrimitive;
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::ExtractIndices(const tinygltf::Primitive& primitive, MeshAuxInfo& info) const
{
	const tinygltf::Accessor indexAccessor = model_.accessors[primitive.indices];

	// Check if index type is valid
	if (indexAccessor.type != TINYGLTF_TYPE_SCALAR)
	{
		ConsoleManager::PrintWarning("Primitive index data type is invalid. Skipping...");
		return error_const::SUCCESS;
	}

	const uint64_t count = static_cast<uint32_t>(indexAccessor.count);
	if (info.ICounter + count > info.TotalIndexCount)
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
			info.IndexList[info.ICounter + i] = info.VCounter + *(dataPtr + i);
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			info.IndexList[info.ICounter + i] = info.VCounter + *(reinterpret_cast<const uint16_t*>(dataPtr + i * sizeof(uint16_t)));
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			info.IndexList[info.ICounter + i] = info.VCounter + *(reinterpret_cast<const uint32_t*>(dataPtr + i * sizeof(uint32_t)));
			break;
		default:
			ConsoleManager::PrintWarning("Unsupported index component type: " + std::to_string(indexAccessor.componentType));
			return error_const::SUCCESS;
		}
	}

	info.ICounter += static_cast<uint32_t>(count);

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::InitMeshes()
{
	// Go through each texture ID, get the respective mesh index, init the mesh pointers and store a reference in the aux struct
	for (std::pair<const int32_t, MeshAuxInfo>& pair : meshInfo_)
	{
		MeshAuxInfo& info = pair.second;

		Mesh* mesh = &meshList_[info.MeshIndex];
		mesh->VertexList = std::make_unique<Vertex[]>(info.TotalVertexCount);
		mesh->IndexList = std::make_unique<uint32_t[]>(info.TotalIndexCount);
		mesh->VertexCount = info.TotalVertexCount;
		mesh->IndexCount = info.TotalIndexCount;

		info.VertexList = mesh->VertexList.get();
		info.IndexList = mesh->IndexList.get();

		Err err = GetMaterialMapsInfo(pair.first, info.MapsInfo);
		if (err.Code())
			ConsoleManager::PrintWarning("Error while trying to obtain maps info from material: " + err.Message());
	}

	return error_const::SUCCESS;
}

int32_t VertexIndexExtractor::GetPrimitiveMaterialId(const tinygltf::Primitive& primitive) const
{
	if (primitive.material < 0 || primitive.material >= static_cast<int32_t>(model_.materials.size()))
		return -1;

	return primitive.material;
}

Err VertexIndexExtractor::GetMaterialMapsInfo(const int32_t materialId, MaterialMapsInfo& mapsInfo) const
{
	if (materialId < 0)
		return error_const::SUCCESS;

	const tinygltf::Material& mat = model_.materials[materialId];

	mapsInfo = {
		mat.pbrMetallicRoughness.baseColorTexture.index,
		mat.pbrMetallicRoughness.baseColorTexture.texCoord,
		mat.normalTexture.index,
		mat.normalTexture.texCoord,
		mat.pbrMetallicRoughness.metallicRoughnessTexture.index,
		mat.pbrMetallicRoughness.metallicRoughnessTexture.texCoord,
		mat.occlusionTexture.index,
		mat.occlusionTexture.texCoord,
		mat.emissiveTexture.index,
		mat.emissiveTexture.texCoord,
		static_cast<float>(mat.pbrMetallicRoughness.metallicFactor),
		static_cast<float>(mat.pbrMetallicRoughness.roughnessFactor),
		mat.doubleSided
	};

	struct MaterialMapsInfo
	{
		int32_t DiffuseTexId = 0;
		int32_t DiffuseTexCoordIndex = 0;

		int32_t NormalTexId = 0;
		int32_t NormalTexCoordIndex = 0;

		int32_t MetallicRoughnessTexId = 0;
		int32_t MetallicRoughnessCoordIndex = 0;

		int32_t OcclusionTexId = 0;
		int32_t OcclusionTexCoordIndex = 0;

		int32_t EmissiveTexId = 0;
		int32_t EmissiveTexCoordIndex = 0;

		float MetallicFactor = 0;
		float RoughnessFactor = 0;
		bool DoubleSided = true;
	};

	return error_const::SUCCESS;
}
