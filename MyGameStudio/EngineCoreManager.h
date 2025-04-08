#pragma once
#include "Err.h"
#include "LocalizationManager.h"
#include "TestManager.h"

class EngineCoreManager
{
private:

public:
	EngineCoreManager() = delete;

	static Err Startup();
};
