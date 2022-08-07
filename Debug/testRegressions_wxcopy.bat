..@ECHO OFF

echo "testing level1"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level1\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 0
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level1 failed"
IF %ERRORLEVEL% EQU 20 echo "test level1 ok"
echo.

echo "testing level2"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level2\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 1
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level2 failed"
IF %ERRORLEVEL% EQU 20 echo "test level2 ok"
echo.

echo "testing level3"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level3\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 2
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level3 failed"
IF %ERRORLEVEL% EQU 20 echo "test level3 ok"
echo.

echo "testing level4"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level4\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 3
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level4 failed"
IF %ERRORLEVEL% EQU 20 echo "test level4 ok"
echo.

echo "testing level5"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level5\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 4
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level5 failed"
IF %ERRORLEVEL% EQU 20 echo "test level5 ok"
echo.

echo "testing level6"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level6\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 5
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level6 failed"
IF %ERRORLEVEL% EQU 20 echo "test level6 ok"
echo.

echo "testing level7"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level7\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 6
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level7 failed"
IF %ERRORLEVEL% EQU 20 echo "test level7 ok"
echo.

echo "testing level8"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level8\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 7
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level8 failed"
IF %ERRORLEVEL% EQU 20 echo "test level8 ok"
echo.

echo "testing level9"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level9\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 8
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level9 failed"
IF %ERRORLEVEL% EQU 20 echo "test level9 ok"
echo.

echo "testing level10"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level10\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 9
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level10 failed"
IF %ERRORLEVEL% EQU 20 echo "test level10 ok"
echo.

echo "testing level11"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level11\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 10
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level11 failed"
IF %ERRORLEVEL% EQU 20 echo "test level11 ok"
echo.

echo "testing level12"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level12\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 11
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level12 failed"
IF %ERRORLEVEL% EQU 20 echo "test level12 ok"
echo.

echo "testing level13"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level13\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 12
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level13 failed"
IF %ERRORLEVEL% EQU 20 echo "test level13 ok"
echo.

echo "testing level14"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level14\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 13
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level14 failed"
IF %ERRORLEVEL% EQU 20 echo "test level14 ok"
echo.

echo "testing level15"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level15\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 14
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level15 failed"
IF %ERRORLEVEL% EQU 20 echo "test level15 ok"
echo.

echo "testing level16"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level16\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 15
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level16 failed"
IF %ERRORLEVEL% EQU 20 echo "test level16 ok"
echo.

echo "testing level17"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level17\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 16
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level17 failed"
IF %ERRORLEVEL% EQU 20 echo "test level17 ok"
echo.

echo "testing level18"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level18\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 17
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level18 failed"
IF %ERRORLEVEL% EQU 20 echo "test level18 ok"
echo.

echo "testing level19"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level19\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 18
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level19 failed"
IF %ERRORLEVEL% EQU 20 echo "test level19 ok"
echo.

echo "testing level20"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level20\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 19
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level20 failed"
IF %ERRORLEVEL% EQU 20 echo "test level20 ok"
echo.

echo "testing level21"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level21\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 20
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level21 failed"
IF %ERRORLEVEL% EQU 20 echo "test level21 ok"
echo.

echo "testing level22"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level22\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 21
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level22 failed"
IF %ERRORLEVEL% EQU 20 echo "test level22 ok"
echo.

echo "testing level23"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level23\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 22
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level23 failed"
IF %ERRORLEVEL% EQU 20 echo "test level23 ok"
echo.

echo "testing level24"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level24\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 23
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level24 failed"
IF %ERRORLEVEL% EQU 20 echo "test level24 ok"
echo.

echo "testing level25"
del /q ..\remc2\memimages\regressions\*.*
copy ..\remc2\memimages\regressions\level25\*.* ..\remc2\memimages\regressions\
remc2.exe --mode_test_regressions_game reglevel 24
echo ERRORLEVEL 
echo %ERRORLEVEL%
IF %ERRORLEVEL% NEQ 20 echo "test level25 failed"
IF %ERRORLEVEL% EQU 20 echo "test level25 ok"
echo.

set /p DUMMY=Hit ENTER to continue...
