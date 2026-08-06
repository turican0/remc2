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

#ifndef WIN32
extern timeval timeStart;
#endif

void mydelay(int timems);
unsigned long mygethundredths();
unsigned long mygetthousandths();
unsigned long mygetsecond();

double clockToMilliseconds(clock_t ticks);
#ifndef _MSC_VER
timeval GetTimeStart();
#endif
void SetTimeStart();

#endif //PORT_TIME
