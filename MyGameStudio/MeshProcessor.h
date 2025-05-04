#pragma once
#include <cstdint>
#include "Asset.h"
#include "Err.h"
#include "GltfModel.h"

class MeshProcessor
{
private:
	static Err CompressIndices(tinygltf::Model& model);

public:
	static uint8_t* ProcessMesh(const Asset& metadata, uint64_t& resultSize);
};
