@ECHO OFF

echo "testing level1"
del /s /Y ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level1\*.* ..\remc2\memimages\regressions\
remc2.exe
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level1 failed"
IF %ERRORLEVEL% EQU 20 echo "test level1 ok"

set /p DUMMY=Hit ENTER to continue...
