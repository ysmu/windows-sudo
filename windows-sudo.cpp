#include <tchar.h>
#include <Windows.h>
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

	HINSTANCE hInst = ShellExecute(NULL, _T("open"), pFilename, pArgs, NULL, SW_SHOW);
	if ((int)hInst > 32)
	{
		return 0;
	}
	else
	{
		//something went wrong
		TCHAR szMsgBoxTitle[MAX_LOAD_STRING];
		LoadString(GetModuleHandle(NULL), IDS_ERROR, szMsgBoxTitle, MAX_LOAD_STRING);

		TCHAR szError[MAX_LOAD_STRING];
		if (LoadString(GetModuleHandle(NULL), (UINT)hInst, szError, MAX_LOAD_STRING) == 0)
		{
			LoadString(GetModuleHandle(NULL), IDS_UNKNOWN_ERROR, szError, MAX_LOAD_STRING);
		}
		MessageBox(NULL, szError, szMsgBoxTitle, MB_ICONERROR);
		return -(int)hInst;
	}
}

