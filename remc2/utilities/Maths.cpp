#include "Maths.h"

unsigned int Maths::sub_7277A_radix_3d(unsigned int a1)//25377a
{
	uint32_t v1; // eax
	unsigned int i; // ebx

	if (!a1)
		return 0;
	x_BitScanReverse(&v1, a1);//FIX IT!
	for (i = (unsigned __int16)x_WORD_727B0[v1]; (signed int)(a1 / i) < (signed int)i; i = (a1 / i + i) >> 1)
		;
	return i;
}

void Maths::x_BitScanReverse(uint32_t* Destination, uint32_t Source) 
{
	*Destination = 0;
	if (Source & 0x80000000)
	{
		*Destination = 0x1f;
		return;
	}
	if (Source & 0x40000000)
	{
		*Destination = 0x1e;
		return;
	}
	if (Source & 0x20000000)
	{
		*Destination = 0x1d;
		return;
	}
	if (Source & 0x10000000)
	{
		*Destination = 0x1c;
		return;
	}
	if (Source & 0x8000000)
	{
		*Destination = 0x1b;
		return;
	}
	if (Source & 0x4000000)
	{
		*Destination = 0x1a;
		return;
	}
	if (Source & 0x2000000)
	{
		*Destination = 0x19;
		return;
	}
	if (Source & 0x1000000)
	{
		*Destination = 0x18;
		return;
	}
	if (Source & 0x800000)
	{
		*Destination = 0x17;
		return;
	}
	if (Source & 0x400000)
	{
		*Destination = 0x16;
		return;
	}
	if (Source & 0x200000)
	{
		*Destination = 0x15;
		return;
	}
	if (Source & 0x100000)
	{
		*Destination = 0x14;
		return;
	}
	if (Source & 0x80000)
	{
		*Destination = 0x13;
		return;
	}
	if (Source & 0x40000)
	{
		*Destination = 0x12;
		return;
	}
	if (Source & 0x20000)
	{
		*Destination = 0x11;
		return;
	}
	if (Source & 0x10000)
	{
		*Destination = 0x10;
		return;
	}
	if (Source & 0x8000)
	{
		*Destination = 0xf;
		return;
	}
	if (Source & 0x4000)
	{
		*Destination = 0xe;
		return;
	}
	if (Source & 0x2000)
	{
		*Destination = 0xd;
		return;
	}
	if (Source & 0x1000)
	{
		*Destination = 0xc;
		return;
	}
	if (Source & 0x800)
	{
		*Destination = 0xb;
		return;
	}
	if (Source & 0x400)
	{
		*Destination = 0xa;
		return;
	}
	if (Source & 0x200)
	{
		*Destination = 0x9;
		return;
	}
	if (Source & 0x100)
	{
		*Destination = 0x8;
		return;
	}
	if (Source & 0x80)
	{
		*Destination = 0x7;
		return;
	}
	if (Source & 0x40)
	{
		*Destination = 0x6;
		return;
	}
	if (Source & 0x20)
	{
		*Destination = 0x5;
		return;
	}
	if (Source & 0x10)
	{
		*Destination = 0x4;
		return;
	}
	if (Source & 0x8)
	{
		*Destination = 0x3;
		return;
	}
	if (Source & 0x4)
	{
		*Destination = 0x2;
		return;
	}
	if (Source & 0x2)
	{
		*Destination = 0x1;
		return;
	}
	if (Source & 0x1)
	{
		*Destination = 0x0;
		return;
	}
	Destination = NULL;
	return;
};