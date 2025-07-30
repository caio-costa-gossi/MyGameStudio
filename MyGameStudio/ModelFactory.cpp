#include "ModelFactory.h"

#include "Asset.h"
#include "AssetDatabase.h"
#include "ConfigManager.h"
#include "ConsoleManager.h"
#include "DataStream.h"
#include "Image.h"
#include "ImageLoader.h"
#include "SystemPathHelper.h"
#include "VertexIndexExtractor.h"
#include "ZipFile.h"

Model ModelFactory::CreateModel(const tinygltf::Model& model, const Asset& modelMetadata)
{
	Model newModel;
	auto extractor = VertexIndexExtractor(model);

	std::unique_ptr<Mesh[]> meshData;
	uint32_t meshCount;

	// Extract all model information from .glb, import all texture images
	Err err = extractor.ExtractVerticesIndices(meshData, meshCount, modelMetadata);
	if (err.Code())
		ConsoleManager::PrintError(err);

	// Get future model ID to save it before committing the data to the asset pipeline
	err = AssetDatabase::FindAssetId(modelMetadata.SourceLocation.c_str(), newModel.ModelId);
	if (err.Code())
		ConsoleManager::PrintError("Error getting future model ID: " + err.Message());

	newModel.MeshCount = meshCount;
	newModel.Meshes = std::move(meshData);

	return newModel;
}