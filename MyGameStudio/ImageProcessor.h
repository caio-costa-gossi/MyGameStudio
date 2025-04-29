#pragma once
#include "Asset.h"
#include "Err.h"

class ImageProcessor
{
private:
	Err DecompressImage();

public:
	ImageProcessor();

	uint8_t* ProcessImage(const Asset& imageMetadata, const uint8_t* fileBuffer);
};
