#include "port_time.h"

void mydelay(int milliseconds) {
#ifdef WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif // win32

};

long mygethundredths() {
	return clock()*0.1;
};
