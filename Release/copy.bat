copy ..\Debug\remc2.exe .
copy ..\Debug\config.ini .
copy ..\Debug\*.dll .
xcopy ..\Debug\original-game .
del SDL.dll
del SDL_mixer.dll
mkdir original-game
xcopy ..\Debug\netherw .\netherw  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\Debug\fix-sound .\fix-sound  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\Debug\music-ogg .\music-ogg  /E /C /I /Q /G /H /R /K /Y /Z /J
