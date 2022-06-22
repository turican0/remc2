cd /d "C:\prenos\remc2\remc2" &msbuild "remc2.vcxproj" /t:sdvViewer /p:configuration="Debug" /p:platform=Win32
exit %errorlevel% 