#include "Basic-tests.h"
unsigned int pseudoRand()
{
	static unsigned int nSeed = 5323;
	nSeed = (8253729 * nSeed + 2396403);
	return nSeed % 32767;
}

void TestError() {
	printf("!!! Error !!!");
	allert_error();
}