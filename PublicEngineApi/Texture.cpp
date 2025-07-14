#include "Texture.h"
#include <glad/glad.h>

Texture::Texture()
{
	
}

Err Texture::Init(const char* filename)
{
	
}

void Texture::Use() const
{
	glBindTexture(GL_TEXTURE_2D, textureId_);
}
