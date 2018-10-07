REM   This batch file called by the debug.bat and release.bat files in 
REM     the source code directories under this directory.  Once you enter 
REM     the pathnames to your tools, You can simply run debug.bat and 
REM     release.bat to build Miles quickly and easily.





set MSVC8Dir=
set MASMPath=
set DX9Path=





@if "%MSVC8INPATH%"=="1" goto no_path
@if "%MSVC8Dir%"=="" goto no_path

@set MSVC8INPATH=1

set include=%MSVC8Dir%\vc\include;%MSVC8Dir%\vc\platformsdk\include
set lib=%MSVC8Dir%\vc\lib;%MSVC8Dir%\vc\platformsdk\lib
set path=%MSVC8Dir%\vc\bin;%MSVC8Dir%\common7\ide;%path%

:no_path

