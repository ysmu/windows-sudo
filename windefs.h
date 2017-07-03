#pragma once

typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation = 0,
	ProcessDebugPort = 7,
	ProcessWow64Information = 26,
	ProcessImageFileName = 27,
	ProcessBreakOnTermination = 29
} PROCESSINFOCLASS;

// ======================
// For programs whose bitness is the same as that of the kernel
// i.e. 32bit program on 32bit kernel, 64bit program on 64 bit kernel
// ======================
// we only need the structure up to ProcessParameters
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;

typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	PVOID Handle;
} CURDIR, *PCURDIR;

//we only need the structure up to CurrentDirectory
typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	ULONG MaximumLength;
	ULONG Length;
	ULONG Flags;
	ULONG DebugFlags;
	PVOID ConsoleHandle;
	PVOID ConsoleFlags;
	PVOID StandardInput;
	PVOID StandardOutput;
	PVOID StandardError;
	CURDIR CurrentDirectory;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

//ref: http://terminus.rewolf.pl/terminus/structures/ntdll/_PEB_combined.html
//we only need the structure up to ProcessParameters
typedef struct _PEB {
	BYTE InheritedAddressSpace;
	BYTE ReadImageFileExecOptions;
	BYTE BeingDebugged;
	BYTE Reserved;
	PVOID Mutant;					/* +0x4 */
	PVOID ImageBaseAddress;			/* +0x8 */
	PVOID Ldr;						/* +0xc */
	PVOID ProcessParameters;		/* +0x10 */
} PEB;

typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	PVOID PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;

typedef NTSTATUS(NTAPI* NtQueryInformationProcessFunc)(
	IN HANDLE ProcessHandle,
	ULONG ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);

typedef NTSTATUS(NTAPI* NtReadVirtualMemoryFunc)(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	OUT PVOID Buffer,
	IN SIZE_T Size,
	OUT PSIZE_T NumberOfBytesRead
);

// ======================
// For 32bit programs under 64bit kernels (Wow64)
// ======================
// we only need the structure up to ProcessParameters
typedef struct _UNICODE_STRING_WOW64 {
	USHORT Length;
	USHORT MaximumLength;
	DWORD _padding;
	PVOID64 Buffer;
} UNICODE_STRING_WOW64;

typedef struct _CURDIR_WOW64
{
	UNICODE_STRING_WOW64 DosPath;
	ULONG Handle;
} CURDIR_WOW64, *PCURDIR_WOW64;

//we only need the structure up to CurrentDirectory
typedef struct _RTL_USER_PROCESS_PARAMETERS_WOW64
{
	ULONG MaximumLength;
	ULONG Length;
	ULONG Flags;
	ULONG DebugFlags;
	PVOID64 ConsoleHandle;
	PVOID64 ConsoleFlags;
	PVOID64 StandardInput;
	PVOID64 StandardOutput;
	PVOID64 StandardError;
	CURDIR_WOW64 CurrentDirectory;
} RTL_USER_PROCESS_PARAMETERS_WOW64, *PRTL_USER_PROCESS_PARAMETERS_WOW64;

//ref: http://terminus.rewolf.pl/terminus/structures/ntdll/_PEB_combined.html
//we only need the structure up to ProcessParameters
typedef struct _PEB_WOW64 {
	BYTE InheritedAddressSpace;
	BYTE ReadImageFileExecOptions;
	BYTE BeingDebugged;
	BYTE Reserved;
	PVOID64 Mutant;					/* +0x4 */
	PVOID64 ImageBaseAddress;		/* +0x10 */
	PVOID64 Ldr;					/* +0x18 */
	PVOID64 ProcessParameters;		/* +0x20 */
} PEB_WOW64, *PPEB_WOW64;

typedef struct _PROCESS_BASIC_INFORMATION_WOW64 {
	PVOID64 Reserved1;
	PVOID64 PebBaseAddress;
	PVOID64 Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID64 Reserved3;
} PROCESS_BASIC_INFORMATION_WOW64;

typedef NTSTATUS(NTAPI* NtWow64ReadVirtualMemory64Func)(
	IN HANDLE ProcessHandle,
	IN PVOID64 BaseAddress,
	OUT PVOID Buffer,
	IN ULONG64 Size,
	OUT PULONG64 NumberOfBytesRead
);