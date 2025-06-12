#pragma once
#include "Asset.h"
#include "Err.h"

class UserScriptProcessor
{
private:
	static std::string registerClassText_;

	static Err LoadFile(const std::string& filepath, uint8_t* fileBuffer, uint64_t bufferSize);

public:
	static uint8_t* ProcessScript(const Asset& metadata, uint64_t& resultSize, bool ctrlFlag = false);
	static uint8_t* ProcessHeader(const Asset& metadata, uint64_t& resultSize);
};
