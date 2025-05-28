#pragma once
#include <string>

struct CallbackData
{
	uint16_t EventId;
	std::string EventDescription;
	void* Param1;
	void* Param2;
	void* Param3;
};
