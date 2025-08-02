#include "DrawingCache.h"

#include "GameConsoleManager.h"
#include "Model.h"
#include "VaoFactory.h"

const std::vector<uint32_t>& DrawingCache::GetModelVaoList(const Model& model)
{
	if (attributeMap_.find(model.ModelId) == attributeMap_.end())
	{
		std::vector<uint32_t> vaoList;

		for (uint32_t i = 0; i < model.MeshCount; ++i)
		{
			uint32_t vao;

			Err err = VaoFactory::NewAttribObject(model.Meshes[i], vao);
			if (err.Code())
			{
				GameConsoleManager::PrintError(err);
				continue;
			}

			vaoList.push_back(vao);
		}

		attributeMap_[model.ModelId] = vaoList;
	}

	return attributeMap_.at(model.ModelId);
}

uint32_t DrawingCache::GetBillboardVao(const uint32_t bboardImageId)
{
	if (attributeMap_.find(bboardImageId) == attributeMap_.end())
	{
		uint32_t vao;

		Err err = VaoFactory::NewBillboardAttribObject(vao);
		if (err.Code())
		{
			GameConsoleManager::PrintError(err);
			return 0;
		}

		const std::vector<uint32_t> vaoList = { vao };
		attributeMap_[bboardImageId] = vaoList;
	}

	return attributeMap_[bboardImageId][0];
}

Err DrawingCache::StoreTexture(const uint32_t textureAssetId)
{
	if (textures_.find(textureAssetId) == textures_.end())
	{
		Texture newTexture;

		Err err = newTexture.Init(textureAssetId);
		if (err.Code())
			return err;

		textures_[textureAssetId] = newTexture;
	}

	return error_const::SUCCESS;
}

Texture& DrawingCache::GetTexture(const uint32_t textureAssetId)
{
	return textures_[textureAssetId];
}


TextureList DrawingCache::textures_;
AttributeList DrawingCache::attributeMap_;