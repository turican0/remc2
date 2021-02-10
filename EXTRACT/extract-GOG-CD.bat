if not exist "CD_Files" mkdir "..\CD_Files"
..\DOSBOX\dosbox.exe -conf "dosboxExtract-GOG-CD.conf" -c exit
copy .\mdi.ini ..\Debug\NETHERW\sound\
copy .\dig.ini ..\Debug\NETHERW\sound\
