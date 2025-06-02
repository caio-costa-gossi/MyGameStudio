#include "ProcessResourceViewer.h"
#include "Err.h"

ProcessResourceViewer::ProcessResourceViewer(const PROCESS_INFORMATION& processInfo):
	processInfo_(processInfo)
{
	InitCpuUsage();

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	numProcessors_ = static_cast<uint8_t>(sysInfo.dwNumberOfProcessors);
}

int64_t ProcessResourceViewer::GetRamUsage() const
{
	PROCESS_MEMORY_COUNTERS pmc;

	if (GetProcessMemoryInfo(processInfo_.hProcess, &pmc, sizeof(pmc)))
		return static_cast<int64_t>(pmc.WorkingSetSize);

	return -1;
}

float ProcessResourceViewer::GetCpuUsage()
{
	FILETIME creationTime, exitTime, kernelTime, userTime, systemTime;

	GetSystemTimeAsFileTime(&systemTime);
	const uint64_t newSystemMeasure = FtimeToUint64(systemTime);

	if (GetProcessTimes(processInfo_.hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) 
	{
		const uint64_t newMeasure = FtimeToUint64(kernelTime) + FtimeToUint64(userTime);
		const uint64_t systemDelta = newSystemMeasure - lastSystemMeasure_;
		const uint64_t processDelta = newMeasure - lastCpuMeasure_;

		lastSystemMeasure_ = newSystemMeasure;
		lastCpuMeasure_ = newMeasure;

		return static_cast<float>(processDelta) / static_cast<float>(systemDelta * numProcessors_) * 100;
	}

	return -1;
}

Err ProcessResourceViewer::InitCpuUsage()
{
	FILETIME creationTime, exitTime, kernelTime, userTime, systemTime;

	GetSystemTimeAsFileTime(&systemTime);
	lastSystemMeasure_ = FtimeToUint64(systemTime);

	if (GetProcessTimes(processInfo_.hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
		lastCpuMeasure_ = FtimeToUint64(kernelTime) + FtimeToUint64(userTime);

	return error_const::SUCCESS;
}

uint64_t ProcessResourceViewer::FtimeToUint64(FILETIME ftime) const
{
	ULARGE_INTEGER measure;
	measure.HighPart = ftime.dwHighDateTime;
	measure.LowPart = ftime.dwLowDateTime;
	return measure.QuadPart;
}