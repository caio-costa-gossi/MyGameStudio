#pragma once
#include <unordered_map>

#include "Mesh.h"
#include "Model.h"
#include "Texture.h"

using TextureList = std::unordered_map<uint32_t, Texture>;
using AttributeList = std::unordered_map<uint32_t, std::vector<uint32_t>>;

class DrawingCache
{
private:
	static TextureList textures_;
	static AttributeList attributeMap_;

public:
	static Err StoreTexture(uint32_t textureAssetId);
	static Texture& GetTexture(uint32_t textureAssetId);
	static const std::vector<uint32_t>& GetModelVaoList(const Model& model);
	static uint32_t GetBillboardVao(uint32_t bboardImageId);
};