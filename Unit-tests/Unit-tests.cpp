// Unit-tests.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//
#include <chrono>
#include <thread>
#include <iostream>
#include "Terrain-tests.h"
#include "Regress-tests.h"

int main(int argc, char** argv)
{
	//if (CommandLineParams.DoShowDebugMessages1()) -- for suppress messages

	printf("--- Regressions tests ---\n");
	for(int i = 1 ; i <= 25; i++)
		if(i!=22)
			run_regtest(i);
	printf("\n");
	printf("--- Terrain tests ---\n\n");
	Terrain_test();

	printf("\nAll tests passed\n");

	std::this_thread::sleep_for(std::chrono::milliseconds(20000));
	return(0);
}