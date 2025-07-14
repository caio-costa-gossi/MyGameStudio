#include "Image.h"

#include "ImageLoader.h"

Image::Image(const char* filepath)
{
	Data = ImageLoader::Load(filepath, Width, Height, Channels);
}

Image::~Image()
{
	if (Data != nullptr)
		ImageLoader::Free(Data);
}

Image::Image(Image&& other) noexcept
{
	Data = other.Data;
	Width = other.Width;
	Height = other.Height;
	Channels = other.Channels;

	other.Data = nullptr;
}

Image& Image::operator=(Image&& other) noexcept
{
	Data = other.Data;
	Width = other.Width;
	Height = other.Height;
	Channels = other.Channels;

	other.Data = nullptr;

	return *this;
}
