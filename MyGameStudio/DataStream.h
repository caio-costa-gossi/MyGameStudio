#pragma once
#include <cstdint>
#include <cstring>

#include "Err.h"

struct DataStream
{
	uint8_t* Data;
	uint64_t DataPointer;
	uint64_t DataSize;

	DataStream()
	{
		Data = nullptr;
		DataPointer = 0;
		DataSize = 0;
	}

	explicit DataStream(const uint64_t dataSize, const bool initToZero = true) : DataSize(dataSize)
	{
		Data = new uint8_t[dataSize];
		DataPointer = 0;
		if (initToZero) memset(Data, 0, dataSize);
	}

	~DataStream()
	{
		delete[] Data;
	}

	Err InitStream(const uint64_t dataSize, const bool initToZero = true)
	{
		DataSize = dataSize;
		Data = new uint8_t[dataSize];
		DataPointer = 0;
		if (initToZero) memset(Data, 0, dataSize);

		return error_const::SUCCESS;
	}

	Err Write(const uint8_t* source, const uint64_t size)
	{
		if (DataPointer >= DataSize) return error_const::BUFFER_OVERFLOW;

		memcpy_s(&Data[DataPointer], size, source, size);
		DataPointer += size;

		return error_const::SUCCESS;
	}

	Err Write(const void* source, const uint64_t size)
	{
		if (DataPointer >= DataSize) return error_const::BUFFER_OVERFLOW;

		memcpy_s(&Data[DataPointer], size, source, size);
		DataPointer += size;

		return error_const::SUCCESS;
	}

	DataStream(const DataStream& other) = delete;
	DataStream& operator= (const DataStream& other) = delete;

	DataStream(DataStream&& other) noexcept
	{
		DataPointer = other.DataPointer;
		DataSize = other.DataSize;
		Data = other.Data;

		other.DataPointer = 0;
		other.DataSize = 0;
		other.Data = nullptr;
	}
	DataStream& operator=(DataStream&& other) noexcept
	{
		DataPointer = other.DataPointer;
		DataSize = other.DataSize;
		Data = other.Data;

		other.DataPointer = 0;
		other.DataSize = 0;
		other.Data = nullptr;

		return *this;
	}
};
