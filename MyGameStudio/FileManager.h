#pragma once
#include <condition_variable>
#include <cstdint>
#include <queue>

#include "Enums.h"
#include "Err.h"

struct FileIoTask
{
	uint8_t Priority;
	char FilePath[255];
	char** StreamBuffer;
	size_t BufferSize;
	enums::IoTaskState TaskState = enums::IoTaskState::pending;
	std::string ErrorDescription;
	void (*Callback) (FileIoTask*);

	FileIoTask(const uint8_t priority, const char* filePath, char** streamBuffer, const size_t bufferSize, void(*callback)(FileIoTask*)) : Priority(priority), StreamBuffer(streamBuffer), BufferSize(bufferSize), Callback(callback)
	{
		strncpy_s(FilePath, 255, filePath, _TRUNCATE);
	}

	bool operator<(const FileIoTask& other) const
	{
		return Priority < other.Priority;
	}

	bool operator>(const FileIoTask& other) const
	{
		return Priority > other.Priority;
	}
};

struct FileIoTaskCompare
{
	bool operator()(const FileIoTask* a, const FileIoTask* b) const
	{
		return a->Priority > b->Priority;
	}
};

class FileIoTaskJanitor
{
private:
	FileIoTask* task_;

public:
	explicit FileIoTaskJanitor(FileIoTask* task) : task_(task) { }
	~FileIoTaskJanitor()
	{
		while (true)
		{
			if (task_->TaskState == enums::IoTaskState::error || task_->TaskState == enums::IoTaskState::finished) 
				break;
		}

		delete task_;
	}

	const FileIoTask& GetTask() const { return *task_; }

	FileIoTaskJanitor() = delete;
	FileIoTaskJanitor(const FileIoTaskJanitor&) = delete;
	void operator=(const FileIoTaskJanitor&) = delete;
	FileIoTaskJanitor(FileIoTaskJanitor&& other) noexcept { task_ = other.task_; other.task_ = nullptr; }
	FileIoTaskJanitor& operator=(FileIoTaskJanitor&& other) noexcept { task_ = other.task_; other.task_ = nullptr; return *this; }
};

class FileManager
{
private:
	static std::priority_queue<FileIoTask*, std::vector<FileIoTask*>, FileIoTaskCompare> fileTaskQueue_;
	static bool workerThreadRunning_;
	static std::mutex mutex_;
	static std::condition_variable cv_;
	static std::thread workerThread_;

	static Err RunWorker();
	static Err RunTask(FileIoTask* task);

public:
	static Err Startup();
	static Err Shutdown();

	static FileIoTaskJanitor ReadFileAsync(const uint8_t priority, const char* filePath, char** fileBuffer, size_t bufferSize, void(*callback)(FileIoTask*) = nullptr);
};