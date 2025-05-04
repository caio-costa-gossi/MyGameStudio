#include "GltfModel.h"

GltfModel::GltfModel(const std::string& filepath)
{
	loader_.LoadBinaryFromFile(&model_, &err_, nullptr, filepath);

	//model_.nodes
}
