#ifndef PORT_TIME
#define PORT_TIME

#include <time.h>  

#include <iostream>
#ifdef WIN32
#include <windows.h>
#elif _WIN64
#include <windows.h>
#else
#include <unistd.h>
#endif // win32

void mydelay(int timems);
unsigned long mygethundredths();
unsigned long mygetthousandths();
unsigned long mygetsecond();
double clockToMilliseconds(clock_t ticks);

#endif //PORT_TIME
