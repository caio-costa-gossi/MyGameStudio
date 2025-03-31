#pragma once
#include "Err.h"
#include "TestManager.h"

class EngineCoreManager
{
private:
	static TestManager testManager_;

public:
	EngineCoreManager() = delete;

	static Err Startup();
};
