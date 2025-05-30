#pragma once
#include <string>
#include <windows.h>

struct NewTerminal
{
	PROCESS_INFORMATION ProcessInfo;
	HANDLE WriteHandle;
};

struct CreateTerminalInfo
{
	std::string TerminalCmd;
	std::string WindowLabel;
	uint64_t XPos;
	uint64_t YPos;
	uint64_t XSize;
	uint64_t YSize;
};

class TerminalFactory
{
private:


public:
	static NewTerminal CreateTerminal(const CreateTerminalInfo& info, bool generateCommPipe = false);
};
