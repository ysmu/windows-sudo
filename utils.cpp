#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include "windefs.h"

int GetParentProcessId()
{
	PROCESSENTRY32 stPe32;
	stPe32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(hSnapshot, &stPe32))
	{
		do
		{
			if (stPe32.th32ProcessID == GetCurrentProcessId())
			{
				CloseHandle(hSnapshot);
				return stPe32.th32ParentProcessID;
			}
		} while (Process32Next(hSnapshot, &stPe32));
	}

	CloseHandle(hSnapshot);
	return -1;
}

bool GetProcessCurrentDirectory(wchar_t wszPath[MAX_PATH])
{
	ZeroMemory(wszPath, MAX_PATH * sizeof(wchar_t));

	//open parent process
	int ppid = GetParentProcessId();
	if (ppid < 0)
	{
		return false;
	}
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ppid);
	if (hProcess == NULL)
	{
		return false;
	}

	BOOL isWow64;
	IsWow64Process(GetCurrentProcess(), &isWow64);
	if (isWow64)
	{
		PEB_WOW64 stPEB;
		PROCESS_BASIC_INFORMATION_WOW64 stPbi;

		//read PEB
		NtQueryInformationProcessFunc NtQueryInformationProcess = (NtQueryInformationProcessFunc)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtWow64QueryInformationProcess64");
		if (NtQueryInformationProcess == NULL)
		{
			CloseHandle(hProcess);
			return false;
		}
		if (NtQueryInformationProcess(hProcess, ProcessBasicInformation, &stPbi, sizeof PROCESS_BASIC_INFORMATION_WOW64, NULL) != 0)
		{
			CloseHandle(hProcess);
			return false;
		}
		NtWow64ReadVirtualMemory64Func NtWow64ReadVirtualMemoryWow64 = (NtWow64ReadVirtualMemory64Func)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWow64ReadVirtualMemory64");
		if (NtWow64ReadVirtualMemoryWow64(hProcess, stPbi.PebBaseAddress, &stPEB, sizeof PEB_WOW64, NULL) != 0)
		{
			CloseHandle(hProcess);
			return false;
		}

		RTL_USER_PROCESS_PARAMETERS_WOW64 stUPP;
		if (NtWow64ReadVirtualMemoryWow64(hProcess, stPEB.ProcessParameters, &stUPP, sizeof RTL_USER_PROCESS_PARAMETERS_WOW64, NULL) != 0)
		{
			CloseHandle(hProcess);
			return false;
		}
		if (NtWow64ReadVirtualMemoryWow64(hProcess, stUPP.CurrentDirectory.DosPath.Buffer, wszPath, stUPP.CurrentDirectory.DosPath.Length, NULL) != 0)
		{
			CloseHandle(hProcess);
			return false;
		}
	}
	else
	{
		PEB stPEB;
		PROCESS_BASIC_INFORMATION stPbi;

		//read PEB
		NtQueryInformationProcessFunc NtQueryInformationProcess = (NtQueryInformationProcessFunc)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess");
		if (NtQueryInformationProcess == NULL)
		{
			CloseHandle(hProcess);
			return false;
		}
		if (NtQueryInformationProcess(hProcess, ProcessBasicInformation, &stPbi, sizeof PROCESS_BASIC_INFORMATION, NULL) != 0)
		{
			CloseHandle(hProcess);
			return false;
		}
		if (ReadProcessMemory(hProcess, stPbi.PebBaseAddress, &stPEB, sizeof PEB, NULL) == 0)
		{
			CloseHandle(hProcess);
			return false;
		}

		RTL_USER_PROCESS_PARAMETERS stUPP;
		if (ReadProcessMemory(hProcess, stPEB.ProcessParameters, &stUPP, sizeof RTL_USER_PROCESS_PARAMETERS, NULL) == 0)
		{
			CloseHandle(hProcess);
			return false;
		}
		if (ReadProcessMemory(hProcess, stUPP.CurrentDirectory.DosPath.Buffer, wszPath, stUPP.CurrentDirectory.DosPath.Length, NULL) == 0)
		{
			CloseHandle(hProcess);
			return false;
		}
	}
	CloseHandle(hProcess);
	return true;
}