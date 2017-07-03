@echo off

reg query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set arch=86 || set arch=64

if %arch%==86 (
	sudo cmd /c "copy sudo.exe %WINDIR%\system32\sudo.exe"
)
if %arch%==64 (
	sudo cmd /c "copy sudo.exe %WINDIR%\SysWoW64\sudo.exe"
	sudo cmd /c "copy sudo64.exe %WINDIR%\system32\sudo.exe"
)