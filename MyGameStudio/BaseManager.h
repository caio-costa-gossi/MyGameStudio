#pragma once
#include "Err.h"

class BaseManager
{
public:
	BaseManager() = default;
	virtual ~BaseManager() = default;

	virtual Err Startup() = 0;
};