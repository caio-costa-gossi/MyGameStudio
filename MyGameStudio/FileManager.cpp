#include "FileManager.h"
#include <fstream>

Err FileManager::Startup()
{
	workerThreadRunning_ = true;
	workerThread_ = std::thread(RunWorker);
	return error_const::SUCCESS;
}

Err FileManager::Shutdown()
{
	workerThreadRunning_ = false;
	cv_.notify_all();
	workerThread_.join();
	return error_const::SUCCESS;
}

FileIoTaskJanitor FileManager::ReadFileAsync(const uint8_t priority, const char* filePath, char** fileBuffer, const size_t bufferSize, void(*callback)(FileIoTask*))
{
	auto task = new FileIoTask(priority, filePath, fileBuffer, bufferSize, callback);
	fileTaskQueue_.push(task);
	cv_.notify_all();
	return FileIoTaskJanitor(task);
}

Err FileManager::RunWorker()
{
	while (workerThreadRunning_ || !fileTaskQueue_.empty())
	{
		std::unique_lock<std::mutex> lock(mutex_);

		if (fileTaskQueue_.empty())
			cv_.wait(lock, [] { return !fileTaskQueue_.empty() || !workerThreadRunning_; });

		if (workerThreadRunning_ || !fileTaskQueue_.empty())
		{
			FileIoTask* nextTask = fileTaskQueue_.top();
			if (nextTask != nullptr) RunTask(nextTask);
			fileTaskQueue_.pop();
		}
	}

	return error_const::SUCCESS;
}

Err FileManager::RunTask(FileIoTask* task)
{
	auto file = std::ifstream(task->FilePath, std::ios::in);

	if (task->BufferSize > std::numeric_limits<std::streamsize>::max())
	{
		task->TaskState = enums::IoTaskState::error;
		task->ErrorDescription = "Buffer size is too big";
		return error_const::GENERIC_EXCEPTION;
	}

	file.read(*(task->StreamBuffer), static_cast<std::streamsize>(task->BufferSize));

	if (task->Callback != nullptr) task->Callback(task);
	task->TaskState = enums::IoTaskState::finished;

	return error_const::SUCCESS;
}

Err FileManager::WinSeparatorToUnix(std::string& string)
{
	for (size_t i = 0; i < string.size(); ++i)
	{
		if (string[i] == '\\')
			string.replace(i, 1, "/");
	}

	return error_const::SUCCESS;
}


auto FileManager::fileTaskQueue_ = std::priority_queue<FileIoTask*, std::vector<FileIoTask*>, FileIoTaskCompare>();
auto FileManager::workerThreadRunning_ = false;
std::mutex FileManager::mutex_;
std::condition_variable FileManager::cv_;
std::thread FileManager::workerThread_;