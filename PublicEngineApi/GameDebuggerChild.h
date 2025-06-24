#pragma once
#include "Err.h"

// Class for child process to manage debugging tasks
class GameDebuggerChild
{
public:
	static Err InitDebuggerPipe(int argc, char** argv);
	static Err SendInfo();
	static Err Shutdown();
};