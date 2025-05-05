#pragma once
#include <cstdint>
#include "Asset.h"
#include "Err.h"
#include "GltfModel.h"

class MeshProcessor
{
private:
	static Err CompressIndices(tinygltf::Model& model);
	static Err VerifyModel(const tinygltf::Model& model);
	static Err Triangulate(tinygltf::Model& model);

	static uint8_t* GetFileBuffer(tinygltf::TinyGLTF& loader, const tinygltf::Model& model, uint64_t& resultSize);

	static void ChangeBuffer(uint8_t* buffer, const std::vector<uint8_t>& newData, size_t initialOffset);

public:
	static uint8_t* ProcessMesh(const Asset& metadata, uint64_t& resultSize, std::string& errMsg);
};
