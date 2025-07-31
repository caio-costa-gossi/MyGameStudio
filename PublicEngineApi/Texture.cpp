#include "Texture.h"
#include <glad/glad.h>

#include "AssetRuntimeManager.h"
#include "GameConsoleManager.h"

Err Texture::Init(const uint32_t assetId)
{
	// Load image into memory and decompress
	textureImage_ = AssetRuntimeManager::LoadImg(assetId);
	if (textureImage_ == nullptr)
		return error_const::RENDER_TEXTURE_INIT_FAIL;

	// Upload to GPU and get textureId_
	Err err = InitGlTex();
	if (err.Code())
		return err;

	// Free image from memory
	err = AssetRuntimeManager::FreeImg(assetId);
	if (err.Code())
		return err;

	textureImage_ = nullptr;
	isInit_ = true;

	return error_const::SUCCESS;
}

Err Texture::Init(const char* filepath, uint32_t& assetId)
{
	// Load image into memory and decompress
	textureImage_ = AssetRuntimeManager::LoadImg(filepath, assetId);
	if (textureImage_ == nullptr)
		return error_const::RENDER_TEXTURE_INIT_FAIL;

	// Upload to GPU and get textureId_
	Err err = InitGlTex();
	if (err.Code())
		return err;

	// Free image from memory
	err = AssetRuntimeManager::FreeImg(assetId);
	if (err.Code())
		return err;

	textureImage_ = nullptr;
	isInit_ = true;

	return error_const::SUCCESS;
}

void Texture::Use(const uint8_t textureChannel) const
{
	if (!isInit_)
	{
		GameConsoleManager::PrintError("Texture was not initialized yet! No way to use it.");
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0 + textureChannel);
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