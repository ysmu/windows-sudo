#include <Windows.h>
#include <tchar.h>
#include "utils.h"
#include "resource.h"

#define	MAX_LOAD_STRING		1024

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	TCHAR *pFilename = lpCmdLine;
	if (*pFilename == '\0')
	{
		return -1;
	}

	TCHAR *pArgs = NULL;
	for(; *lpCmdLine != '\0'; lpCmdLine++)
	{
		if (*lpCmdLine == ' ')
		{
			*lpCmdLine = '\0';
			pArgs = lpCmdLine + 1;
			break;
		}
	}

	//We want to use the same current directory as our calling parent
	//This way, when we do a `sudo cmd` in a cmd prompt,
	//we can get a privileged cmd prompt in the same directory,
	//instead of in the default %WINDIR%\system32 (SysWoW64),
	wchar_t wszCurrentDirectory[MAX_PATH];
	if (GetProcessCurrentDirectory(wszCurrentDirectory))
	{
		SetCurrentDirectoryW(wszCurrentDirectory);
	}
	else
	{
		//somehow, we're unable to retrieve our parent's current directory
		//resort to user's home directory
		if (GetEnvironmentVariable(L"USERPROFILE", wszCurrentDirectory, MAX_PATH) <= MAX_PATH)
		{
			SetCurrentDirectoryW(wszCurrentDirectory);
		}
	}

	HINSTANCE hInst = ShellExecute(NULL, _T("open"), pFilename, pArgs, NULL, SW_SHOW);
	if ((long long)hInst > 32)
	{
		return 0;
	}
	else
	{
		//something went wrong
		int errorCode = reinterpret_cast<int>(hInst);

		TCHAR szMsgBoxTitle[MAX_LOAD_STRING];
		LoadString(GetModuleHandle(NULL), IDS_ERROR, szMsgBoxTitle, MAX_LOAD_STRING);

		TCHAR szError[MAX_LOAD_STRING];
		if (LoadString(GetModuleHandle(NULL), errorCode, szError, MAX_LOAD_STRING) == 0)
		{
			LoadString(GetModuleHandle(NULL), IDS_UNKNOWN_ERROR, szError, MAX_LOAD_STRING);
		}
		MessageBox(NULL, szError, szMsgBoxTitle, MB_ICONERROR);
		return -errorCode;
	}
}