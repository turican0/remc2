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

	printf("--- Regressions tests ---\n\n");

	run_regtest(1);
	run_regtest(2);
	run_regtest(3);
	run_regtest(4);
	run_regtest(5);
	run_regtest(6);
	run_regtest(7);
	run_regtest(8);
	run_regtest(9);
	run_regtest(10);
	run_regtest(11);
	run_regtest(12);
	run_regtest(13);
	run_regtest(14);
	run_regtest(15);
	run_regtest(16);
	run_regtest(17);
	run_regtest(18);
	run_regtest(19);
	run_regtest(20);
	run_regtest(21);
	//run_regtest(22);
	run_regtest(23);
	run_regtest(24);
	run_regtest(25);

	printf("--- Terrain tests ---\n\n");
	Terrain_test();

	printf("\nAll tests passed\n");

	std::this_thread::sleep_for(std::chrono::milliseconds(20000));
	return(0);
}