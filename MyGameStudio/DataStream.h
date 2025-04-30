#pragma once
#include <cstdint>
#include <cstring>

#include "Err.h"

struct DataStream
{
	uint8_t* Data;
	uint64_t DataPointer;
	uint64_t DataSize;

	explicit DataStream(const uint64_t dataSize, const bool initToZero = true) : DataSize(dataSize)
	{
		Data = new uint8_t[dataSize];
		DataPointer = 0;
		if (initToZero) memset(Data, 0, dataSize);
	}

	void Write(const uint8_t* source, const uint64_t size)
	{
		memcpy_s(&Data[DataPointer], size, source, size);
		DataPointer += size;
	}

	void Write(const void* source, const uint64_t size)
	{
		memcpy_s(&Data[DataPointer], size, source, size);
		DataPointer += size;
	}
};
