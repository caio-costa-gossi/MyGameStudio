#pragma once
#include <chrono>
#include "Err.h"
#include "LocalizationManager.h"

class EngineCoreManager
{
private:
	static std::chrono::time_point<std::chrono::steady_clock> engineBeginTime_;
	static std::chrono::time_point<std::chrono::steady_clock> engineFinishTime_;

public:
	EngineCoreManager() = delete;

	static Err Startup();
	static Err Config();
	static Err Shutdown();
};
