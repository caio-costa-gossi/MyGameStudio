#pragma once
#include "Enums.h"
#include "Err.h"

// Class for child process to manage debugging tasks
class GameDebuggerChild
{
private:
	static enums::GameDebugType debugType_;
	static std::string debugInfo_;

	static void GetInputDebugInfo();

public:
	static Err InitDebuggerPipe(int argc, char** argv);
	static Err SendInfo();
	static Err Shutdown();
};