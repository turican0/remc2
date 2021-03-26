
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

echo "testing level3"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level3\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 2
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level3 failed"
IF %ERRORLEVEL% EQU 20 echo "test level3 ok"

echo "testing level4"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level4\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 3
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level4 failed"
IF %ERRORLEVEL% EQU 20 echo "test level4 ok"

echo "testing level5"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level5\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 4
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level5 failed"
IF %ERRORLEVEL% EQU 20 echo "test level5 ok"

echo "testing level6"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level6\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 5
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level6 failed"
IF %ERRORLEVEL% EQU 20 echo "test level6 ok"

set /p DUMMY=Hit ENTER to continue...
