
@ECHO OFF

echo "testing shadow_level1"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\shadow1\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 30
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test shadow_level1 failed"
IF %ERRORLEVEL% EQU 20 echo "test shadow_level1 ok"
echo.

echo "testing shadow_level2"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\shadow2\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 31
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test shadow_level2 failed"
IF %ERRORLEVEL% EQU 20 echo "test shadow_level2 ok"
echo.

echo "testing shadow_level3"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\shadow3\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 32
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test shadow_level3 failed"
IF %ERRORLEVEL% EQU 20 echo "test shadow_level3 ok"
echo.

echo "testing shadow_level4"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\shadow4\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 33
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test shadow_level4 failed"
IF %ERRORLEVEL% EQU 20 echo "test shadow_level4 ok"
echo.

echo "testing shadow_level5"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\shadow5\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 34
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test shadow_level5 failed"
IF %ERRORLEVEL% EQU 20 echo "test shadow_level5 ok"
echo.


set /p DUMMY=Hit ENTER to continue...
