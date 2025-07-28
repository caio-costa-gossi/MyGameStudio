#include "Texture.h"
#include <glad/glad.h>

#include "AssetRuntimeManager.h"

Err Texture::Init(const uint32_t assetId)
{
	textureImage_ = AssetRuntimeManager::LoadImg(assetId);
	if (textureImage_ == nullptr)
		return error_const::RENDER_TEXTURE_INIT_FAIL;

	Err err = InitGlTex();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

void Texture::Use() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, textureId_);
}

Err Texture::InitGlTex()
{
	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_2D, textureId_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage_->Width, textureImage_->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage_->Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return error_const::SUCCESS;
}
