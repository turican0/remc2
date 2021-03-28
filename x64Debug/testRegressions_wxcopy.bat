
@ECHO OFF

echo "testing level1"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level1\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 0
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level1 failed"
IF %ERRORLEVEL% EQU 20 echo "test level1 ok"

echo "testing level2"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level2\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 1
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level2 failed"
IF %ERRORLEVEL% EQU 20 echo "test level2 ok"

set /p DUMMY=Hit ENTER to continue...
