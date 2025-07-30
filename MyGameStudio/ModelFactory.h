#pragma once
#include <memory>
#include <tiny_gltf.h>

#include "Asset.h"
#include "Err.h"
#include "Mesh.h"
#include "Model.h"

class ModelFactory
{
public:
	static Model CreateModel(const tinygltf::Model& model, const Asset& modelMetadata);
};
