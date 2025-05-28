#pragma once
#include "Err.h"

class BaseRuntimeManager
{
public:
	BaseRuntimeManager() = default;
	virtual ~BaseRuntimeManager() = default;

	virtual Err Startup() = 0;
};