#include "TerminalFactory.h"
#include "Err.h"

NewTerminal TerminalFactory::CreateTerminal(const CreateTerminalInfo& info, const bool generateCommPipe)
{
	// Communication pipe handles
	HANDLE hReadFrom = nullptr, hWriteTo = nullptr;
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, true };

	DWORD flags = STARTF_USEPOSITION | STARTF_USESIZE;

	if (generateCommPipe)
	{
		CreatePipe(&hReadFrom, &hWriteTo, &sa, false);
		flags |= STARTF_USESTDHANDLES;
	}

	STARTUPINFOA si = 
	{
		sizeof(si),
		nullptr,
		nullptr,
		const_cast<LPSTR>(info.WindowLabel.c_str()),
		static_cast<DWORD>(info.XPos),
		static_cast<DWORD>(info.YPos),
		static_cast<DWORD>(info.XSize),
		static_cast<DWORD>(info.YSize),
		0,
		0,
		0,
		flags,
		0,
		0,
		nullptr,
		generateCommPipe ? hReadFrom : nullptr,
		nullptr,
		nullptr
	};

	PROCESS_INFORMATION pi;

	CreateProcessA(
		nullptr,
		const_cast<LPSTR>(info.TerminalCmd.c_str()),
		nullptr,
		nullptr,
		generateCommPipe,
		CREATE_NEW_CONSOLE,
		nullptr,
		nullptr,
		&si,
		&pi
	);

	return NewTerminal { pi, hWriteTo };
}
