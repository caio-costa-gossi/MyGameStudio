#pragma once
#include <cstdint>
#include "Asset.h"
#include "Err.h"
#include <tiny_gltf.h>

class ModelProcessor
{
private:
	static Err CompressIndices(tinygltf::Model& model);
	static Err VerifyModel(const tinygltf::Model& model);
	static Err Triangulate(tinygltf::Model& model);

	static uint8_t* GetFileBuffer(tinygltf::TinyGLTF& loader, const tinygltf::Model& model, uint64_t& resultSize);

	static void ChangeBuffer(std::vector<uint8_t>& oldData, uint64_t oldDataByteCount, const std::vector<uint8_t>& newData, size_t initialOffset, tinygltf::Model& model);
	static size_t GetComponentTypeSize(int componentType);

public:
	static uint8_t* ProcessModel(const Asset& metadata, uint64_t& resultSize, std::string& errMsg);
};
