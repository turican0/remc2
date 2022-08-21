@echo off

:: ---------------------------------------------------------------------------
:: Displaying script information

echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ Chortkeh Fraunhofer IIS MPEG Layer-3 Codec Fix for Windows 7 / Vista       ³
echo ³ Revision 4                                                                 ³
echo ³ July 4, 2013                                                               ³
echo ³ Command-line scripting by Komeil Bahmanpour                                ³
echo ³ http://www.komeil.com/blog/enable-fraunhofer-mp3-l3codecp-acm-windows      ³
echo ³ Copyright (c) 2007-2013 Chortkeh. All rights reserved.                     ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
echo.

:: ---------------------------------------------------------------------------
:: Checking to see if the batch file has been run-as-administrator.
reg add HKLM /f > nul 2>&1
if errorlevel 1 (
  echo Registry manipulation error occurred.
  echo.
  echo This script requires administrative privileges to work properly.
  echo.
  echo This file must be run as administrator. To do so, right-click on the file
  echo and select run as administrator.
  echo.
  pause
  goto:eof
)

:: ---------------------------------------------------------------------------
:: Determining processor_architecture: x86 / AMD64 / IA64 / EM64T

if /i %PROCESSOR_ARCHITECTURE%==x86 goto x86

:: ---------------------------------------------------------------------------
:: Still here, it's an x64. 64-bit systems require both native (x64) and WoW64 to be fixed.

echo Removing incompatible WoW64 values for both professional and advanced codecs...
reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows NT\CurrentVersion\drivers.desc" /v "l3codecp.acm" /f 2> nul
reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows NT\CurrentVersion\drivers.desc" /v "l3codeca.acm" /f 2> nul
reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows NT\CurrentVersion\drivers.desc" /v "%SYSTEMROOT%\SysWOW64\l3codeca.acm" /f 2> nul
echo.
echo Adding WoW64 values for Fraunhofer IIS MPEG Layer-3 professional codec...
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows NT\CurrentVersion\drivers.desc" /v "%SYSTEMROOT%\SysWOW64\l3codecp.acm" /t REG_SZ /d "Fraunhofer IIS MPEG Audio Layer-3 Codec (professional)" /f
if errorlevel 1 goto Error
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows NT\CurrentVersion\Drivers32" /v "msacm.l3acm" /t REG_SZ /d "%SYSTEMROOT%\SysWOW64\l3codecp.acm" /f
if errorlevel 1 goto Error
echo.

:x86

echo Removing incompatible values for both professional and advanced codecs...
reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\drivers.desc" /v "l3codecp.acm" /f 2> nul
reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\drivers.desc" /v "l3codeca.acm" /f 2> nul
reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\drivers.desc" /v "%SYSTEMROOT%\System32\l3codeca.acm" /f 2> nul
echo.
echo Adding values for Fraunhofer IIS MPEG Layer-3 professional codec...
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\drivers.desc" /v "%SYSTEMROOT%\System32\l3codecp.acm" /t REG_SZ /d "Fraunhofer IIS MPEG Audio Layer-3 Codec (professional)" /f
if errorlevel 1 goto Error
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Drivers32" /v "msacm.l3acm" /t REG_SZ /d "%SYSTEMROOT%\System32\l3codecp.acm" /f
if errorlevel 1 goto Error
echo.

pause
goto:eof

:: ---------------------------------------------------------------------------

:Error
echo.
echo Operation failed.
echo.
pause
goto:eof

:: ---------------------------------------------------------------------------
