#pragma once
#include "BaseManager.h"
#include "Err.h"

class TestManager : public BaseManager
{
public:
	Err Startup() override;
};
