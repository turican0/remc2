copy ..\..\Debug\remc2.exe ..
copy ..\..\Debug\config.ini ..
copy ..\..\Debug\*.dll ..
xcopy ..\..\Debug\original-game ..
copy .\README-install.txt ..
copy .\config.ini ..
copy .\run-editor.bat ..
copy .\run-testlevel.bat ..
del SDL.dll
del SDL_mixer.dll
mkdir ..\original-game
mkdir ..\original-game\language
xcopy ..\..\Debug\fix-sound ..\fix-sound  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\..\Debug\music-ogg ..\music-ogg  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\..\Debug\biggraphics ..\biggraphics  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\..\Debug\kiss ..\kiss  /E /C /I /Q /G /H /R /K /Y /Z /J
copy ..\..\tools\makeczmc2\makeczmc2\l7.txt ..\original-game\language\
