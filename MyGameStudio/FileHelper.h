#pragma once
#include <cstdint>
#include <queue>

#include "Err.h"

struct FileioTask
{
	uint8_t Priority;
	char FilePath[255];
	char** StreamBuffer;

	bool operator<(const FileioTask& other) const
	{
		return Priority < other.Priority;
	}

	bool operator>(const FileioTask& other) const
	{
		return Priority > other.Priority;
	}
};

class FileHelper
{
private:
	static std::priority_queue<FileioTask> gFileTaskQueue_;

public:
	static Err Startup();
};