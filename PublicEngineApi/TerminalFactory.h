#pragma once
#include <string>
#include <windows.h>

class TerminalFactory
{
private:


public:
	static PROCESS_INFORMATION CreateTerminal(const std::string& command);
};
