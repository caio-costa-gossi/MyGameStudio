#include "SystemsInfoHelper.h"
#include <Windows.h>

uint64_t SystemsInfoHelper::GetScreenWidth()
{
	return static_cast<uint64_t>(GetSystemMetrics(SM_CXSCREEN));
}

uint64_t SystemsInfoHelper::GetScreenHeight()
{
	return static_cast<uint64_t>(GetSystemMetrics(SM_CYSCREEN));
}