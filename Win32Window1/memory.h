#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>


// name = window name
DWORD getPID(LPCSTR name)
{
	HWND hwnd = FindWindow(0, name);

	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	return pid;
}

// pid = process id, name = module name
DWORD modBase(DWORD pid, LPCSTR name)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry = { 0 };
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{
			do
			{		
				if (!strcmp(modEntry.szModule, name))
				{
					CloseHandle(hSnap);
					return (DWORD)modEntry.modBaseAddr;
				}
			}
			while (Module32Next(hSnap, &modEntry));
		}
	}

	CloseHandle(hSnap);

	return 0;
}

// base = module base, address = first offset, offset = used to find pointer, pid = process id
DWORD ptrAddr(DWORD base, DWORD address, std::vector<DWORD> offsets, DWORD pid)
{
	// in CE "base" + address
	DWORD start = base + address;

	HANDLE process = OpenProcess(PROCESS_VM_READ, 0, pid);

	for (auto& i : offsets)
	{
		ReadProcessMemory(process, (void*)start, &start, sizeof(start), 0);
		start += i;
	}

	CloseHandle(process);

	return start;
}


// start = address to start, pid = process id
template<class T>
T readMem(DWORD start, DWORD pid)
{
	T buffer = 0;
	HANDLE process = OpenProcess(PROCESS_VM_READ, 0, pid);
	ReadProcessMemory(process, (LPCVOID)start, &buffer, sizeof(buffer), 0);
	CloseHandle(process);
	return buffer;
}


// maybe later used for pattern scan
std::vector<std::byte> readRegion(DWORD start, DWORD size, DWORD pid)
{
	std::vector<std::byte> buffer(size);

	HANDLE process = OpenProcess(PROCESS_VM_READ, 0, pid);
	ReadProcessMemory(process, (LPCVOID)start, buffer.data(), size, 0);
	CloseHandle(process);

	return buffer;
}