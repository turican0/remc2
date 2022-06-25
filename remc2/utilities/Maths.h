#pragma once

#ifndef MATHS_H
#define MATHS_H

#include "../engine/engine_support.h"

class Maths {

private:

	static uint16_t x_WORD_727B0[224];
	static uint16_t x_WORD_DE350[257];

public:

	static int32_t x_DWORD_DB750[4608]; //2ac750 //speed table1 //sin and cos table
	static char x_BYTE_D41D8[328];

	static unsigned int sub_58490_radix_3d_2(axis_3d* a1, axis_3d* a2);
	static unsigned int sub_7277A_radix_3d(unsigned int a1);
	static int16_t sub_58210_radix_tan(axis_3d* a1, axis_3d* a2);
	static uint16_t sub_72633_maybe_tan(int16_t a1, int16_t a2);
	static int16_t sub_581E0_maybe_tan2(axis_3d* a1, axis_3d* a2);
	static void x_BitScanReverse(uint32_t* Destination, uint32_t Source);
	static int SubtrackUntilZero(int x, int y);

};

#endif //MATHS_H