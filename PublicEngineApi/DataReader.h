#pragma once
#include <cstdint>

struct DataReader
{
	uint64_t DataPointer = 0;
	const uint8_t* Data;
	uint64_t MaxSize = 0;

	DataReader(const uint8_t* data, const uint64_t maxSize) : Data(data), MaxSize(maxSize) { }

	Err Read(uint8_t* destination, const uint64_t size)
	{	
		if (DataPointer >= MaxSize) 
			return error_const::BUFFER_OVERFLOW;

		memcpy_s(destination, size, &Data[DataPointer], size);
		DataPointer += size;

		return error_const::SUCCESS;
	}
};
