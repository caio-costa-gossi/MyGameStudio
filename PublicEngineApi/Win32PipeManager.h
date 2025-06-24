#pragma once
#include <string>
#include <Windows.h>

#include "Err.h"

class Win32PipeManager
{
private:
	static HANDLE hPipe_;
	static bool connected_;

public:
	static HANDLE CreatePipe(const std::string& unformattedPipeName, bool multidirectional);
	static Err SetPipeHandle(HANDLE hPipe);

	static HANDLE GetPipeHandleFromName(const std::string& unformattedPipeName, bool isWrite = true);

	static Err SendPipeMessage(const std::string& message);
	static std::string ReceivePipeMessage();

	static Err ClosePipeHandle();
};
