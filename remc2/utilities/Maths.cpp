#include "Maths.h"

unsigned int Maths::sub_58490_radix_3d_2(axis_3d* a1, axis_3d* a2)//239490
{
	return Maths::sub_7277A_radix_3d(
		(signed __int16)(a2->x - a1->x) * (signed __int16)(a2->x - a1->x)
		+ (signed __int16)(a2->y - a1->y) * (signed __int16)(a2->y - a1->y));
}

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

int16_t Maths::sub_58210_radix_tan(axis_3d* a1, axis_3d* a2)//239210
{
	int16_t v2; // ax
	v2 = sub_58490_radix_3d_2(a1, a2);//
	return sub_72633_maybe_tan(a1->z - a2->z, -v2);
}

uint16_t Maths::sub_72633_maybe_tan(int16_t a1, int16_t a2)//253633 //4c //3e
{
	//signed int v2; // ebx
	uint16_t result; // ax
	//int v4; // ebx
	//int v5; // eax
	//int v6; // ebx

	//debug
	//loadfromsnapshot((char*)"0160-00253633", (uint8_t*)&a1, 0x354ef8, 2);//4c
	//loadfromsnapshot((char*)"0160-00253633", (uint8_t*)&a2, 0x354efc, 2);//3e
	//loadfromsnapshot((char*)"0160-00253633", (uint8_t*)x_WORD_DE350, 0x2af350, 600);//fix it!
	//debug

	if (!a1)
	{
		if (!a2)
			return 0;
	}
	if (a1 < 0)
	{
		if (a2 < 0)
		{
			if (-a1 < -a2)
				result = 0x800 - x_WORD_DE350[(-a1 << 8) / -a2];
			else
				result = x_WORD_DE350[(-a2 << 8) / -a1] + 0x600;
		}
		else if (-a1 < a2)
		{
			result = x_WORD_DE350[(-a1 << 8) / a2] + 0x400;
		}
		else
		{
			result = 0x600 - x_WORD_DE350[(a2 << 8) / -a1];
		}
	}
	else if (a2 < 0)
	{
		if (a1 < -a2)
			result = x_WORD_DE350[(a1 << 8) / -a2];
		else
			result = 0x200 - x_WORD_DE350[(-a2 << 8) / a1];
	}
	else if (a1 < a2)
	{
		result = 0x400 - x_WORD_DE350[(a1 << 8) / a2];
	}
	else
	{
		result = x_WORD_DE350[(a2 << 8) / a1] + 0x200;
	}
	return result;
}

int16_t Maths::sub_581E0_maybe_tan2(axis_3d* a1, axis_3d* a2)//2391E0
{
	//354f10->35f418 +2->35012a .. 012a
	//354f14->354f20 +2->350168 .. 0168
	//0168-18a=3e
	//354f10->35f418->ec0141 .. 0141
	//354f14->354f20->012a018d .. 018d
	//18d-141=4c

	//ebx=a1 //141
	//edx=a2 //1d8
	//ax edx+2

	return sub_72633_maybe_tan(a2->x - a1->x, a2->y - a1->y);//253633
	//rozdil1- 4c rozdil2 - 3e
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