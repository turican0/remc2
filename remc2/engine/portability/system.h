#ifndef SYSTEM_INCLUDED
#define SYSTEM_INCLUDED

#include <iostream>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // win32

void mydelay(int timems);

#endif //SYSTEM_INCLUDED
