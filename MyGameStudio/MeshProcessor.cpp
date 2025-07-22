#include "MeshProcessor.h"

#include <set>
#include <sstream>

#include "ConsoleManager.h"
#include "LocalizationManager.h"
#include "MeshFactory.h"
#include "Serialization.h"

#undef max
#undef min

uint8_t* MeshProcessor::ProcessMesh(const Asset& metadata, uint64_t& resultSize, std::string& errMsg)
{
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;

	// Mesh pre-processing
	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "0%", enums::ConsoleMessageType::info);
	loader.LoadBinaryFromFile(&model, &errMsg, nullptr, metadata.SourceLocation);

	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "20%", enums::ConsoleMessageType::info);
	Err error = VerifyModel(model);
	if (error.Code())
	{
		errMsg = LocalizationManager::GetLocalizedString(string_const::G_NOT_TRIANGULATED);
		return nullptr;
	}

	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "40%", enums::ConsoleMessageType::info);
	CompressIndices(model);

	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "60%", enums::ConsoleMessageType::info);
	Triangulate(model);

	// Mesh importation
	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "80%", enums::ConsoleMessageType::info);
	const Mesh mesh = MeshFactory::CreateMesh(model, metadata);

	// Mesh serialization
	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "100%", enums::ConsoleMessageType::info);

	const DataStream stream = Serialization::SerializeMesh(mesh, resultSize);
	uint8_t* meshData = new uint8_t[resultSize];
	memcpy_s(meshData, resultSize, stream.Data, resultSize);

	return meshData;
}

uint8_t* MeshProcessor::GetFileBuffer(tinygltf::TinyGLTF& loader, const tinygltf::Model& model, uint64_t& resultSize)
{
	std::ostringstream outputStream;
	loader.WriteGltfSceneToStream(&model, outputStream, true, true);
	const std::string output = outputStream.str();

	resultSize = output.size();
	const auto resultBuffer = new uint8_t[resultSize];
	memcpy_s(resultBuffer, resultSize, output.data(), resultSize);

	return resultBuffer;
}

Err MeshProcessor::VerifyModel(const tinygltf::Model& model)
{
	for (const tinygltf::Mesh& mesh : model.meshes)
	{
		for (const tinygltf::Primitive& primitive : mesh.primitives)
			if (primitive.mode != 4 && primitive.mode != 5 && primitive.mode != 6)
				return error_const::MODEL_NOT_TRIANGULATED;
	}

	return error_const::SUCCESS;
}

Err MeshProcessor::Triangulate(tinygltf::Model& model)
{
	for (tinygltf::Mesh& mesh : model.meshes)
	{
		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			std::vector<uint8_t> newIndices;
			const tinygltf::Accessor accessor = model.accessors[primitive.indices];
			const tinygltf::BufferView bufferView = model.bufferViews[accessor.bufferView];
			tinygltf::Buffer buffer = model.buffers[bufferView.buffer];

			const uint8_t* uByteIndices = buffer.data.data();
			const uint16_t* uShortIndices = reinterpret_cast<uint16_t*>(buffer.data.data());

			if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
			{
				for (size_t i = 1; i < accessor.count - 1; ++i)
					newIndices.insert(newIndices.end(), {uByteIndices[0], uByteIndices[i], uByteIndices[i + 1]});
			}
			else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
			{
				newIndices.resize(accessor.count * sizeof(uint16_t) * 3);
				uint16_t* pNewIndices = reinterpret_cast<uint16_t*>(newIndices.data());

				for (size_t i = 1; i < accessor.count - 1; ++i)
				{
					pNewIndices[i * 3 - 3] = uShortIndices[0];
					pNewIndices[i * 3 - 2] = uShortIndices[i];
					pNewIndices[i * 3 - 1] = uShortIndices[i + 1];
				}
			}
			else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
			{
				newIndices.reserve(accessor.count * 3);

				for (size_t i = 0; i < accessor.count - 2; ++i)
				{
					if (i % 2 == 0)
						newIndices.insert(newIndices.end(), { uByteIndices[i], uByteIndices[i + 1], uByteIndices[i + 2] });
					else
						newIndices.insert(newIndices.end(), { uByteIndices[i + 1], uByteIndices[i], uByteIndices[i + 2] });
				}
			}
			else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
			{
				newIndices.resize(accessor.count * sizeof(uint16_t) * 3);
				uint16_t* pNewIndices = reinterpret_cast<uint16_t*>(newIndices.data());

				for (size_t i = 0; i < accessor.count - 2; ++i)
				{
					if (i % 2 == 0)
					{
						pNewIndices[i * 3] = uShortIndices[i];
						pNewIndices[i * 3 + 1] = uShortIndices[i + 1];
						pNewIndices[i * 3 + 2] = uShortIndices[i + 2];
					}
					else
					{
						pNewIndices[i * 3] = uShortIndices[i + 1];
						pNewIndices[i * 3 + 1] = uShortIndices[i];
						pNewIndices[i * 3 + 2] = uShortIndices[i + 2];
					}
				}
			}
			else
				continue;

			ChangeBuffer(buffer.data, accessor.count * GetComponentTypeSize(accessor.componentType), newIndices, bufferView.byteOffset + accessor.byteOffset, model);
			primitive.mode = TINYGLTF_MODE_TRIANGLES;
		}
	}

	return error_const::SUCCESS;
}

Err MeshProcessor::CompressIndices(tinygltf::Model& model)
{
	for (tinygltf::Mesh& mesh : model.meshes)
	{
		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			tinygltf::Accessor& accessor = model.accessors[primitive.indices];
			tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
			tinygltf::Buffer& buffer = model.buffers[view.buffer];

			size_t indexAmount = accessor.count;

			if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
				continue;

			const uint32_t* indices = reinterpret_cast<uint32_t*>(buffer.data.data() + view.byteOffset + accessor.byteOffset);

			uint32_t maxIndex = 0;
			for (size_t i = 0; i < indexAmount; ++i)
				maxIndex = std::max(maxIndex, indices[i]);

			int newComponentType;
			if (maxIndex < 255)
			{
				newComponentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
			}
			else if (maxIndex < 65535)
			{
				newComponentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
			}
			else continue;

			std::vector<unsigned char> compressedData;
			compressedData.reserve(indexAmount);

			if (newComponentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
			{
				for (size_t i = 0; i < indexAmount; ++i)
					compressedData.push_back(static_cast<uint8_t>(indices[i]));
			}
			else
			{
				compressedData.resize(indexAmount * sizeof(uint16_t));
				uint16_t* pCompressedData = reinterpret_cast<uint16_t*>(compressedData.data());

				for (size_t i = 0; i < indexAmount; ++i)
					pCompressedData[i] = static_cast<uint16_t>(indices[i]);
			}

			ChangeBuffer(buffer.data, accessor.count * GetComponentTypeSize(accessor.componentType), compressedData, view.byteOffset + accessor.byteOffset, model);
			accessor.componentType = newComponentType;
		}
	}

	return error_const::SUCCESS;
}

void MeshProcessor::ChangeBuffer(std::vector<uint8_t>& oldData, const uint64_t oldDataByteCount, const std::vector<uint8_t>& newData, const size_t initialOffset, tinygltf::Model& model)
{
	const size_t oldDataSize = oldDataByteCount;
	const size_t newDataSize = newData.size();
	const size_t biggerCount = std::max(oldDataSize, newDataSize);

	const int sizeDiff = static_cast<int>(newDataSize) - static_cast<int>(oldDataSize);
	const int commonSize = std::min(oldDataSize, newDataSize);

	// Memory management
	std::copy_n(newData.begin(), commonSize, oldData.begin());

	if (newDataSize > oldDataSize)
		oldData.insert(oldData.begin() + static_cast<long long>(initialOffset + commonSize), newData.begin() + commonSize, newData.end());

	if (newDataSize < oldDataSize)
		oldData.erase(oldData.begin() + static_cast<long long>(initialOffset + newDataSize), oldData.begin() + static_cast<long long>(initialOffset + oldDataSize));

	// Update metadata
	std::set<int> updatedBuffers;

	for (const tinygltf::Accessor& accessor : model.accessors)
	{
		if (accessor.byteOffset + model.bufferViews[accessor.bufferView].byteOffset > initialOffset && updatedBuffers.find(accessor.bufferView) == updatedBuffers.end())
		{
			model.bufferViews[accessor.bufferView].byteOffset += sizeDiff;
			updatedBuffers.insert(accessor.bufferView);
		}
	}

	for (const tinygltf::Image& image : model.images)
	{
		if (model.bufferViews[image.bufferView].byteOffset > initialOffset && updatedBuffers.find(image.bufferView) == updatedBuffers.end())
		{
			model.bufferViews[image.bufferView].byteOffset += sizeDiff;
			updatedBuffers.insert(image.bufferView);
		}
	}
}

size_t MeshProcessor::GetComponentTypeSize(const int componentType)
{
	switch (componentType)
	{
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
		return sizeof(unsigned char);
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		return sizeof(unsigned short);
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
		return sizeof(unsigned int);
	case TINYGLTF_COMPONENT_TYPE_BYTE:
		return sizeof(char);
	case TINYGLTF_COMPONENT_TYPE_SHORT:
		return sizeof(short);
	case TINYGLTF_COMPONENT_TYPE_INT:
		return sizeof(int);
	default:
		return 0;
	}
}
