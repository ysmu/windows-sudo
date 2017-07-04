@echo off

reg query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set arch=86 || set arch=64

::system32 gets redirected to SysWOW64 on 64bit machines
sudo cmd /c "del /f %WINDIR%\system32\sudo.exe"
if %arch%==64 (
	sudo64 cmd /c "del /f %WINDIR%\system32\sudo.exe"
)