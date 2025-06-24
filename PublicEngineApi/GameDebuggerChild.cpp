#include "GameDebuggerChild.h"
#include "Enums.h"
#include "GameConsoleManager.h"
#include "Win32PipeManager.h"

Err GameDebuggerChild::InitDebuggerPipe(const int argc, char** argv)
{
	if (argc <= 2)
		return error_const::INVALID_PARAMETERS;

	const enums::GameDebugType debugType = static_cast<enums::GameDebugType>(std::strtol(argv[1], nullptr, 10));
	GameConsoleManager::PrintInfo("Debug initialized. DebugType selected: " + std::to_string(debugType));

	const std::string pipeName = argv[2];
	GameConsoleManager::PrintInfo("Debug pipe name: " + pipeName);

	const HANDLE hPipe = Win32PipeManager::GetPipeHandleFromName(pipeName);

	Err err = Win32PipeManager::SetPipeHandle(hPipe);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err GameDebuggerChild::SendInfo()
{
	Err err = Win32PipeManager::SendPipeMessage("Hello world!\n");
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err GameDebuggerChild::Shutdown()
{
	Err err = Win32PipeManager::ClosePipeHandle();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}
