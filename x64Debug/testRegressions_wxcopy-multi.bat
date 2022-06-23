
@ECHO OFF

echo "testing multi_level1"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi1\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 50
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level1 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level1 ok"
echo.

echo "testing multi_level2"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi2\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 51
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level2 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level2 ok"
echo.

echo "testing multi_level3"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi3\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 52
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level3 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level3 ok"
echo.

echo "testing multi_level4"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi4\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 53
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level4 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level4 ok"
echo.

echo "testing multi_level5"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi5\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 54
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level5 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level5 ok"
echo.

echo "testing multi_level6"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi6\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 55
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level6 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level6 ok"
echo.

echo "testing multi_level7"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi7\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 56
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level7 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level7 ok"
echo.

echo "testing multi_level8"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi8\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 57
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level8 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level8 ok"
echo.

echo "testing multi_level9"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi9\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 58
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level9 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level9 ok"
echo.

echo "testing multi_level10"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\multi10\*.* ..\remc2\memimages\regressions\
remc2_without_xcopy.exe reglevel 59
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test multi_level10 failed"
IF %ERRORLEVEL% EQU 20 echo "test multi_level10 ok"
echo.

set /p DUMMY=Hit ENTER to continue...
