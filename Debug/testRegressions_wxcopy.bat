
@ECHO OFF

echo "testing level1"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level1\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 0
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level1 failed"
IF %ERRORLEVEL% EQU 20 echo "test level1 ok"
echo.

echo "testing level2"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level2\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 1
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level2 failed"
IF %ERRORLEVEL% EQU 20 echo "test level2 ok"
echo.

echo "testing level3"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level3\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 2
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level3 failed"
IF %ERRORLEVEL% EQU 20 echo "test level3 ok"
echo.

echo "testing level4"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level4\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 3
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level4 failed"
IF %ERRORLEVEL% EQU 20 echo "test level4 ok"
echo.

echo "testing level5"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level5\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 4
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level5 failed"
IF %ERRORLEVEL% EQU 20 echo "test level5 ok"
echo.

echo "testing level6"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level6\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 5
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level6 failed"
IF %ERRORLEVEL% EQU 20 echo "test level6 ok"
echo.

echo "testing level7"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level7\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 6
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level7 failed"
IF %ERRORLEVEL% EQU 20 echo "test level7 ok"
echo.

echo "testing level8"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level8\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 7
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level8 failed"
IF %ERRORLEVEL% EQU 20 echo "test level8 ok"
echo.

echo "testing level9"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level9\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 8
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level9 failed"
IF %ERRORLEVEL% EQU 20 echo "test level9 ok"
echo.

echo "testing level10"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level10\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 9
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level10 failed"
IF %ERRORLEVEL% EQU 20 echo "test level10 ok"
echo.

echo "testing level11"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level11\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 10
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level11 failed"
IF %ERRORLEVEL% EQU 20 echo "test level11 ok"
echo.

echo "testing level12"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level12\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 11
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level12 failed"
IF %ERRORLEVEL% EQU 20 echo "test level12 ok"
echo.

set /p DUMMY=Hit ENTER to continue...
