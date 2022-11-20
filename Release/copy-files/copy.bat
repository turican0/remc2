del ..\remc2.iobj
del ..\remc2.pdb
del ..\remc2.ipdb
del ..\remc2.lib
del ..\remc2.exp
copy ..\..\Debug\config.ini ..
copy ..\..\Debug\*.dll ..
xcopy ..\..\Debug\original-game ..
copy .\README-install.txt ..
copy .\config.ini ..
copy .\run-editor.bat ..
copy .\run-testlevel.bat ..
copy .\extract-GOG-CD.bat ..
copy .\dosbox-copy.conf ..
copy .\msvcp140.dll ..
copy .\msvcp140_1.dll ..
copy .\msvcp140_1d.dll ..
copy .\msvcp140_2.dll ..
copy .\msvcp140_2d.dll ..
copy .\msvcp140_codecvt_ids.dll ..
copy .\msvcp140d.dll ..
copy .\msvcp140d_codecvt_ids.dll ..
copy .\ucrtbased.dll ..
copy .\vcruntime140.dll ..
copy .\vcruntime140d.dll ..
copy .\VC_redist.x86.exe ..
copy .\vcredist_x86.exe ..
copy .\oalinst.exe ..
del SDL.dll
del SDL_mixer.dll
mkdir ..\original-game-GOG
mkdir ..\original-game
mkdir ..\original-game\language
xcopy ..\..\Debug\fix-sound ..\fix-sound  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\..\Debug\music-ogg ..\music-ogg  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\..\Debug\biggraphics ..\biggraphics  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\..\Debug\font ..\font  /E /C /I /Q /G /H /R /K /Y /Z /J
xcopy ..\..\Debug\kiss ..\kiss  /E /C /I /Q /G /H /R /K /Y /Z /J
copy .\lang\l1.txt ..\original-game\language\
copy .\lang\l2.txt ..\original-game\language\
copy .\lang\l3.txt ..\original-game\language\
copy .\lang\l4.txt ..\original-game\language\
copy .\lang\l5.txt ..\original-game\language\
copy .\lang\l6.txt ..\original-game\language\
copy .\lang\d2.txt ..\original-game\language\
copy ..\..\tools\makeczmc2\makeczmc2\l7.txt ..\original-game\language\
copy .\XXCOPY16.exe ..\original-game\
