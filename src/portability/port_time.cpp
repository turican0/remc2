#include "port_time.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#ifndef WIN32
timeval timeStart = { 0, 0 };
#endif

void mydelay(int milliseconds) {
#ifdef WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif // win32

};

long startTime = 0;

unsigned long relativeClock() {
	return (clock() - startTime);
}

unsigned long mygethundredths() {
#ifdef WIN32
	return relativeClock() / (CLOCKS_PER_SEC/100);
#else
	struct timeval timecheck;
	gettimeofday(&timecheck, nullptr);
    return ((timecheck.tv_sec - GetTimeStart().tv_sec) * 1000 + (timecheck.tv_usec - GetTimeStart().tv_usec) / 1000) / 10;
#endif
};

unsigned long mygetthousandths() {
#ifdef WIN32
	return relativeClock() / (CLOCKS_PER_SEC/1000);
#else
	struct timeval timecheck;
	gettimeofday(&timecheck, nullptr);
    return ((timecheck.tv_sec - GetTimeStart().tv_sec) * 1000 + (timecheck.tv_usec - GetTimeStart().tv_usec) / 1000);
#endif
};

unsigned long mygetsecond() {
#ifdef WIN32
	return relativeClock();
#else
	struct timeval timecheck;
	gettimeofday(&timecheck, nullptr);
    return ((timecheck.tv_sec - GetTimeStart().tv_sec) * 1000 + (timecheck.tv_usec - GetTimeStart().tv_usec) / 1000) / 1000;
#endif
};

double clockToMilliseconds(clock_t ticks) {
	// units/(units/time) => time (seconds) * 1000 = milliseconds
	return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

#ifndef _MSC_VER
timeval GetTimeStart()
{
	if (timeStart.tv_sec == 0 && timeStart.tv_usec == 0)
		SetTimeStart();
	return timeStart;
}
#endif

void SetTimeStart()
{
#ifndef _MSC_VER
	gettimeofday(&timeStart, nullptr);
#else
	startTime = clock();
#endif
}
