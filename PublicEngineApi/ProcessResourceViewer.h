#pragma once
#include <cstdint>
#include <Windows.h>
#include <Psapi.h>
#include "Err.h"

class ProcessResourceViewer
{
private:
	PROCESS_INFORMATION processInfo_;
	uint8_t numProcessors_;
	uint64_t lastCpuMeasure_;
	uint64_t lastSystemMeasure_;

	Err InitCpuUsage();
	uint64_t FtimeToUint64(FILETIME ftime) const;

public:
	explicit ProcessResourceViewer(const PROCESS_INFORMATION& processInfo);
	int64_t GetRamUsage() const;
	float GetCpuUsage();
};
