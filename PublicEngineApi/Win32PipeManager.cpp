#include "Win32PipeManager.h"

HANDLE Win32PipeManager::CreatePipe(const std::string& unformattedPipeName, const bool multidirectional)
{
    const HANDLE hPipe = CreateNamedPipeA(
		(R"(\\.\pipe\)" + unformattedPipeName).c_str(),
        multidirectional ? PIPE_ACCESS_DUPLEX : PIPE_ACCESS_INBOUND,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        multidirectional ? 1024 : 0,
        1024,
        0,
        nullptr
    );

    return hPipe;
}

Err Win32PipeManager::SetPipeHandle(const HANDLE hPipe)
{
    hPipe_ = hPipe;
    return error_const::SUCCESS;
}

HANDLE Win32PipeManager::GetPipeHandleFromName(const std::string& unformattedPipeName, const bool isWrite)
{
    const HANDLE hPipe = CreateFileA(
        (R"(\\.\pipe\)" + unformattedPipeName).c_str(),
        isWrite ? GENERIC_WRITE : GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    return hPipe;
}

Err Win32PipeManager::SendPipeMessage(const std::string& message)
{
    if (hPipe_ == nullptr)
        return error_const::SETTING_MISSING;

    DWORD bytesWritten;

    if (!WriteFile(hPipe_, message.c_str(), message.size(), &bytesWritten, nullptr))
        return Err("Error writing to pipe!", static_cast<uint16_t>(GetLastError()));

    return error_const::SUCCESS;
}

std::string Win32PipeManager::ReceivePipeMessage()
{
    if (hPipe_ == nullptr)
        return "Pipe handle was not initialized";

    if (!connected_)
    {
	    connected_ = ConnectNamedPipe(hPipe_, nullptr) ? true : GetLastError() == ERROR_PIPE_CONNECTED;
        if (!connected_)
            return "Error connecting to pipe!";
    }

    char message[1024];
    DWORD bytesRead;

    if (!ReadFile(hPipe_, message, sizeof(message) - 1, &bytesRead, nullptr))
        return "Error reading from pipe!";

    message[bytesRead] = '\0';

    return { message };
}

Err Win32PipeManager::ClosePipeHandle()
{
    if (hPipe_ == nullptr)
        return error_const::INVALID_PARAMETERS;

    CloseHandle(hPipe_);

    return error_const::SUCCESS;
}


HANDLE Win32PipeManager::hPipe_;
bool Win32PipeManager::connected_ = false;