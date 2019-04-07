#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <direct.h>  
#include <io.h>  

#include <windows.h>
#include "../portability/dirent.h"
#include "../portability/port_time.h"
#include "../portability/port_filesystem.h"
#include "../portability/port_sdl_sound.h"
#include "defs.h"

#include "INIReader.h"
#include "ini.h"



#ifndef READ_CONFIG
#define READ_CONFIG

void readini(char* filename);
extern int config_skip_screen;

#endif //READ_CONFIG