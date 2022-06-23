#include "port_time.h"

void mydelay(int milliseconds) {
#ifdef WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif // win32

};

long mygethundredths() {
	return clock()/ (CLOCKS_PER_SEC/100);
};

long mygetthousandths() {
	return clock() / (CLOCKS_PER_SEC / 1000);
};

long mygetsecond() {
	return clock();
};
