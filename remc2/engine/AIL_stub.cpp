
#include "AIL_stub.h"

unsigned     int x__readeflags(void) { stub_fix_it(); return 0; };

x_WORD __CS__;
x_WORD __GS__;
x_WORD __DS__;
x_DWORD _EAX;
x_DWORD _EBX;
x_DWORD _ECX;
x_DWORD _EDX;
x_WORD __FS__;
x_WORD __ES__;
x_WORD __SS__;
x_WORD _CF;
x_WORD _ZF;
x_WORD _SF;
x_DWORD _EBP;



void x__writeeflags(unsigned Value) { stub_fix_it(); };
unsigned int x__getcallerseflags(void) { stub_fix_it(); return 0; };

int nullsub_3(x_DWORD number) { stub_fix_it(); return 0; };
int _no_support_loaded(x_DWORD number1, x_DWORD number2) { stub_fix_it(); return 0; };

//----- (000A0EEC) --------------------------------------------------------
void sub_A0EEC_s_plus2()
{
	x_DWORD_E3FF2++;
}
// E3FF2: using guessed type int x_DWORD_E3FF2;

//----- (000A0EF9) --------------------------------------------------------
void sub_A0EF9_s_minus2()//281ef9
{
	x_DWORD_E3FF2--;
}
// E3FF2: using guessed type int x_DWORD_E3FF2;

//----- (0009D450) --------------------------------------------------------
Bit8u* sub_9D450_malloc4(int a1)
{
	Bit8u* v2; // [esp+4h] [ebp-4h]

	v2 = (Bit8u*)malloc(a1);//off_E3E30_mallocx(a1);
	if (v2)
		sub_9D710(v2, a1);
	return v2;
}
// E3E30: invalid function type has been ignored
// E3E30: using guessed type int (*off_E3E30_mallocx)(x_DWORD);

int x_free(void* ptr) { free(ptr); return 0; };

// E29E8: using guessed type char x_BYTE_E29E8;
// 17ECA0: using guessed type int x_DWORD_17ECA0;
// 17FEA0: using guessed type int x_DWORD_17FEA0;
void sub_83E80_freemem4(Bit8u* a1) {
	free(a1);
}

//----- (0009D490) --------------------------------------------------------
int sub_9D490_free4(void* a1, int a2)
{
	int result; // eax

	if (a1)
	{
		//sub_9D740(a1, a2);
		//result = off_E3E34_freex(a1);
		result = x_free(a1);
	}
	return result;
	//fix it
	//return 0;
}
// E3E34: using guessed type int (*off_E3E34_freex)(int);

//----- (0009D560) --------------------------------------------------------
void sub_9D560(Bit8u* a1)
{
	sub_83E80_freemem4(a1);
}

void dbgfprintf(FILE* file, const char *format, ...) {
	//void dbgfprintf(FILE* file,char* str) {
	fprintf(file, format);
	fprintf(file, "\n");
}

char x_toupper(char inputchar) {
	return toupper(inputchar);
}; //weak

char* mygetenv(const char *a1)//27ae33
{
	//_dupenv_s(&v7, &sz, "MDMUSIC");
	char** v1; // ebx
	unsigned int v2; // kr04_4
	int v3; // edi
	char* v5; // esi

	v1 = (char**)dword_E4CC4;//2b5cc4
	if (dword_E4CC4 && a1)
	{
		v2 = strlen(a1) + 1;
		v3 = v2 - 1;
		while (1)
		{
			v5 = v1[0];
			if (!v1[0])
				break;
			if (!strnicmp(v5, a1, v3) && v5[v3] == 61)
				return &v5[v2];
			v1++;
		}
	}
	return 0;
}

void qmemcpy(void* a, void* b, size_t c) {
	memcpy(a, b, c);
};

unsigned int sub_91F20();
Bit32s sub_91F70_AIL_call_driver(AIL_DRIVER* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out);//AIL_call_driver
void sub_91F50(unsigned int a1);
void sub_9E250(HMDIDRIVER mdidrv) {
	if (!x_DWORD_E3E38)
	{
		x_DWORD_E3E38 = 1;
		x_DWORD_181DA8 = sub_91F20();
		sub_91F70_AIL_call_driver(mdidrv->drvr_0, 770, 0, 0);
		sub_91F50(x_DWORD_181DA8);
		x_DWORD_E3E38 = 0;
	}
}; // weak
void sub_A2450(HMDIDRIVER user) { stub_fix_it(); }; // weak
int sub_A2DE0() { stub_fix_it(); return 0; }; // weak
int sub_A47A0() { stub_fix_it(); return 0; }; // weak
int sub_A4920() { stub_fix_it(); return 0; }; // weak
//void sub_A6530(Bit32u user) { stub_fix_it();}; // weak
int sub_A9C00() { stub_fix_it(); return 0; }; // weak
int sub_A9C50() { stub_fix_it(); return 0; }; // weak
void sub_A6F30(void* a/*HDIGDRIVER a*/) { stub_fix_it();/*return 0; */ }; // weak
int /*__far*/ sub_C0000() { stub_fix_it(); return 0; }; // weak


//----- (0009E2B0) --------------------------------------------------------
int sub_9E2B0(char* a1, int a2, x_DWORD *a3)
{
	int v3; // ebx
	int v5; // [esp+4h] [ebp-10h]
	int v6; // [esp+8h] [ebp-Ch]
	int j; // [esp+Ch] [ebp-8h]
	unsigned int i; // [esp+10h] [ebp-4h]

	v6 = 0;
	v5 = 1;
	for (i = 0; strlen(a1) > i; i++)
	{
		if (a1[i] == 45)
		{
			v5 = -v5;
		}
		else if (!(IsTable[a1[i] + 1] & 2))
		{
			for (j = 0; j < a2; j++)
			{
				v3 = x_BYTE_D3450[j];
				if (x_toupper(a1[i]) == v3)
				{
					v6 = a2 * v6 + j;
					break;
				}
			}
			if (j == a2)
				break;
		}
	}
	if (a3)
		*a3 = i + (int)a1;
	return v5 * v6;
}
// 98805: using guessed type x_DWORD x_toupper(x_DWORD);
// 99D6B: using guessed type x_DWORD strlen(x_DWORD);


//----- (0009E3A0) --------------------------------------------------------
int sub_9E3A0_AIL_API_read_INI(AIL_INI* INI, char* filename/*,char* a8*/)//27f3a0
{
	unsigned int v2; // eax
	unsigned int v3; // eax
	unsigned int v4; // eax
	unsigned int v5; // eax
	//char v7[300]; // [esp+0h] [ebp-180h]
	//char v8[200]; // [esp+80h] [ebp-100h]
	//Bit16s v9[12]; // [esp+100h] [ebp-80h]
	FILE* v9x;
	//errno_t v9y;
	//__int16 v10; // [esp+102h] [ebp-7Eh]
	//__int16 v11; // [esp+104h] [ebp-7Ch]
	//__int16 v12; // [esp+106h] [ebp-7Ah]
	char v13[80]; // [esp+118h] [ebp-68h]
	char *v14; // [esp+168h] [ebp-18h]
	char *v15; // [esp+16Ch] [ebp-14h]
	char *v16; // [esp+170h] [ebp-10h]
	unsigned int i; // [esp+174h] [ebp-Ch]
	int v18; // [esp+178h] [ebp-8h]
	char v19[80]; // [esp+17Ch] [ebp-4h]

	memset(INI->device_name, 0, 280);
	memset(&INI->IO, -1, 24);
	//v9y = myfopen_s(&v9x,a2,(const char*) "rt");
	v9x = myopent(filename, (char*)"rt");
	if (!v9x)
		return 0;
	while (fgets(v13, 80, v9x) /*&& !(v9x[12] & 0x10)*/)//355140
	{
		for (i = strlen(v13) - 1; (i & 0x80000000) == 0 && IsTable[(v13[i] + 1)] & 2; i--)
			v13[i] = 0;
		for (i = 0; ; i++)
		{
			v2 = strlen(v13);
			if (v2 <= i || !(IsTable[(v13[i] + 1)] & 2))
				break;
		}
		v15 = &v13[i];
		while (1)
		{
			v3 = strlen(v13);
			if (v3 <= i || IsTable[(v13[i] + 1)] & 2)
				break;
			i++;
		}
		v14 = &v13[i];
		while (1)
		{
			v4 = strlen(v13);
			if (v4 <= i || !(IsTable[(v13[i] + 1)] & 2))
				break;
			i++;
		}
		v16 = &v13[i];
		v5 = strlen(v13);
		if (v5 > i)
		{
			*v14 = 0;
			if (*v15 != 59)
			{
				if (!_strnicmp(v15, "DRIVER", 7))
				{
					strcpy(INI->driver_name, strlen(v16) + 1, v16);
				}
				else if (!_strnicmp(v15, "DEVICE", 7))
				{
					strcpy(INI->device_name, strlen(v16) + 1, v16);
				}
				else if (!_strnicmp(v15, "IO_ADDR", 8))
				{
					INI->IO.IO = sub_9E2B0(v16, 16, 0);
				}
				else if (!_strnicmp(v15, "IRQ", 4))
				{
					INI->IO.IRQ = sub_9E2B0(v16, 10, 0);
				}
				else if (!_strnicmp(v15, "DMA_8_bit", 10))
				{
					INI->IO.DMA_8_bit = sub_9E2B0(v16, 10, 0);
				}
				else if (!_strnicmp(v15, "DMA_16_bit", 11))
				{
					INI->IO.DMA_16_bit = sub_9E2B0(v16, 10, 0);
				}
			}
		}
	}
	fclose(v9x);
	if (strlen(INI->device_name) && strlen(INI->device_name))
	{
		//qmemcpy(filename, INI->device_name, 0x118u);//fix it
		v18 = 1;
	}
	else
	{
		//qmemcpy(&x_BYTE_181C90, (void*)"Corrupted .INI file\n", 0x15u);
		strcpy(x_BYTE_181C90, "Corrupted .INI file\n");
		v18 = 0;
	}
	return v18;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 99B23: using guessed type x_DWORD strnicmp(x_DWORD, x_DWORD, x_DWORD);
// 99D46: using guessed type x_DWORD strcpy(x_DWORD, x_DWORD);
// 99D6B: using guessed type x_DWORD strlen(x_DWORD);
// A1AF0: using guessed type x_DWORD fopen(x_DWORD, x_DWORD);
// A1E05: using guessed type x_DWORD fclose(x_DWORD);
// AD7AA: using guessed type x_DWORD fgets(x_DWORD, x_DWORD, x_DWORD);
// 181C90: using guessed type char x_BYTE_181C90;
// 9E3A0: using guessed type char var_68[80];

//----- (0009E6E0) --------------------------------------------------------
int sub_9E6E0_set_preference(Bit32u number, Bit32s value)
{
	int oldvalue;
	oldvalue = x_DWORD_181DAC[number];
	x_DWORD_181DAC[number] = value;
	return oldvalue;
}
// 181DAC: using guessed type int x_DWORD_181DAC[];

//----- (00083D70) --------------------------------------------------------
void* sub_83D70_malloc1(int a1)//264d70
{
	/*int *v1; // eax
	unsigned int v2; // edi
	int *v3; // ebx
	unsigned int v4; // esi
	unsigned int v5; // edx

	sub_85070();
	v1 = (int*)&x_DWORD_17ECA0;
	v2 = -1;
	v4 = a1 + 271;
	v3 = 0;
	LOWORD(v4) = (a1 + 271) & 0xFFF0;
	while ( v1 )
	{
	  v5 = v1[1];
	  if ( v4 <= v5 && v2 > v5 && !*((x_BYTE *)v1 + 16) && x_DWORD_17E0A4[3 * *((unsigned __int8 *)v1 + 17)] )
	  {
		v3 = v1;
		v2 = v1[1];
	  }
	  v1 = (int *)v1[2];
	}
	if ( !v3 || !sub_83E00((int)v3, v4) )
	  return 0;
	sub_85350();
	memset((void*)*v3, 0, v4);//probl�m
	return (void*)*v3;*/
	return malloc(a1);
}

//----- (0009D4D0) --------------------------------------------------------
signed int sub_9D4D0(int a1, Bit8u** a2, Bit32u* a3, Bit32u* a4)//27e4d0
{
	Bit8u* v6; // [esp+Ch] [ebp-4h]

	v6 = (Bit8u*)sub_83D70_malloc1(16 * a1);
	if (v6 == NULL)
		return 0;
	*a3 = 0;//(int)v6 >> 4 << 16;//fixed - remove segment
	*a2 = v6;
	*a4 = 1232;
	//sub_9D590_lock_linear_region((Bit8u*)(*a3 >> 12), (*a3 >> 12) + 16 * a1 - 1);
	return 1;
}

//----- (0009E720) --------------------------------------------------------
AIL_DRIVER* sub_9E720_AIL_API_install_driver(/*int a1, */Bit8u* driver_image, Bit32s n_bytes)//27f720
{
	//VDI_HDR* v3; // edx
	int v4; // ecx
	int v5; // ST0C_4
	int v6; // ST08_4
	int v7; // ST0C_4
	int v8; // ST08_4
	int v9; // ST0C_4
	int v10; // ST08_4
	AIL_DRIVER* result; // [esp+0h] [ebp-Ch]
	unsigned int i; // [esp+4h] [ebp-8h]
	AIL_DRIVER* v14; // [esp+8h] [ebp-4h]

	//fix it
	//v3 = new VDI_HDR();
	v4 = 0;
	//fix it

	v14 = (AIL_DRIVER*)sub_9D450_malloc4(sizeof(AIL_DRIVER));
	if (v14)
	{
		v14->size_3 = n_bytes;
		if (sub_9D4D0((n_bytes + 15) >> 4, (Bit8u**)&v14->buf_2, &v14->seg_0, &v14->sel_1))
		{
			memmove((void*)v14->buf_2, (const void*)driver_image, n_bytes);
			//fix it - make buf reader
			v14->VHDR_4 = (VDI_HDR*)v14->buf_2;
			if (!_strnicmp((const char*)v14->VHDR_4->ID_8, "AIL3DIG", 7))
			{
				v14->type_5 = 0;
			}
			else
			{
				if (_strnicmp((char*)v14->VHDR_4->ID_8, "AIL3MDI", 7))
				{
					//qmemcpy(&x_BYTE_181C90, (void*)"Invalid driver type\n", 0x15u);
					strcpy(x_BYTE_181C90, "Invalid driver type\n");
					v5 = v14->sel_1;
					v6 = v14->seg_0;
					sub_9D560((Bit8u*)v14->buf_2);
					sub_9D490_free4(v14, 44);
					return 0;
				}
				v14->type_5 = 1;
			}
			for (i = 0; i < 0x10; i++)
			{
				if (!x_DWORD_181C50[i])
				{
					x_DWORD_181C50[i] = v14;
					v14->VHDR_4->VDI_HDR_var50 = i;
					break;
				}
			}
			if (i != 16)
			{
				v14->VHDR_4->VDI_HDR_var48 = 0;
				v14->initialized_6 = 0;
				v14->PM_ISR_7 = -1;
				v14->VHDR_4->VDI_HDR_var54_prev_ISR = sub_91C10_AIL_get_real_vect(/*v3, v4, a1,*/ 102);//save old vector
				sub_91D50_AIL_set_real_vect(102, /*v14->AIL_DRIVER_var0_seg + */v14->VHDR_4->VDI_HDR_var52_this_ISR);//set new vector
				v14->destructor_9 = 0;
				v14->descriptor_10 = 0;
				sub_91F70_AIL_call_driver(v14, 768, 0, 0);
				if (v14->VHDR_4->VDI_HDR_var46 > 0)
				{
					v14->server_8 = sub_92600_AIL_register_timer(sub_9E250);
					if (v14->server_8 == -1)
					{
						//qmemcpy(&x_BYTE_181C90, (void*)"Out of timer handles\n", 0x16u);
						strcpy(x_BYTE_181C90, "Out of timer handles\n");
						sub_91D50_AIL_set_real_vect(102, v14->VHDR_4->VDI_HDR_var54_prev_ISR);
						v9 = v14->sel_1;
						v10 = (int)v14->seg_0;
						sub_9D560((Bit8u*)v14->buf_2);
						sub_9D490_free4(v14, 44);
						return 0;
					}
					sub_92740_AIL_set_timer_user(v14->server_8, (signed __int32)v14);
					sub_92930_AIL_set_timer_frequency(v14->server_8, v14->VHDR_4->VDI_HDR_var46);
					sub_92BA0_AIL_start_timer(v14->server_8);
				}
				else
				{
					v14->server_8 = -1;
				}
				return v14;
			}
			//qmemcpy(x_BYTE_181C90, (void*)"Out of driver handles\n", 0x17u);
			strcpy(x_BYTE_181C90, "Out of driver handles\n");
			v7 = v14->sel_1;
			v8 = (int)v14->seg_0;
			sub_9D560((Bit8u*)v14->buf_2);
			sub_9D490_free4(v14, 44);
			result = 0;
		}
		else
		{
			//qmemcpy(x_BYTE_181C90, (void*)"Insufficient low memory\n", 0x19u);
			strcpy(x_BYTE_181C90, "Insufficient low memory\n");
			sub_9D490_free4(v14, 44);
			result = 0;
		}
	}
	else
	{
		strcpy(x_BYTE_181C90, strlen("Insufficient memory for driver descriptor\n") + 1, "Insufficient memory for driver descriptor\n");
		result = 0;
	}
	return result;
}
// 99B23: using guessed type x_DWORD strnicmp(x_DWORD, x_DWORD, x_DWORD);
// 9E250: using guessed type int sub_9E250();
// AD81F: using guessed type x_DWORD memmove(x_DWORD, x_DWORD, x_DWORD);
// 181C90: using guessed type char x_BYTE_181C90;

//----- (0009EA60) --------------------------------------------------------
int sub_9EA60_AIL_API_uninstall_driver(AIL_DRIVER* a1)
{
	int v1; // ST0C_4
	int v2; // ST08_4
	int result; // eax
	signed int i; // [esp+0h] [ebp-4h]

	if (a1->destructor_9)
		//(*(void (**)(x_DWORD))(a1 + 36))(*(x_DWORD *)(a1 + 40));
	{
		(a1->destructor_9)((HDIGDRIVER)a1->descriptor_10);
	}
	if (a1->server_8 != -1)
		sub_92DC0_AIL_release_timer_handle(a1->server_8);
	if (a1->initialized_6)
	{
		if (a1->PM_ISR_7 != -1)
			sub_91E90_AIL_restore_USE16_ISR(a1->PM_ISR_7);
		sub_91F70_AIL_call_driver(a1, 774, 0, 0);
	}
	sub_91D50_AIL_set_real_vect(102, 0/*(Bit8u*)*(x_DWORD *)(*(x_DWORD *)(a1 + 16) + 54)*/);//fix
	v1 = a1->sel_1;
	v2 = a1->seg_0;
	sub_9D560((Bit8u*)a1->buf_2);
	result = sub_9D490_free4(a1, 44);
	for (i = 0; i < 16; i++)
	{
		if (x_DWORD_181C50[i] == a1)
			x_DWORD_181C50[i] = 0;
		result = i;
	}
	return result;
}

//----- (0009EB60) --------------------------------------------------------
IO_PARMS* sub_9EB60_AIL_API_get_IO_environment(AIL_DRIVER *drvr)
{
	//ac_sound_get_io_environment
	char* v3; // [esp+4h] [ebp-8h]
	char* v4; // [esp+8h] [ebp-4h]

	//fix it
	v3 = 0;
	//fix it

	v4 = (char*)(*(x_DWORD *)(*(x_DWORD *)(drvr + 16) + 18) & 0xFFFF) + 16 * (*(x_DWORD *)(*(x_DWORD *)(drvr + 16) + 18) >> 16);
	if (!v4)
		return 0;
	//_dupenv_s((char**)v3,(size_t*)sizeof(v3),(char*)v4);
	v3 = mygetenv(v4);
	if (!v3)
		return 0;
	strncpy((char*)*(x_DWORD *)(drvr + 16) + 58, sizeof((drvr + 16) + 58), (const char*)v3, 128);
	if (sub_91F70_AIL_call_driver(drvr, 771, 0, 0) == -1)
		return 0;
	qmemcpy(&unk_181DF8, (void *)(*(x_DWORD *)(drvr + 16) + 22), 0x18u);
	return &unk_181DF8;
}
// 99E33: using guessed type x_DWORD getenv(x_DWORD);
// AD871: using guessed type x_DWORD strncpy(x_DWORD, x_DWORD, x_DWORD);

//----- (0009EDD0) --------------------------------------------------------
bool sub_9EDD0()
{
	bool result; // eax

	if (x_DWORD_E3E3C)
	{
		sub_9D650((unsigned int)sub_9E1B0, (unsigned int)sub_9EDD0);
		sub_9D740((unsigned int)x_DWORD_181C50, 64);
		sub_9D740((unsigned int)x_DWORD_181DAC, 76);
		sub_9D740((unsigned int)&x_BYTE_181C90, 256);
		sub_9D740((unsigned int)&unk_181D90, 24);
		sub_9D740((unsigned int)&x_DWORD_E3E38, 4);
		result = sub_9D740((unsigned int)&x_DWORD_181DA8, 4);
		x_DWORD_E3E3C = 0;
	}
	return result;
}
// E3E38: using guessed type int x_DWORD_E3E38;
// E3E3C: using guessed type int x_DWORD_E3E3C;
// 181C90: using guessed type char x_BYTE_181C90;
// 181DA8: using guessed type int x_DWORD_181DA8;
// 181DAC: using guessed type int x_DWORD_181DAC[];

//----- (0009E1B0) --------------------------------------------------------
void sub_9E1B0()
{
	//bool result; // eax

	if (!x_DWORD_E3E3C)
	{
		sub_9D590_lock_linear_region((Bit8u*)sub_9E1B0, (unsigned int)sub_9EDD0);
		sub_9D710((Bit8u*)x_DWORD_181C50, 64);
		sub_9D710((Bit8u*)x_DWORD_181DAC, 76);
		sub_9D710((Bit8u*)&x_BYTE_181C90, 256);
		sub_9D710((Bit8u*)&unk_181D90, 24);
		sub_9D710((Bit8u*)&x_DWORD_E3E38, 4);
		sub_9D710((Bit8u*)&x_DWORD_181DA8, 4);
		x_DWORD_E3E3C = 1;
	}
	//return result;
}
// E3E38: using guessed type int x_DWORD_E3E38;
// E3E3C: using guessed type int x_DWORD_E3E3C;
// 181C90: using guessed type char x_BYTE_181C90;
// 181DA8: using guessed type int x_DWORD_181DA8;
// 181DAC: using guessed type int x_DWORD_181DAC[];


//----- (0009EC30) --------------------------------------------------------
void sub_9EC30()
{
	signed int i; // [esp+0h] [ebp-4h]

	sub_9E1B0();
	sub_91A80_AIL_set_preference(0, 200);
	sub_91A80_AIL_set_preference(1, 1);
	sub_91A80_AIL_set_preference(2, 0x8000);
	sub_91A80_AIL_set_preference(3, 100);
	sub_91A80_AIL_set_preference(4, 16);
	sub_91A80_AIL_set_preference(5, 100);
	sub_91A80_AIL_set_preference(6, 655);
	sub_91A80_AIL_set_preference(7, 0);
	sub_91A80_AIL_set_preference(8, 0);
	sub_91A80_AIL_set_preference(9, 1);
	sub_91A80_AIL_set_preference(10, 0);
	sub_91A80_AIL_set_preference(11, 120);
	sub_91A80_AIL_set_preference(12, 8);
	sub_91A80_AIL_set_preference(13, 127);
	sub_91A80_AIL_set_preference(14, 1);
	sub_91A80_AIL_set_preference(15, 0);
	sub_91A80_AIL_set_preference(16, 2);
	sub_91A80_AIL_set_preference(17, 1);
	sub_91A80_AIL_set_preference(18, 1);
	for (i = 0; i < 16; i++)
		x_DWORD_181C50[i] = 0;
	x_BYTE_181C90[0] = 0;
	sub_A10F4_sound_proc_irq();
}
// 181C90: using guessed type char x_BYTE_181C90;

//----- (0009ED70) --------------------------------------------------------
signed int sub_9ED70()
{
	signed int i; // [esp+0h] [ebp-8h]
	AIL_DRIVER* v2; // [esp+4h] [ebp-4h]

	for (i = 15; i >= 0; i--)
	{
		v2 = x_DWORD_181C50[i];
		if (v2)
			sub_93160_AIL_uninstall_driver(v2);
	}
	sub_92E50_AIL_release_all_timers();
	return sub_A11E2();
}

//----- (0009D650) --------------------------------------------------------
bool sub_9D650(unsigned int a1, unsigned int a2)
{
	unsigned int v3; // [esp+0h] [ebp-54h]
	unsigned int v4; // [esp+4h] [ebp-50h]
	int v5; // [esp+Ch] [ebp-48h]
	unsigned int v6; // [esp+10h] [ebp-44h]
	int v7; // [esp+14h] [ebp-40h]
	unsigned int v8; // [esp+1Ch] [ebp-38h]
	int v9; // [esp+20h] [ebp-34h]
	char v10; // [esp+28h] [ebp-2Ch]
	int v11; // [esp+40h] [ebp-14h]
	unsigned int v12; // [esp+48h] [ebp-Ch]
	unsigned int v13; // [esp+4Ch] [ebp-8h]
	int v14; // [esp+50h] [ebp-4h]

	//fix it
	v11 = 0;
	//fix it

	v14 = 0;
	if (a1 >= a2)
		v3 = a2;
	else
		v3 = a1;
	v13 = v3;
	if (a1 < a2)
		v4 = a2;
	else
		v4 = a1;
	v12 = v4 - v13 + 1;
	v5 = 1537;
	v6 = v13 >> 16;
	v7 = (unsigned __int16)v13;
	v8 = v12 >> 16;
	v9 = (unsigned __int16)(v4 - v13 + 1);
	//int386(49, (REGS*)&v5, (REGS*)&v10); // unlock linear region
	return v11 == 0;
}
// 98D52: using guessed type x_DWORD int386(x_DWORD, x_DWORD, x_DWORD);

//----- (0009D710) --------------------------------------------------------
void sub_9D710(Bit8u* a1, int a2)
{
	sub_9D590_lock_linear_region(a1, a1[a2]);
}

//----- (0009D740) --------------------------------------------------------
bool sub_9D740(unsigned int a1, int a2)
{
	return sub_9D650(a1, a2 + a1);
}

//----- (0009D590) --------------------------------------------------------
void sub_9D590_lock_linear_region(Bit8u* a1, unsigned int a2)
{
	//unsigned int v3; // [esp+0h] [ebp-54h]
	//unsigned int v4; // [esp+4h] [ebp-50h]
	//int v5; // [esp+Ch] [ebp-48h]
	//unsigned int v6; // [esp+10h] [ebp-44h]
	//int v7; // [esp+14h] [ebp-40h]
	//unsigned int v8; // [esp+1Ch] [ebp-38h]
	int v9; // [esp+20h] [ebp-34h]
	char v10; // [esp+28h] [ebp-2Ch]
	int v11; // [esp+40h] [ebp-14h]
	//unsigned int v12; // [esp+48h] [ebp-Ch]
	//unsigned int v13; // [esp+4Ch] [ebp-8h]
	//int v14; // [esp+50h] [ebp-4h]

	//fix it
	v11 = 0;
	v9 = 0;
	v10 = 0;
	//fix it

	/*v14 = 0;
	if ( a1 >= a2 )
	  v3 = a2;
	else
	  v3 = a1;
	v13 = v3;
	if ( a1 < a2 )
	  v4 = a2;
	else
	  v4 = a1;
	v12 = v4 - v13 + 1;
	v5 = 0x600;
	v6 = v13 >> 16;
	v7 = (unsigned __int16)v13;
	v8 = v12 >> 16;
	v9 = (unsigned __int16)(v4 - v13 + 1);
	int386(0x31, (REGS*)&v5, (REGS*)&v10);//Lock Linear Region*/ // fix it
	//return v11 == 0; //fix it
}
// 98D52: using guessed type x_DWORD int386(x_DWORD, x_DWORD, x_DWORD);

//----- (000986A0) --------------------------------------------------------
bool sub_986A0_unlock_linear_region()//2796a0
{
	bool result; // eax

	if (x_DWORD_E39BC)
	{
		result = sub_9D650((unsigned int)sub_915A0_sound_proc23, (unsigned int)sub_986A0_unlock_linear_region);
		x_DWORD_E39BC = 0;
	}
	return result;
}
// E39BC: using guessed type int x_DWORD_E39BC;

//----- (000915A0) --------------------------------------------------------
void sub_915A0_sound_proc23()
{
	//bool result; // eax

	if (!x_DWORD_E39BC)
	{
		sub_9D590_lock_linear_region((Bit8u*)sub_915A0_sound_proc23, (unsigned int)sub_986A0_unlock_linear_region);
		sub_9D710((Bit8u*)&x_DWORD_181BF4, 4);
		sub_9D710((Bit8u*)&x_DWORD_181BF8, 4);
		sub_9D710((Bit8u*)&x_DWORD_181BF0_AIL_debugfile, 4);
		sub_9D710((Bit8u*)&x_DWORD_181BFC, 4);
		sub_9D710((Bit8u*)&x_DWORD_181C0C, 4);
		sub_9D710((Bit8u*)&x_DWORD_181C08, 4);
		sub_9D710((Bit8u*)&x_DWORD_181C10, 4);
		sub_9D710((Bit8u*)&x_DWORD_181C00, 4);
		x_DWORD_E39BC = 1;
	}
	//return result;
}
// E39BC: using guessed type int x_DWORD_E39BC;
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181BFC: using guessed type int x_DWORD_181BFC;
// 181C00: using guessed type int x_DWORD_181C00;
// 181C08: using guessed type int x_DWORD_181C08;
// 181C0C: using guessed type int x_DWORD_181C0C;
// 181C10: using guessed type int x_DWORD_181C10;

void sub_91660(HMDIDRIVER user)
{
	if ((x_DWORD_181BFC != 99 || x_DWORD_181C0C != 59 || x_DWORD_181C08 != 59 || x_DWORD_181C10 != 99) && ++x_DWORD_181C10 == 100)
	{
		x_DWORD_181C10 = 0;
		if (++x_DWORD_181C08 == 60)
		{
			x_DWORD_181C08 = 0;
			if (++x_DWORD_181C0C == 60)
			{
				x_DWORD_181C0C = 0;
				++x_DWORD_181BFC;
			}
		}
	}
}

//----- (000916F0) --------------------------------------------------------
/*signed int sub_916F0_sound_proc24()
{
  int i; // [esp+4h] [ebp-14h]
  int j; // [esp+4h] [ebp-14h]
  int v3; // [esp+8h] [ebp-10h]
  int v4; // [esp+Ch] [ebp-Ch]
  int v5; // [esp+10h] [ebp-8h]
  int v6; // [esp+14h] [ebp-4h]

  sub_A0EEC_s_plus2();
  v6 = x_DWORD_181BFC;
  v5 = x_DWORD_181C0C;
  v4 = x_DWORD_181C08;
  v3 = x_DWORD_181C10;
  sub_A0EF9_s_minus2();
  if ( x_DWORD_181C04 == 1 )
  {
	dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "[%.02u:%.02u:%.02u.%.02u] ", v6, v5, v4, v3);
  }
  else
  {
	for ( i = 0; i < 0xE; i++ )
	  dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
  }
  for ( j = 1; j < x_DWORD_181C04; j++ )
	dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
  return 1;
}*/
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BFC: using guessed type int x_DWORD_181BFC;
// 181C04: using guessed type int x_DWORD_181C04;
// 181C08: using guessed type int x_DWORD_181C08;
// 181C0C: using guessed type int x_DWORD_181C0C;
// 181C10: using guessed type int x_DWORD_181C10;

//----- (000919C0) --------------------------------------------------------
signed int sub_919C0()
{
	signed int result; // eax
	bool v1; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v1 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v1)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_shutdown()\n");
	result = sub_9ED70();
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8))
	{
		result = sub_A16A2();
		if (!result)
			result = fclose(x_DWORD_181BF0_AIL_debugfile);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// A1E05: using guessed type x_DWORD fclose(x_DWORD);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000A102C) --------------------------------------------------------
char sub_A102C(int a1)//28202c //fix
{
	unsigned int v1; // et0
	char result; // al
	unsigned int v3; // [esp-4h] [ebp-10h]
	void *retaddr; // [esp+10h] [ebp+4h]

	//fix it
	retaddr = 0;
	//fix it

	v1 = x__readeflags();
	v3 = v1;
	//_disable();
	//__outx_BYTE(0x43u, 0x36u);
	x_DWORD_E3FE6 = a1;
	//__outx_BYTE(0x40u, a1);
	result = BYTE1(a1);
	//__outx_BYTE(0x40u, BYTE1(a1));
	//_disable();
	if (BYTE1(retaddr) & 2)
		;//_enable();
	x__writeeflags(v3);
	return result;
}
// E3FE6: using guessed type int x_DWORD_E3FE6;

//----- (000A105C) --------------------------------------------------------
char sub_A105C(unsigned int a1)//28205c
{
	int v1; // eax

	v1 = 0;
	if (a1 < 0xD68D)
		v1 = 10000 * (unsigned __int64)a1 / 0x20BC;
	return sub_A102C(v1);
}

//----- (000A108F) --------------------------------------------------------
void sub_A108F()//28208f
{
	unsigned int v0; // ecx
	unsigned int v1; // edi

	sub_91BD0_s_plus();
	v0 = -1;
	v1 = 0;
	do
	{
		if (x_DWORD_E3E9C[v1] && x_DWORD_E3F1C[v1] < v0)
			v0 = x_DWORD_E3F1C[v1];
		v1++;
	} while (v1 < 16);
	if (v0 != x_DWORD_E3FEA)
	{
		x_DWORD_E3FEA = v0;
		sub_A105C(v0);
		memset(x_DWORD_E3EDC, 0, 0x40u);
	}
	sub_91BF0_s_minus();
}
// E3E9C: using guessed type int x_DWORD_E3E9C[];
// E3EDC: using guessed type int x_DWORD_E3EDC[];
// E3F1C: using guessed type int x_DWORD_E3F1C[];
// E3FEA: using guessed type int x_DWORD_E3FEA;



//----- (000A10F4) --------------------------------------------------------
void sub_A10F4_sound_proc_irq()//2820f4
{
	unsigned int v0; // et0
	unsigned __int16 v1; // dx
	unsigned int v2; // [esp-8h] [ebp-14h]

	//fix
	v1 = 0;
	//fix

	v0 = x__readeflags();
	v2 = v0;
	//_disable();
	x_WORD_E3FF6 = __DS__;
	sub_A18BE();
	x_DWORD_E3FEE = 0;
	x_DWORD_E3FF2 = 0;
	x_DWORD_E3FEA = -1;
	x_DWORD_E3FF8 = -1;
	memset(x_DWORD_E3E9C, 0, 0x40u);
	memset(x_DWORD_E3EDC, 0, 0x40u);
	memset(x_DWORD_E3F1C, 0, 0x40u);
	memset(x_DWORD_E3F5C, 0, 0x40u);
	/*__asm
	{
	  int     31h; DPMI Services   ax=func xxxxh //eax=200,ebx=8 - system timer?
	  int     21h; DOS - 2+ - GET INTERRUPT VECTOR
	}*/
	x_DWORD_E3FDC = 8;
	x_WORD_E3FE0 = __DS__;
	x_DWORD_E3FE2 = v1;
	// fix it:__asm { int     21h; DOS - SET INTERRUPT VECTOR }
	x_DWORD_E3ED8 = 2;
	sub_92890_AIL_set_timer_period(60, 54925);
	//_disable();
	if (v2 & 0x200)
		;// _enable();
	x__writeeflags(v2);
}
// E3E9C: using guessed type int x_DWORD_E3E9C[];
// E3ED8: using guessed type int x_DWORD_E3ED8;
// E3EDC: using guessed type int x_DWORD_E3EDC[];
// E3F1C: using guessed type int x_DWORD_E3F1C[];
// E3F5C: using guessed type int x_DWORD_E3F5C[];
// E3FDC: using guessed type int x_DWORD_E3FDC;
// E3FE0: using guessed type __int16 x_WORD_E3FE0;
// E3FE2: using guessed type int x_DWORD_E3FE2;
// E3FEA: using guessed type int x_DWORD_E3FEA;
// E3FEE: using guessed type int x_DWORD_E3FEE;
// E3FF2: using guessed type int x_DWORD_E3FF2;
// E3FF6: using guessed type __int16 x_WORD_E3FF6;
// E3FF8: using guessed type int x_DWORD_E3FF8;

//----- (000A11E2) --------------------------------------------------------
signed int sub_A11E2()//2821e2
{
	unsigned int v0; // et0
	signed int result; // eax
	unsigned int v2; // [esp-4h] [ebp-10h]

	v0 = x__readeflags();
	v2 = v0;
	//_disable();
	sub_A102C(0);
	result = 9480;
	// fix it:__asm { int     21h; DOS - SET INTERRUPT VECTOR }
	//_disable();
	if (v2 & 0x200)
		;// _enable();
	x__writeeflags(v2);
	return result;
}
// E3FDC: using guessed type int x_DWORD_E3FDC;
// E3FE0: using guessed type __int16 x_WORD_E3FE0;

//----- (000A121D) --------------------------------------------------------
Bit16u sub_A121D_AIL_API_get_real_vect(Bit32u vectnum/*int a1, __int16 a2, int a3, int a4*/)//28221d
{
	//Bit8u* v4; // ecx
	//v4 = 0;
	//fix it:__asm { int     31h; DPMI Services   ax=func xxxxh }
	/*v4 = a4 << 16;
	LOWORD(v4) = a2;*/
	//fix it:__asm { int     21h; DOS - 2+ - GET INTERRUPT VECTOR }
	//return ac_get_real_vect(vectnum);
	return 0;
}

//----- (000A1249) --------------------------------------------------------
void sub_A1249_AIL_API_set_real_vect(Bit32u vectnum, Bit16u real_ptr)//282249
{
	//ac_set_real_vect(vectnum, real_ptr);
	//signed int result; // eax

	//result = 513;
	//fix it:__asm { int     31h; DPMI Services   ax=func xxxxh }
	//return result;
}

//----- (000A12C5) --------------------------------------------------------
// write access to const memory has been detected, the output may be wrong!
signed int sub_A12C5_sound_proc_irq(int a1, int a2, __int16 a3)//2822c5
{
	signed int result; // eax

	if (x_DWORD_E3FF8 == -1)
	{
		/*__asm
		{
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  lar     ecx, edx
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  lar     ecx, edx
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  lar     ecx, edx
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  int     31h; DPMI Services   ax=func xxxxh
		  lar     ecx, edx
		  int     31h; DPMI Services   ax=func xxxxh
		}*/
		x_DWORD_A128C = (int)((char *)&unk_E4004 - ((unsigned int)&unk_E4004 & 0xFFFFFFF0) + 512);
		x_WORD_A1286 = 0;
		x_WORD_A1277 = 0;
		x_WORD_A12AC = 0;
		x_WORD_A12B3 = a3;
		x_WORD_A12A2 = (__int16)((char *)&x_DWORD_A1270 - ((unsigned int)&x_DWORD_A1270 & 0xFFFFFFF0) + 74);
		x_DWORD_E3FF8 = a1;
		//fix it:__asm { int     31h; DPMI Services   ax=func xxxxh }
		x_DWORD_E3FFC = 49407;
		x_DWORD_E4000 = __DS__;
		result = 517;
		//fix it:__asm { int     31h; DPMI Services   ax=func xxxxh }
	}
	return result;
}
// A1462: write access to const memory at A128C has been detected
// A146A: write access to const memory at A1286 has been detected
// A1473: write access to const memory at A1277 has been detected
// A147C: write access to const memory at A12AC has been detected
// A1485: write access to const memory at A12B3 has been detected
// A1493: write access to const memory at A12A2 has been detected
// A1270: using guessed type int x_DWORD_A1270;
// A1277: using guessed type __int16 x_WORD_A1277;
// A1286: using guessed type __int16 x_WORD_A1286;
// A128C: using guessed type int x_DWORD_A128C;
// A12A2: using guessed type __int16 x_WORD_A12A2;
// A12AC: using guessed type __int16 x_WORD_A12AC;
// A12B3: using guessed type __int16 x_WORD_A12B3;
// E3FF8: using guessed type int x_DWORD_E3FF8;
// E3FFC: using guessed type int x_DWORD_E3FFC;
// E4000: using guessed type int x_DWORD_E4000;

//----- (000A14DB) --------------------------------------------------------
void sub_A14DB_AIL_API_restore_USE16_ISR(int a1)//2824db
{
	//signed int result; // eax

	if (a1 != -1 && a1 == x_DWORD_E3FF8)
	{
		//result = 517;
		//fix it:__asm { int     31h; DPMI Services   ax=func xxxxh }
		x_DWORD_E3FF8 = -1;
	}
	//return result;
}
// E3FF8: using guessed type int x_DWORD_E3FF8;
// E3FFC: using guessed type int x_DWORD_E3FFC;
// E4000: using guessed type int x_DWORD_E4000;

//----- (000A1520) --------------------------------------------------------
unsigned int sub_A1520()//282520
{
	unsigned int v0; // et0
	unsigned int result; // eax

	v0 = x__getcallerseflags();
	result = v0;
	//_disable();
	return result;
}

//----- (000A1524) --------------------------------------------------------
void sub_A1524(unsigned int a1)//282524
{
	void *retaddr; // [esp+4h] [ebp+4h]

	//fix it
	retaddr = 0;
	//fix it

	//_disable();
	if (BYTE1(retaddr) & 2)
		;// _enable();
	x__writeeflags(a1);
}

//----- (000A158B) --------------------------------------------------------
Bit32s sub_A158B_AIL_API_call_driver(AIL_DRIVER* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out)//28258b
//int sub_A158B_AIL_API_call_driver(Bit32s* drvr, __int16 a2, x_WORD *a3, x_WORD *a4)
{
	//return ac_sound_call_driver(drvr, fn, in, out);
	return 0;
	/*__int16 v8; // dx
	__int16 v9; // si
	__int16 v10; // di
	__int16 v11; // cx
	__int16 v12; // dx
	__int16 v13; // si
	__int16 v14; // di
	int v15; // ST00_4
	__int16 v17; // [esp+Eh] [ebp-3Ah]
	Bit8u v18[0x30]; // [esp+10h] [ebp-38h]
	__int16 v19; // [esp+1Eh] [ebp-2Ah]
	__int16 v20; // [esp+22h] [ebp-26h]
	__int16 v21; // [esp+26h] [ebp-22h]
	__int16 v22; // [esp+2Ah] [ebp-1Eh]
	__int16 v23; // [esp+2Eh] [ebp-1Ah]
	__int16 v24; // [esp+30h] [ebp-18h]
	__int16 v25; // [esp+32h] [ebp-16h]

	//fix
	v20 = 0;
	v21 = 0;
	//fix

	sub_91BD0_s_plus();
	memset(&v17, 0, sizeof(v17));
	_CF =0;//fix it:_CF = __CFSHR__(50, 2);
	_ZF = 0;
	_SF = 0;
	memset(v18, 0, 0x30u);
	v25 = 0;
	v24 = 0;
	// fix this - __asm { pushfw }
	v23 = 0;
	v22 = fn;
	if ( in )
	{
	  v8 = in->DI;
	  v9 = in->DX;
	  v10 = in->SI;
	  v21 = in->CX;
	  v20 = v8;
	  HIWORD(v18) = v9;
	  v17 = v10;
	}
	v19 = *(x_WORD *)(*(x_DWORD *)(drvr + 8) + 50);
	//fix it:__asm { int     31h; DPMI Services   ax=func xxxxh }
	if (out)
	{
	  v11 = v21;
	  v12 = v20;
	  v13 = HIWORD(v18);
	  v14 = v17;
	  out->AX = v22;
	  out->CX = v11;
	  out->DI = v12;
	  out->DX = v13;
	  out->SI = v14;
	  out->BX = v19;
	}
	v15 = v22;
	sub_91BF0_s_minus();
	return v15;*/
}

//----- (000A1665) --------------------------------------------------------
void sub_A1665_AIL_API_delay(int a1)//282665
{
	//mydelay(a1);

	/*unsigned __int8 result; // al
	unsigned __int16 v2; // dx
	int i; // ecx
	unsigned __int8 v4; // al

	result = MEMORY[0x463];
	v2 = MEMORY[0x463] + 6;
	for ( i = a1; i; i-- )
	{
	  do
		v4 = __inx_BYTE(v2);
	  while ( !(v4 & 8) );
	  do
		result = __inx_BYTE(v2);
	  while ( result & 8 );
	}
	return result;*/
}

//----- (000A16A2) --------------------------------------------------------
int sub_A16A2()//2826a2
{
	return x_DWORD_E3FEE;
}
// E3FEE: using guessed type int x_DWORD_E3FEE;

//----- (000A16AE) --------------------------------------------------------
HTIMER sub_A16AE_register_timer(AILTIMERCB callback_fn)//2826ae
{
	unsigned int v1; // eax
	unsigned int v2; // ST00_4

	sub_91BD0_s_plus();
	v1 = 0;
	while (*(int *)((char *)x_DWORD_E3E9C + v1))
	{
		v1 += 4;
		if (v1 >= 0x3C)
		{
			v1 = -1;
			goto LABEL_6;
		}
	}
	x_DWORD_E3E9C[v1] = 1;
	x_DWORD_E3E5C_timer[v1] = callback_fn;
LABEL_6:
	v2 = v1;
	sub_91BF0_s_minus();
	return v2;
}
// E3E5C: using guessed type int x_DWORD_E3E5C[];
// E3E9C: using guessed type int x_DWORD_E3E9C[];

//----- (000A16F5) --------------------------------------------------------
signed __int32 sub_A16F5_AIL_API_set_timer_user(int a1, signed __int32 a2)//2826f5
{
	signed __int32 v2; // eax
	signed __int32 v3; // ST00_4

	//
	v2 = 0;
	//

	sub_91BD0_s_plus();
	if (a1 != -1)
		;// v2 = x_InterlockedExchange((long*)(int *)((char *)x_DWORD_E3F9C + a1), a2);
	v3 = v2;
	sub_91BF0_s_minus();
	return v3;
}
// E3F9C: using guessed type int x_DWORD_E3F9C[];

//----- (000A171D) --------------------------------------------------------
void sub_A171D_release_timer_handle(Bit32s a1)//28271d
{
	sub_91BD0_s_plus();
	if (a1 != -1)
		*(int *)((char *)x_DWORD_E3E9C + a1) = 0;
	sub_91BF0_s_minus();
}
// E3E9C: using guessed type int x_DWORD_E3E9C[];

//----- (000A1744) --------------------------------------------------------
void sub_A1744_AIL_API_release_all_timers()//282744
{
	int v0; // esi
	unsigned __int8 v1; // of

	sub_91BD0_s_plus();
	v0 = 56;
	do
	{
		sub_92DC0_AIL_release_timer_handle(v0);
		v1 = __OFSUB__(v0, 4);
		v0 -= 4;
	} while (!((v0 < 0) ^ v1));
	sub_91BF0_s_minus();
}

//----- (000A1768) --------------------------------------------------------
void sub_A1768_start_timer(HTIMER timer)//282768
{
	sub_91BD0_s_plus();
	if (timer != -1 && *(int *)((char *)x_DWORD_E3E9C + timer) == 1)
		*(int *)((char *)x_DWORD_E3E9C + timer) = 2;
	sub_91BF0_s_minus();
}
// E3E9C: using guessed type int x_DWORD_E3E9C[];

//----- (000A1798) --------------------------------------------------------
void sub_A1798()//282798
{
	int v0; // esi
	unsigned __int8 v1; // of

	sub_91BD0_s_plus();
	v0 = 56;
	do
	{
		sub_92BA0_AIL_start_timer(v0);
		v1 = __OFSUB__(v0, 4);
		v0 -= 4;
	} while (!((v0 < 0) ^ v1));
	sub_91BF0_s_minus();
}

//----- (000A17BC) --------------------------------------------------------
void sub_A17BC_stop_timer(HTIMER timer)//2827bc
{
	sub_91BD0_s_plus();
	if (timer != -1 && *(int *)((char *)x_DWORD_E3E9C + timer) == 2)
		*(int *)((char *)x_DWORD_E3E9C + timer) = 1;
	sub_91BF0_s_minus();
}
// E3E9C: using guessed type int x_DWORD_E3E9C[];

//----- (000A17EC) --------------------------------------------------------
void sub_A17EC()//2827ec
{
	int v0; // esi
	unsigned __int8 v1; // of

	sub_91BD0_s_plus();
	v0 = 56;
	do
	{
		sub_92CB0_AIL_stop_timer(v0);
		v1 = __OFSUB__(v0, 4);
		v0 -= 4;
	} while (!((v0 < 0) ^ v1));
	sub_91BF0_s_minus();
}

//----- (000A1810) --------------------------------------------------------
void sub_A1810_set_timer_period(HTIMER timer, Bit32u microseconds)//282810
{
	sub_91BD0_s_plus();
	*(int *)((char *)x_DWORD_E3F1C + timer) = microseconds;
	*(int *)((char *)x_DWORD_E3EDC + timer) = 0;
	sub_A108F();
	sub_91BF0_s_minus();
}
// E3EDC: using guessed type int x_DWORD_E3EDC[];
// E3F1C: using guessed type int x_DWORD_E3F1C[];

//----- (000A1840) --------------------------------------------------------
void sub_A1840_AIL_API_set_timer_frequency(HTIMER timer, Bit32u hertz)//282840
{
	sub_91BD0_s_plus();
	sub_92890_AIL_set_timer_period(timer, 1000000 / hertz);
	sub_91BF0_s_minus();
}




//----- (00091A80) --------------------------------------------------------
int sub_91A80_AIL_set_preference(Bit32u number, Bit32s value)//int (__stdcall *mss32_AIL_set_preference)(unsigned int, int) = 0;
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_preference(%d,%d)\n", number, value);
	result = sub_9E6E0_set_preference(number, value);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00091BD0) --------------------------------------------------------
void sub_91BD0_s_plus()//fix it - remove
{
	sub_A0EEC_s_plus2();
}

//----- (00091BF0) --------------------------------------------------------
void sub_91BF0_s_minus()//fix it - remove
{
	sub_A0EF9_s_minus2();
}

//----- (00091C10) --------------------------------------------------------
Bit16u sub_91C10_AIL_get_real_vect(/*VDI_HDR* a1, int a2, int a3,*/ Bit32u vectnum/*U32       vectnum*/)//AIL_get_real_vect
{
	int v4; // eax
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v8; // [esp+4h] [ebp-Ch]
	Bit16u result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v8 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && (v4 = sub_A16A2(), !v4 && sub_916F0_sound_proc24());
	if (v8)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_get_real_vect(0x%X)\n", vectnum);
	result = sub_A121D_AIL_API_get_real_vect(vectnum/*, a1, a3, a2*/);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00091D50) --------------------------------------------------------
void sub_91D50_AIL_set_real_vect(Bit32u vectnum, Bit16u real_ptr)//AIL_set_real_vect
{
	//signed int result; // eax
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_real_vect(0x%X,0x%X)\n", vectnum, real_ptr);
	sub_A1249_AIL_API_set_real_vect(vectnum, real_ptr);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00091E90) --------------------------------------------------------
void sub_91E90_AIL_restore_USE16_ISR(Bit32s a1)//AIL_restore_USE16_ISR
{
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_restore_USE16_ISR(%d)\n", a1);
	sub_A14DB_AIL_API_restore_USE16_ISR(a1);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00091F20) --------------------------------------------------------
unsigned int sub_91F20()
{
	return sub_A1520();
}

//----- (00091F50) --------------------------------------------------------
void sub_91F50(unsigned int a1)
{
	sub_A1524(a1);
}

//----- (00091F70) --------------------------------------------------------
Bit32s sub_91F70_AIL_call_driver(AIL_DRIVER* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out)//AIL_call_driver //272f70
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v7; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v7 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v7)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_call_driver(0x%X,0x%X,0x%X,0x%X)\n", drvr, fn, in, out);
	result = sub_A158B_AIL_API_call_driver(drvr, fn, in, out);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000920D0) --------------------------------------------------------
void sub_920D0_AIL_delay(int a1)//AIL_delay
{
	//unsigned __int8 result; // al
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_delay(%d)\n", a1);
	/*result = */sub_A1665_AIL_API_delay(a1);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092160) --------------------------------------------------------
int sub_92160()
{
	return sub_A16A2();
}

//----- (00092190) --------------------------------------------------------
int sub_92190_AIL_read_INI(AIL_INI* INI, char* filename/*, char* a8, IO_PARMS IO*/)//AIL_read_INI // 273190
{
	int i1; // [esp+0h] [ebp-28h]
	int i2; // [esp+0h] [ebp-28h]
	int mm; // [esp+4h] [ebp-24h]
	int nn; // [esp+4h] [ebp-24h]
	int kk; // [esp+8h] [ebp-20h]
	int ll; // [esp+8h] [ebp-20h]
	int ii; // [esp+Ch] [ebp-1Ch]
	int jj; // [esp+Ch] [ebp-1Ch]
	int m; // [esp+10h] [ebp-18h]
	int n; // [esp+10h] [ebp-18h]
	int k; // [esp+14h] [ebp-14h]
	int l; // [esp+14h] [ebp-14h]
	int i; // [esp+18h] [ebp-10h]
	int j; // [esp+18h] [ebp-10h]
	bool v17; // [esp+1Ch] [ebp-Ch]
	int result; // [esp+24h] [ebp-4h]

	x_DWORD_181C04++;
	v17 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v17)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_API_read_INI(0x%X,%s)\n", INI, filename);
	result = sub_9E3A0_AIL_API_read_INI(INI, filename/*/*a8, &IO*/);
	if (result)
	{
		if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
		{
			for (i = 0; i < 0xE; i++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
			for (j = 1; j < x_DWORD_181C04; j++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Driver = %s\n", INI->driver_name);
			for (k = 0; k < 0xE; k++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
			for (l = 1; l < x_DWORD_181C04; l++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Device = %s\n", INI->device_name);
			for (m = 0; m < 0xE; m++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
			for (n = 1; n < x_DWORD_181C04; n++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "IO     = %X\n", INI->IO.IO);//*(signed __int16 *)(a1 + 256)
			for (ii = 0; ii < 0xE; ii++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
			for (jj = 1; jj < x_DWORD_181C04; jj++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "IRQ    = %d\n", INI->IO.IRQ);//*(signed __int16 *)(a1 + 258)
			for (kk = 0; kk < 0xE; kk++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
			for (ll = 1; ll < x_DWORD_181C04; ll++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "DMA_8  = %d\n", INI->IO.DMA_8_bit);//*(signed __int16 *)(a1 + 260)
			for (mm = 0; mm < 0xE; mm++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
			for (nn = 1; nn < x_DWORD_181C04; nn++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "DMA_16 = %d\n", INI->IO.DMA_16_bit);//*(signed __int16 *)(a1 + 262)
		}
	}
	else if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i1 = 0; i1 < 0xE; i1++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (i2 = 1; i2 < x_DWORD_181C04; i2++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %u\n", 0);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092600) --------------------------------------------------------
HTIMER sub_92600_AIL_register_timer(AILTIMERCB callback_fn/*AILTIMERCB    callback_fn*/)//AIL_register_timer
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v4; // [esp+4h] [ebp-Ch]
	HTIMER result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_register_timer(0x%X)\n", callback_fn);
	result = sub_A16AE_register_timer(callback_fn);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %u\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092740) --------------------------------------------------------
signed __int32 sub_92740_AIL_set_timer_user(int timer/*HTIMER      timer*/, signed __int32 user/*UINTa       user*/)//AIL_set_timer_user
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	signed __int32 v6; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_timer_user(%u,%u)\n", timer, user);
	v6 = sub_A16F5_AIL_API_set_timer_user(timer, user);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %u\n", v6);
	}
	x_DWORD_181C04--;
	return v6;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092890) --------------------------------------------------------
void sub_92890_AIL_set_timer_period(HTIMER timer, Bit32u microseconds)//AIL_set_timer_period
{
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_timer_period(%u,%u)\n", timer, microseconds);
	sub_A1810_set_timer_period(timer, microseconds);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092930) --------------------------------------------------------
void sub_92930_AIL_set_timer_frequency(HTIMER timer/*HTIMER timer*/, Bit32u hertz/*U32 hertz*/)//AIL_set_timer_frequency
{
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_timer_frequency(%u,%u)\n", timer, hertz);
	sub_A1840_AIL_API_set_timer_frequency(timer, hertz);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092BA0) --------------------------------------------------------
void sub_92BA0_AIL_start_timer(HTIMER timer/*HTIMER      timer*/)//AIL_start_timer
{
	bool v1; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v1 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v1)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_start_timer(%u)\n", timer);
	sub_A1768_start_timer(timer);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092CB0) --------------------------------------------------------
void sub_92CB0_AIL_stop_timer(HTIMER timer/*HTIMER      timer*/)//AIL_stop_timer
{
	bool v1; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v1 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v1)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_stop_timer(%u)\n", timer);
	sub_A17BC_stop_timer(timer);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092DC0) --------------------------------------------------------
void sub_92DC0_AIL_release_timer_handle(Bit32s timer/*HTIMER      timer*/)//AIL_release_timer_handle
{
	bool v1; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v1 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v1)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_release_timer_handle(%u)\n", timer);
	sub_A171D_release_timer_handle(timer);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092E50) --------------------------------------------------------
void sub_92E50_AIL_release_all_timers()//AIL_release_all_timers
{
	bool v0; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v0 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v0)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_release_all_timers()\n");
	sub_A1744_AIL_API_release_all_timers();
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00092ED0) --------------------------------------------------------
IO_PARMS* sub_92ED0_AIL_get_IO_environment(AIL_DRIVER* drvr/*AIL_DRIVER *drvr  */)//AIL_get_IO_environment  
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v4; // [esp+4h] [ebp-Ch]
	IO_PARMS* result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_get_IO_environment(0x%X)\n", drvr);
	result = sub_9EB60_AIL_API_get_IO_environment(drvr);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093010) --------------------------------------------------------
AIL_DRIVER* sub_93010_AIL_install_driver(/*int a1, */Bit8u* driver_image, Bit32s n_bytes)//AIL_install_driver //274010
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v6; // [esp+4h] [ebp-Ch]
	AIL_DRIVER *v7; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v6 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v6)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_install_driver(0x%X,%u)\n", driver_image, n_bytes);
	v7 = sub_9E720_AIL_API_install_driver(/*a1, */driver_image, n_bytes);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", v7);
	}
	x_DWORD_181C04--;
	return v7;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093160) --------------------------------------------------------
int sub_93160_AIL_uninstall_driver(AIL_DRIVER* drvr/*AIL_DRIVER *drvr  */)//AIL_uninstall_driver  
{
	int result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_uninstall_driver(0x%X)\n", drvr);
	result = sub_9EA60_AIL_API_uninstall_driver(drvr);
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000A3600) --------------------------------------------------------
HDIGDRIVER sub_A3600_AIL_API_install_DIG_driver_file(/*int a1,*/ char* filename, IO_PARMS* IO)//284600
{
	//ac_sound_install_dig_driver_file
	int v3; // eax
	HDIGDRIVER result; // [esp+0h] [ebp-10h]
	Bit8u* v6; // [esp+4h] [ebp-Ch]
	HDIGDRIVER v7; // [esp+8h] [ebp-8h]
	AIL_DRIVER *v8; // [esp+Ch] [ebp-4h]

	v6 = sub_9DEA0_read_file(filename, 0);
	if (v6)
	{
		v3 = sub_9DE20_get_file_lenght(filename);
		v8 = sub_93010_AIL_install_driver(/*a1, */v6, v3);
		/*
		chyby
		2 a 3 c095 versus 382c

		*/
		x_free((void*)v6);
		if (v8)
		{
			//   IO
			v7 = sub_A2EA0(v8, *IO);
			if (!v7)
				sub_93160_AIL_uninstall_driver(v8);
			result = v7;
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		strcpy(x_BYTE_181C90, strlen("Driver file not found\n") + 1, "Driver file not found\n");
		result = 0;
	}
	return result;
}
// E3E34: using guessed type int (*off_E3E34_freex)(int);
// 181C90: using guessed type char x_BYTE_181C90;

//----- (00093330) --------------------------------------------------------
HDIGDRIVER sub_93330_AIL_install_DIG_driver_file(/*int a1, */char* filename, IO_PARMS* IO)//AIL_install_DIG_driver_file // 274330
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v6; // [esp+4h] [ebp-Ch]
	HDIGDRIVER result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v6 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v6)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_install_DIG_driver_file(%s,0x%X)\n", filename, IO);
	result = sub_A3600_AIL_API_install_DIG_driver_file(/*a1, */filename, IO);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000A3720) --------------------------------------------------------
int sub_A3720_AIL_API_install_DIG_INI(/*int a1, */HDIGDRIVER* dig)//284720
{
	//ac_sound_install_dig_ini
	//Bit8u v3[4096]; // [esp+0h] [ebp-11Ch]
	char v4; // [esp+80h] [ebp-9Ch]
	char v5; // [esp+100h] [ebp-1Ch]
	int result; // [esp+118h] [ebp-4h]

	//char filename[200];
	//IO_PARMS IO = {};
	AIL_INI a2;//fix
	//a2 = jmeno souboru
	if (sub_92190_AIL_read_INI(/*(Bit8u*)a1, */&a2, (char*)"DIG.INI"/*, filename, IO*/))
	{//3550c8 - SB16.DIG  //355148 - 220
		*dig = sub_93330_AIL_install_DIG_driver_file(/*a1, */a2.driver_name, &a2.IO);
		if (*dig)
			result = 0;
		else
			result = 2;
	}
	else
	{
		qmemcpy(&x_BYTE_181C90, (void*)"Unable to open file DIG.INI\n", 0x1Du);
		result = 1;
	}
	return result;
}
// 181C90: using guessed type char x_BYTE_181C90;

//----- (000A37A0) --------------------------------------------------------
int sub_A37A0(unsigned int *a1)
{
	return sub_93160_AIL_uninstall_driver((AIL_DRIVER*)*a1);
}

//----- (000931F0) --------------------------------------------------------
int sub_931F0_AIL_install_DIG_INI(/*int a1,*/ HDIGDRIVER* dig/*HDIGDRIVER *dig*/)//AIL_install_DIG_INI //2741f0
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_install_DIG_INI(0x%X)\n", dig);
	result = sub_A3720_AIL_API_install_DIG_INI(/*a1,*/ dig);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;





//----- (00093480) --------------------------------------------------------
int sub_93480_AIL_uninstall_DIG_driver(unsigned int *dig/*HDIGDRIVER dig*/)//AIL_uninstall_DIG_driver //274480
{
	int result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_uninstall_DIG_driver(0x%X)\n", dig);
	result = sub_A37A0(dig);
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093510) --------------------------------------------------------
HSAMPLE sub_93510_AIL_allocate_sample_handle(HDIGDRIVER dig/*HDIGDRIVER dig*/)//AIL_allocate_sample_handle //00274510
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v4; // [esp+4h] [ebp-Ch]
	HSAMPLE result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_allocate_sample_handle(0x%X)\n", dig);
	result = sub_A3820_allocate_sample_handle(dig);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000937A0) --------------------------------------------------------
int sub_937A0_AIL_release_sample_handle(int S/*HSAMPLE S*/)//AIL_release_sample_handle //2747a0
{
	int result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_release_sample_handle(0x%X)\n", S);
	result = sub_A38C0_AIL_API_release_sample_handle(S);
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093830) --------------------------------------------------------
void sub_93830_AIL_init_sample(HSAMPLE S/*HSAMPLE S*/)//AIL_init_sample //274830
{
	//int result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_init_sample(0x%X)\n"/*, (char)S*/);
	/*result = */sub_A38E0_init_sample(S);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000938C0) --------------------------------------------------------
Bit32s sub_938C0_AIL_set_sample_file(HSAMPLE S/*HSAMPLE S*/, Bit8u* file_image/*void const FAR *file_image*/, Bit32s block/*S32 block*/)//AIL_set_sample_file //2748c0
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v6; // [esp+4h] [ebp-Ch]
	signed int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04--;
	v6 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v6)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sample_file(0x%X,0x%X,%d)\n", S, file_image, block);
	result = sub_A4B20_set_sample_file(S, file_image, block);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093A10) --------------------------------------------------------
void sub_93A10_AIL_set_sample_address(HSAMPLE S, Bit8u* start, Bit32u len)//AIL_set_sample_address //274a10
{
	//x_DWORD *result; // eax
	bool v4; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sample_address(0x%X,0x%X,%u)\n", S, start, len);
	/*result = */sub_A3A30_set_sample_address(S, start, len);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093AB0) --------------------------------------------------------
void sub_93AB0_AIL_set_sample_type(HSAMPLE S, Bit32s a2, Bit32u a3)//AIL_set_sample_type //274ab0
{
	//int result; // eax
	bool v4; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sample_type(0x%X,%d,%u)\n", S, a2, a3);
	/*result = */sub_A3A70_set_sample_type(S, a2, a3);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093B50) --------------------------------------------------------
void sub_93B50_AIL_start_sample(HSAMPLE S/*HSAMPLE S*/)//AIL_start_sample //274b50
{
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_start_sample(0x%X)\n", S);
	sub_A3CB0_API_AIL_start_sample(S);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093D00) --------------------------------------------------------
void sub_93D00_AIL_end_sample(HSAMPLE S/*HSAMPLE S*/)//274d00 AIL_end_sample //274d00
{
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_end_sample(0x%X)\n", S);
	sub_A3DA0__API_AIL_end_sample(S);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093D90) --------------------------------------------------------
void sub_93D90_AIL_set_sample_playback_rate(HSAMPLE S, Bit32s playback_rate)//AIL_set_sample_playback_rate //274d90
{
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04--;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sample_playback_rate(0x%X,%d)\n", S, playback_rate);
	sub_A3AF0_set_sample_playback_rate(S, playback_rate);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093E30) --------------------------------------------------------
void sub_93E30_AIL_set_sample_volume(HSAMPLE S, Bit32s volume)//AIL_set_sample_volume //274e30
{
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sample_volume(0x%X,%d)\n", S, volume);
	sub_A3B40_set_sample_volume(S, volume);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093ED0) --------------------------------------------------------
void sub_93ED0_AIL_set_sample_volume_pan(HSAMPLE S, Bit32s volume)//AIL_set_sample_volume_pan //274ed0
{
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sample_pan(0x%X,%d)\n", S, volume);
	sub_A3BB0_set_sample_volume_pan(S, volume);
	x_DWORD_181C04++;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00093F70) --------------------------------------------------------
void sub_93F70_AIL_set_sample_loop_count(HSAMPLE S/*HSAMPLE S*/, int loop_count/*S32 loop_count*/)//AIL_set_sample_loop_count //274f70
{
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sample_loop_count(0x%X,%d)\n", S, loop_count);
	sub_A3C90_set_sample_loop_count(S, loop_count);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00094010) --------------------------------------------------------
Bit32u sub_94010_AIL_sample_status(HSAMPLE S)//AIL_sample_status
{
	return SOUND_sample_status(S);
}

Bit32u sub_94010_AIL_sample_status_orig(HSAMPLE S)//AIL_sample_status
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v4; // [esp+4h] [ebp-Ch]
	Bit32u result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_sample_status(0x%X)\n", S);
	result = sub_A3A00_sample_status(S);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %u\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00094650) --------------------------------------------------------
void sub_94650_AIL_set_digital_master_volume(x_DWORD *a1, Bit32s master_volume)//AIL_set_digital_master_volume
{
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_digital_master_volume(0x%X,%d)\n", a1, master_volume);
	sub_A3F70_set_digital_master_volume(a1, master_volume);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095140) --------------------------------------------------------
EOS_callback_type sub_95140_AIL_register_EOS_callback(HSAMPLE S/*HSAMPLE S*/, EOS_callback_type EOS/*AILSAMPLECB EOS*/)//AIL_register_EOS_callback
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	EOS_callback_type result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_register_EOS_callback(0x%X,0x%X)\n", S, EOS);
	result = sub_A3EB0_register_EOS_callback(S, EOS);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000953E0) --------------------------------------------------------
void sub_953E0_AIL_set_sample_user_data(HSAMPLE S/*HSAMPLE S*/, int index/*U32     index*/, int value/* SINTa   value*/)//AIL_set_sample_user_data
{
	//int result; // eax
	bool v4; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sample_user_data(0x%X,%u,%d)\n", S, index, value);
	/*result = */sub_A3F00_set_sample_user_data(S, index, value);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095480) --------------------------------------------------------
int sub_95480_AIL_sample_user_data(HSAMPLE S/*HSAMPLE S*/, int index/*U32     index*/)//AIL_sample_user_data
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_sample_user_data(0x%X,%u)\n", S, index);
	result = sub_A3F30_sample_user_data(S, index);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095710) --------------------------------------------------------
Bit32s sub_95710_AIL_install_MDI_INI(/*int a1,*/ HMDIDRIVER *mdi)//HMDIDRIVER *mdi AIL_install_MDI_INI //276710
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_install_MDI_INI(0x%X)\n", mdi);
	result = sub_A78F0_AIL_API_install_MDI_INI(/*a1,*/ mdi);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095850) --------------------------------------------------------
HMDIDRIVER sub_95850_AIL_install_MDI_driver_file(char* filename, IO_PARMS *IO)//HMDIDRIVER *mdi AIL_install_MDI_driver_file //276850
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v6; // [esp+4h] [ebp-Ch]
	HMDIDRIVER result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v6 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v6)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_install_MDI_driver_file(%s,0x%X)\n", filename, IO);
	result = sub_A77D0_AIL_API_install_MDI_INI(filename, IO);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000959A0) --------------------------------------------------------
int sub_959A0_AIL_uninstall_MDI_driver(HMDIDRIVER mdi/*HMDIDRIVER  mdi*/)//AIL_uninstall_MDI_driver
{
	int result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_uninstall_MDI_driver(0x%X)\n", mdi);
	result = sub_A7970_AIL_API_uninstall_MDI_driver(mdi);
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095A30) --------------------------------------------------------
HSEQUENCE sub_95A30_AIL_allocate_sequence_handle(HMDIDRIVER mdi)//AIL_allocate_sequence_handle
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v4; // [esp+4h] [ebp-Ch]
	HSEQUENCE result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_allocate_sequence_handle(0x%X)\n", mdi);
	result = sub_A7B30_AIL_API_allocate_sequence_handle(mdi);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095C00) --------------------------------------------------------
Bit32s sub_95C00_AIL_init_sequence(HSEQUENCE S, Bit8u* start, Bit32s sequence_num, Bit32u track)//AIL_init_sequence
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v6; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v6 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v6)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_init_sequence(0x%X,0x%X,%d)\n", S, start, sequence_num);
	result = sub_A7C20_AIL_API_init_sequence(S, start, sequence_num, track);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095D50) --------------------------------------------------------
void sub_95D50_AIL_start_sequence(HSEQUENCE S/*HSEQUENCE S*/, Bit32u track)//AIL_start_sequence
{
	//int *result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_start_sequence(0x%X)\n", S);
	/*result = */sub_A8010_AIL_API_start_sequence(S, track);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095DE0) --------------------------------------------------------
void sub_95DE0_AIL_stop_sequence(HSEQUENCE S/*HSEQUENCE S*/)//AIL_stop_sequence
{
	//int result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_stop_sequence(0x%X)\n", S);
	/*result = */sub_A8050_AIL_API_stop_sequence(S);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095E70) --------------------------------------------------------
void sub_95E70_AIL_resume_sequence(HSEQUENCE S)//AIL_resume_sequence
{
	bool v1; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v1 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v1)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_resume_sequence(0x%X)\n", S);
	sub_A8180_AIL_API_resume_sequence(S);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00095F00) --------------------------------------------------------
void sub_95F00_AIL_end_sequence(HSEQUENCE S/*HSEQUENCE S*/)//AIL_end_sequence
{
	//int result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_end_sequence(0x%X)\n", S);
	/*result = */sub_A8250_AIL_API_end_sequence(S);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00096030) --------------------------------------------------------
void sub_96030_AIL_set_sequence_volume(HSEQUENCE S, Bit32s volume, Bit32s milliseconds)//AIL_set_sequence_volume
{
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_sequence_volume(0x%X,%d,%d)\n", S, volume, milliseconds);
	sub_A8360_AIL_API_set_sequence_volume(S, volume, milliseconds);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00096170) --------------------------------------------------------
int sub_96170_AIL_sequence_status(HSEQUENCE S/*HSEQUENCE S*/)//AIL_sequence_status
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v4; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_sequence_status(0x%X)\n", S);
	result = sub_A8410_AIL_API_sequence_status(S);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %u\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00096670) --------------------------------------------------------
void sub_96670_AIL_set_XMIDI_master_volume(HMDIDRIVER mdi, Bit32s master_volume)//AIL_set_XMIDI_master_volume
{
	x_DWORD_181C04++;
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24())
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_XMIDI_master_volume(0x%X,%d)\n", mdi, master_volume);
	sub_A84D0_AIL_API_set_XMIDI_master_volume(mdi, master_volume);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000969A0) --------------------------------------------------------
Bit32s sub_969A0_AIL_MDI_driver_type(HMDIDRIVER mdi)//AIL_MDI_driver_type
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v4; // [esp+4h] [ebp-Ch]
	signed int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_MDI_driver_type(0x%X)\n", mdi);
	result = sub_A7990_AIL_API_MDI_driver_type(mdi);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00096AE0) --------------------------------------------------------
void sub_96AE0_AIL_set_GTL_filename_prefix(Bit8u* prefix/*char const *prefix*/)//AIL_set_GTL_filename_prefix
{
	//int result; // eax
	bool v2; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v2 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v2)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_set_GTL_filename_prefix(%s)\n", prefix);
	sub_A7AA0_AIL_API_set_GTL_filename_prefix(prefix);
	x_DWORD_181C04--;
	//return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00097330) --------------------------------------------------------
int sub_97330_AIL_sequence_position(x_DWORD *S/*HSEQUENCE S*/, x_DWORD *beat/*FAR *beat*/, x_DWORD *measure/*FAR *measure*/)//AIL_sequence_position
{
	int result; // eax
	int i; // [esp+0h] [ebp-8h]
	int j; // [esp+0h] [ebp-8h]
	bool v6; // [esp+4h] [ebp-4h]

	x_DWORD_181C04++;
	v6 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v6)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_sequence_position(0x%X,0x%X,0x%X)\n", S, beat, measure);
	result = sub_A8830(S, beat, measure);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8))
	{
		result = sub_A16A2();
		if (!result)
		{
			for (i = 0; i < 0xE; i++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
			for (j = 1; j < x_DWORD_181C04; j++)
				dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
			/*result = */dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d:%d\n", *measure + 1, *beat + 1);
		}
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00097480) --------------------------------------------------------
x_DWORD *sub_97480_AIL_branch_index(HSEQUENCE S/*HSEQUENCE S*/, int marker/*U32 marker*/)//AIL_branch_index
{
	x_DWORD *result; // eax
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_branch_index(0x%X,%u)\n", S, marker);
	result = sub_A8900(S, marker);
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00097670) --------------------------------------------------------
xCALLBACK sub_97670_AIL_register_trigger_callback(HSEQUENCE S/*HSEQUENCE S*/, xCALLBACK callback/*AILTRIGGERCB   callback*/)//AIL_register_trigger_callback
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	xCALLBACK result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_register_trigger_callback(0x%X,0x%X)\n", S, callback);
	result = sub_A8A20_AIL_API_trigger_callback(S, callback);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00097A60) --------------------------------------------------------
int sub_97A60_AIL_register_event_callback(int mdi/*HMDIDRIVER mdi*/, int callback/*AILEVENTCB callback*/)//AIL_register_event_callback
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	int v6; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_register_event_callback(0x%X,0x%X)\n", mdi, callback);
	v6 = sub_A8AE0(mdi, callback);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", v6);
	}
	x_DWORD_181C04--;
	return v6;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00097BB0) --------------------------------------------------------
int sub_97BB0_AIL_register_timbre_callback(int mdi/*HMDIDRIVER mdi*/, int callback/*AILTIMBRECB callback*/)//AIL_register_timbre_callback
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v5; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04++;
	v5 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v5)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_register_timbre_callback(0x%X,0x%X)\n", mdi, callback);
	result = sub_A8B20(mdi, callback);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = 0x%X\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00097F90) --------------------------------------------------------
int sub_97F90_AIL_lock_channel(x_DWORD *mdi/*HMDIDRIVER mdi*/)//AIL_lock_channel
{
	int i; // [esp+0h] [ebp-10h]
	int j; // [esp+0h] [ebp-10h]
	bool v4; // [esp+4h] [ebp-Ch]
	int result; // [esp+Ch] [ebp-4h]

	x_DWORD_181C04--;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_lock_channel(0x%X)\n", mdi);
	result = sub_A8BF0(mdi);
	if (x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2())
	{
		for (i = 0; i < 0xE; i++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D226C);
		for (j = 1; j < x_DWORD_181C04; j++)
			dbgfprintf(x_DWORD_181BF0_AIL_debugfile, (const char *)&unk_D2270);
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "Result = %d\n", result);
	}
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (000980D0) --------------------------------------------------------
int sub_980D0_AIL_release_channel(x_DWORD *mdi/*HMDIDRIVER mdi*/, int channel/*S32 channel*/)//AIL_release_channel
{
	int result; // eax
	bool v3; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v3 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v3)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_release_channel(0x%X,%d)\n", mdi, channel);
	result = sub_A8EA0(mdi, channel);
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00098170) --------------------------------------------------------
int sub_98170_AIL_map_sequence_channel(x_DWORD *S/*HSEQUENCE S*/, int seq_channel/*S32 seq_channel*/, int new_channel/*S32 new_channel*/)//AIL_map_sequence_channel
{
	int result; // eax
	bool v4; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v4 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v4)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_map_sequence_channel(0x%X,%d,%d)\n", S, seq_channel, new_channel);
	result = sub_A9080(S, seq_channel, new_channel);
	x_DWORD_181C04--;
	return result;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;

//----- (00098360) --------------------------------------------------------
void sub_98360_AIL_send_channel_voice_message(HMDIDRIVER mdi, HSEQUENCE S, Bit32s status, Bit32s data_1, Bit32s data_2)//AIL_send_channel_voice_message // 279360
{
	bool v6; // [esp+0h] [ebp-4h]

	x_DWORD_181C04++;
	v6 = x_DWORD_181BF4 && (x_DWORD_181C04 == 1 || x_DWORD_181BF8) && !sub_A16A2() && sub_916F0_sound_proc24();
	if (v6)
		dbgfprintf(x_DWORD_181BF0_AIL_debugfile, "AIL_send_channel_voice_message(0x%X,0x%X,0x%X,0x%X,0x%X)\n", mdi, S, status, data_1, data_2);
	sub_A9140_AIL_API_send_channel_voice_message(mdi, S, status, data_1, data_2);
	x_DWORD_181C04--;
}
// A18E3: using guessed type x_DWORD fprintf(x_DWORD, const char *, ...);
// 181BF0: using guessed type int x_DWORD_181BF0_AIL_debugfile;
// 181BF4: using guessed type int x_DWORD_181BF4;
// 181BF8: using guessed type int x_DWORD_181BF8;
// 181C04: using guessed type int x_DWORD_181C04;


//----- (000A1870) --------------------------------------------------------
void sub_A1870(int a1, unsigned int a2)//282870
{
	int v2; // eax

	sub_91BD0_s_plus();
	if (a2)
		v2 = 10000 * (unsigned __int64)a2 / 0x2E9C;
	else
		v2 = 54925;
	sub_92890_AIL_set_timer_period(a1, v2);
	sub_91BF0_s_minus();
}

//----- (000A18B2) --------------------------------------------------------
int sub_A18B2()//2828b2
{
	return x_DWORD_E3FE6;
}
// E3FE6: using guessed type int x_DWORD_E3FE6;

//----- (000A18BE) --------------------------------------------------------
void sub_A18BE()//2828be
{
	//sub_9D590_lock_linear_region((Bit8u*)x_DWORD_E3E5C, (unsigned int)x_BYTE_E4A0C);
	//sub_9D590_lock_linear_region((Bit8u*)sub_A0EEC_s_plus2, (unsigned int)sub_A18BE);
}
// E3E5C: using guessed type int x_DWORD_E3E5C[];


//----- (000A2EA0) --------------------------------------------------------
HDIGDRIVER sub_A2EA0(AIL_DRIVER* a1, IO_PARMS IO)//283ea0
{
	int v2; // ST0C_4
	int v3; // ST08_4
	int v4; // ST0C_4
	int v5; // ST08_4
	Bit8u* v6; // eax
	int v7; // ST0C_4
	int v8; // ST08_4
	int v9; // ST0C_4
	int v10; // ST08_4
	unsigned int v11; // eax
	int v12; // ST0C_4
	int v13; // ST08_4
	Bit8u v15[0x18 * 4]; // [esp+0h] [ebp-50h]
	VDI_CALL v16/*[100]*/; // [esp+18h] [ebp-38h]
	//Bit16s v17; // [esp+1Ah] [ebp-36h]
	//unsigned __int16 v18; // [esp+1Ch] [ebp-34h]
	//unsigned __int16 v19; // [esp+1Eh] [ebp-32h]
	HDIGDRIVER result; // [esp+24h] [ebp-2Ch]
	int v21; // [esp+28h] [ebp-28h]
	unsigned int v22; // [esp+2Ch] [ebp-24h]
	unsigned int v23; // [esp+30h] [ebp-20h]
	unsigned int v24; // [esp+34h] [ebp-1Ch]
	int v25; // [esp+38h] [ebp-18h]
	HDIGDRIVER v26 = NULL; // [esp+3Ch] [ebp-14h]
	unsigned int v27; // [esp+40h] [ebp-10h]
	int v28; // [esp+44h] [ebp-Ch]
	int i; // [esp+48h] [ebp-8h]
	int v30; // [esp+4Ch] [ebp-4h]
	IO_PARMS* v31; // [esp+64h] [ebp+14h]
	IO_PARMS* v32; // [esp+64h] [ebp+14h]

	//fix it
	//v17 = 0;
	//v18 = 0;
	//v19 = 0;
	v28 = 0;
	v31 = 0;
	//result = 0;
	//fix it

	sub_A1F90();//lock linear
	v26 = (_DIG_DRIVER*)sub_9D450_malloc4(sizeof(_DIG_DRIVER));
	//fixed
	v26->hw_format_6 = 0;
	v26->hw_mode_flags_7 = 0;
	//fixed
	if (v26)
	{
		v26->drvr_0 = a1;//fix it
		if (v26->drvr_0->type_5)
		{
			qmemcpy(&x_BYTE_181C90, (void*)".DIG driver required\n", 0x16u);
			sub_9D490_free4(v26, 140);
			result = 0;
		}
		else
		{
			sub_91F70_AIL_call_driver(v26->drvr_0, 769, 0, &v16);//354fa8
			//fix it! write read DDT function for next two lines
			v26->DDT_1 = (DIG_DDT*)sound_buffer[0/*v16.AX*/];//(Bit8u*)v16.AX +16 * v16.DX;//fix
			v26->DST_2 = (DIG_DST*)sound_buffer[1/*v16.BX*/];//(Bit8u*)v16.BX + 16 * v16.CX;//fix
			v26->buffer_flag_13 = (Bit16s*)v26->DST_2->active_buffer;
			v26->last_buffer_14 = -1;
			v26->playing_21 = 0;
			v26->quiet_22 = 0;
			v26->n_active_samples_25 = 0;
			v26->master_volume_26 = 127;
			memset(&unk_181D90, -1, 24);
			v28 = 0;
			if (&IO)
			{
				qmemcpy(&unk_181D90, (void*)&IO, 0x18u);
				if (sub_A2C80(v26, &IO))
				{
					v28 = 1;
					qmemcpy(v15, (void*)&IO, 0x18u);
				}
			}
			if (!v28)
			{
				v31 = sub_92ED0_AIL_get_IO_environment(v26->drvr_0);
				if (v31)
				{
					qmemcpy(&unk_181D90, v31, 0x18u);
					if (sub_A2C80(v26, v31))
					{
						v28 = 1;
						qmemcpy(v15, v31, 0x18u);
					}
				}
			}
			if (!v28 && x_DWORD_181DAC[17] == 1)
			{
				for (i = 0; (int)v26->drvr_0->VHDR_4->environment_string_16 > i; i++)//fix
				{
					v32 = (IO_PARMS*)(24 * i
						+ (v26->drvr_0->VHDR_4->environment_string_16 & 0xFFFF)//fix - 12
						+ 16 * (v26->drvr_0->VHDR_4->environment_string_16 >> 16));//fix - 12
					if (!i)
						qmemcpy(&unk_181D90, (void*)v32, 0x18u);
					if (sub_A2C80(v26, v32))
					{
						v28 = 1;
						qmemcpy(v15, (void*)v32, 0x18u);
						break;
					}
				}
			}
			if (v28)
			{
				qmemcpy(&unk_181D90, v15, 0x18u);
				if (x_DWORD_181DAC[18])
					v25 = 0x2000;
				else
					v25 = x_DWORD_181DAC[2];
				v26->DMA_buf_10 = 0;
				//do //fixed
				{
					if (!sub_9D4D0(1,//fixed - find true size DMA buffer
							//(v25 + 15 - (__CFSHL__((v25 + 15) >> 31, 4) + 16 * ((v25 + 15) >> 31))) >> 4,
						(Bit8u**)&v30, (Bit32u*)&v22, (Bit32u*)&v21))
					{
						qmemcpy(&x_BYTE_181C90, (void*)"Could not allocate DMA buffers\n", 0x20u);
						sub_9D490_free4(v26, 140);
						return 0;
					}
					if (v26->DMA_buf_10)
					{
						v2 = v26->DMA_sel_9;
						v3 = (int)v26->DMA_seg_8;
						sub_9D560((Bit8u*)v26->DMA_buf_10);
					}
					v26->DMA_buf_10 = (void*)v30;
					v26->DMA_sel_9 = v22;
					v26->DMA_seg_8 = (void*)v21;
					v27 = v22 >> 12;
					v24 = 0xF0000 & (v22 >> 12);
					v23 = 0xF0000 & (v25 + (v22 >> 12) - 1);
				}
				//while ( (0xF0000 & (v22 >> 12)) != v23 );*/ // fix allocate dma buffers
				if (sub_A2650(v26))
				{
					sub_91F70_AIL_call_driver(v26->drvr_0, 773, 0, 0);
					v26->drvr_0->initialized_6 = 1;
					v26->samples_per_buffer_18 = v26->half_buffer_size_4 / (v26->bytes_per_channel_16 * v26->channels_per_sample_15);
					v26->channels_per_buffer_17 = v26->half_buffer_size_4 / v26->bytes_per_channel_16;
					v26->build_size_19 = 4 * v26->channels_per_buffer_17;
					v6 = sub_9D450_malloc4(v26->build_size_19);
					v26->build_buffer_20 = (Bit32s*)v6;
					if (v26->build_buffer_20)
					{
						v26->n_samples_24 = x_DWORD_181DAC[4];
						v26->samples_23 = (_SAMPLE*)sub_9D450_malloc4(sizeof(_SAMPLE) * v26->n_samples_24);
						if (v26->samples_23)
						{
							for (i = 0; i < v26->n_samples_24; i++)
							{
								v26->samples_23[i].status_1 = 1;
								v26->samples_23[i].driver_0 = v26;
								v26->samples_23[i].index_sample = i;//fixed
							}
							v11 = sub_92600_AIL_register_timer(sub_A2450);
							v26->timer_3 = v11;
							if (v26->timer_3 == -1)
							{
								qmemcpy(&x_BYTE_181C90, (void*)"Out of timer handles\n", 0x16u);
								if (a1->PM_ISR_7 != -1)
									sub_91E90_AIL_restore_USE16_ISR(a1->PM_ISR_7);
								sub_91F70_AIL_call_driver(v26->drvr_0, 774, 0, 0);
								v26->n_samples_24 = 0;
								v12 = v26->DMA_sel_9;
								v13 = (int)v26->DMA_seg_8;
								sub_9D560((Bit8u*)v26->DMA_buf_10);
								sub_9D490_free4(v26->samples_23, sizeof(_SAMPLE) * v26->n_samples_24);
								sub_9D490_free4(v26->build_buffer_20, v26->build_size_19);
								sub_9D490_free4(v26, 140);
								result = 0;
							}
							else
							{
								sub_92740_AIL_set_timer_user(v26->timer_3, (signed __int32)v26);
								sub_92930_AIL_set_timer_frequency(v26->timer_3, x_DWORD_181DAC[0]);//00352DAC not zero!
								sub_92BA0_AIL_start_timer(v26->timer_3);
								v26->var36_aildrv = (x_DWORD)sub_A2DE0;
								v26->var40_aildrv = (x_DWORD)v26;
								sub_B0C1A(v26);
								//dma sub_B0B87(v26, 0);//fix
								//dma sub_B0B87(v26, 1);//fix
								result = v26;
							}
						}
						else
						{
							qmemcpy(&x_BYTE_181C90, (void*)"Could not allocate SAMPLE structures\n", 0x26u);
							if (a1->PM_ISR_7 != -1)
								sub_91E90_AIL_restore_USE16_ISR(a1->PM_ISR_7);
							sub_91F70_AIL_call_driver(v26->drvr_0, 774, 0, 0);
							v26->n_samples_24 = 0;
							v9 = v26->DMA_sel_9;
							v10 = (int)v26->DMA_seg_8;
							sub_9D560((Bit8u*)v26->DMA_buf_10);
							sub_9D490_free4(v26->samples_23, v26->build_size_19);
							sub_9D490_free4(v26, 140);
							result = 0;
						}
					}
					else
					{
						qmemcpy(&x_BYTE_181C90, (void*)"Could not allocate build buffer\n", 0x21u);
						if (a1->PM_ISR_7 != -1)
							sub_91E90_AIL_restore_USE16_ISR(a1->PM_ISR_7);
						sub_91F70_AIL_call_driver(v26->drvr_0, 774, 0, 0);
						v26->n_samples_24 = 0;
						v7 = v26->DMA_sel_9;
						v8 = (int)v26->DMA_seg_8;
						sub_9D560((Bit8u*)v26->DMA_buf_10);
						sub_9D490_free4(v26, 140);
						result = 0;
					}
				}
				else
				{
					v4 = v26->DMA_sel_9;
					v5 = (int)v26->DMA_seg_8;
					sub_9D560((Bit8u*)v26->DMA_buf_10);
					sub_9D490_free4(v26, 140);
					result = 0;
				}
			}
			else
			{
				qmemcpy(&x_BYTE_181C90, (void*)"Digital sound hardware not found\n", 0x22u);
				sub_9D490_free4(v26, 140);
				result = 0;
			}
		}
	}
	else
	{
		qmemcpy(&x_BYTE_181C90, (void*)"Could not allocate memory for driver\n", 0x26u);
		result = 0;
	}
	return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// A2450: using guessed type int sub_A2450();
// A2DE0: using guessed type int sub_A2DE0();
// 181C90: using guessed type char x_BYTE_181C90;
// 181DAC: using guessed type int x_DWORD_181DAC[];
// 181DB4: using guessed type int x_DWORD_181DB4;
// 181DBC: using guessed type int x_DWORD_181DBC;
// 181DF0: using guessed type int x_DWORD_181DF0;
// 181DF4: using guessed type int x_DWORD_181DF4;

//----- (000B0C46) --------------------------------------------------------
void sub_B0C46()
{
	sub_9D590_lock_linear_region((Bit8u*)&x_DWORD_E4E1C, (unsigned int)&x_WORD_E4E3C);
	return sub_9D590_lock_linear_region((Bit8u*)off_AEB40, (unsigned int)sub_B0C46);
}

//----- (000A1F90) --------------------------------------------------------
void sub_A1F90()//282f90
{
	//bool result; // eax

	if (!x_DWORD_E4A90)
	{
		sub_9D590_lock_linear_region((Bit8u*)sub_A1F90, (unsigned int)sub_A4260);
		sub_9D710((Bit8u*)x_BYTE_E4A0C, 128);
		sub_9D710((Bit8u*)&x_DWORD_E4A8C, 4);
		sub_9D710((Bit8u*)&x_DWORD_181E94, 4);
		sub_9D710((Bit8u*)&unk_181E80, 12);
		sub_9D710((Bit8u*)&x_DWORD_181E9C, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EA0, 4);
		sub_9D710((Bit8u*)&x_DWORD_181E90, 4);
		sub_9D710((Bit8u*)&x_DWORD_181E98, 4);
		sub_9D710((Bit8u*)&x_DWORD_181E8C, 4);
		sub_B0C46();
		x_DWORD_E4A90 = 1;
	}
	//return result;
}
// E4A8C: using guessed type int x_DWORD_E4A8C;
// E4A90: using guessed type int x_DWORD_E4A90;
// 181E8C: using guessed type int x_DWORD_181E8C;
// 181E90: using guessed type int x_DWORD_181E90;
// 181E94: using guessed type int x_DWORD_181E94;
// 181E98: using guessed type int x_DWORD_181E98;
// 181E9C: using guessed type int x_DWORD_181E9C;
// 181EA0: using guessed type int x_DWORD_181EA0;

//----- (000A2070) --------------------------------------------------------
void sub_A2070(HDIGDRIVER a1)//283070
{
	VDI_CALL v2; // [esp+0h] [ebp-Ch]
	__int16 v3; // [esp+4h] [ebp-8h]
	__int16 v4; // [esp+6h] [ebp-6h]

	/*
	cx=5622
	dx=a20002
	si=2b6f44
	di=35001a
	*/

	if (!a1->playing_21)
	{
		v4 = a1->hw_format_6;
		v3 = a1->DMA_rate_5;
		sub_91F70_AIL_call_driver(a1->drvr_0, 1025, &v2, 0);
		a1->playing_21 = 1;
	}
}

//----- (000A20D0) --------------------------------------------------------
void sub_A20D0(HDIGDRIVER a1)//2830d0
{
	if (a1->playing_21)
	{
		sub_91F70_AIL_call_driver(a1->drvr_0, 1026, 0, 0);
		a1->playing_21 = 0;
	}
}

//----- (000A2110) --------------------------------------------------------
void sub_A2110(HSAMPLE S)//283110
{
	//int result; // eax
	//int *v2; // edx
	//int *v3; // edx
	//int *v4; // edx
	//int *v5; // edx
	//int *v6; // edx
	//int *v7; // edx
	//int *v8; // edx
	//int *v9; // edx
	//int *v10; // edx
	int v11; // [esp+0h] [ebp-24h]
	int v12; // [esp+4h] [ebp-20h]
	int v13; // [esp+8h] [ebp-1Ch]
	//int v14; // [esp+Ch] [ebp-18h]
	int v15; // [esp+14h] [ebp-10h]
	int v16; // [esp+18h] [ebp-Ch]
	int v17; // [esp+1Ch] [ebp-8h]
	signed int l; // [esp+20h] [ebp-4h]
	signed int m; // [esp+20h] [ebp-4h]
	signed int n; // [esp+20h] [ebp-4h]
	signed int i; // [esp+20h] [ebp-4h]
	signed int j; // [esp+20h] [ebp-4h]
	signed int k; // [esp+20h] [ebp-4h]

	if (S->volume_16 <= 127)
	{
		if (S->volume_16 < 0)
			S->volume_16 = 0;
	}
	else
	{
		S->volume_16 = 127;
	}
	if (S->pan_17 <= 127)
	{
		if (S->pan_17 < 0)
			S->pan_17 = 0;
	}
	else
	{
		S->pan_17 = 127;
	}
	//v14 = S->sam_var17_volume_pan;
	v17 = S->sam_var[26] * S->pan_17 / 127;
	if (v17 < 0)
		v17 = 0;
	if (v17 > 127)
		v17 = 127;
	if (S->format_13 != 1 && S->format_13 != 3)
	{//set volume table
		if (v17)
			v17++;
		if (S->sam_var[6] != 2
			&& S->sam_var[6] != 3
			&& (S->sam_var[6] && S->sam_var[6] != 1 || S->format_13 != 2 && S->format_13 != 3))
		{
			//v11 = (int)&S->sam_varw18;
			v11 = 0;
			//result = (int)S;
			if (S->flags_14 & 1)
			{
				for (i = 0; i < 32768; i += 256)
				{
					//result = v17 * i >> 7;
					/*v8 = */S->vol_scale_18[0][v11] = v17 * i >> 7;
					v11++;
					//*v8 = v17 * i >> 7;
				}
				for (j = -32768; j < 0; j += 256)
				{
					//result = v17 * i >> 7;
					/*v8 = */S->vol_scale_18[0][v11] = v17 * j >> 7;
					v11++;
					//*v8 = v17 * i >> 7;
				}
			}
			else
			{
				for (k = -32768; k < 32768; k += 256)
				{
					//result = v17 * i >> 7;
					/*v8 = */S->vol_scale_18[0][v11] = v17 * k >> 7;
					v11++;
					//*v8 = v17 * i >> 7;
				}
			}
		}
		else
		{
			//v13 = (int)&S->sam_varw18;
			//v12 = (int)&S->sam_var[274];
			v13 = 0;
			v12 = 0;
			v16 = x_BYTE_E4A0C[127 - S->pan_17];
			v15 = x_BYTE_E4A0C[S->pan_17];
			//result = (int)&S;
			if (S->flags_14 & 1)
			{
				for (l = 0; l < 32768; l += 256)
				{
					S->vol_scale_18[0][v13] = v16 * (v17 * l >> 7) >> 7;
					v13++;
					/*
								  v2 = (int *)v13;
								  v13 += 4;
								  *v2 = v16 * (v17 * l >> 7) >> 7;*/
								  //result = v15 * (v17 * l >> 7) >> 7;
					S->vol_scale_18[0][v12] = v15 * (v17 * l >> 7) >> 7;
					v12++;
					/* v3 = (int *)v12;
					 v12 += 4;
					 *v3 = v15 * (v17 * l >> 7) >> 7;*/
				}
				for (m = -32768; m < 0; m += 256)
				{
					S->vol_scale_18[0][v13] = v16 * (v17 * m >> 7) >> 7;
					v13++;
					/*v4 = (int *)v13;
					v13 += 4;
					*v4 = v16 * (v17 * m >> 7) >> 7;*/
					//result = v15 * (v17 * m >> 7) >> 7;
					S->vol_scale_18[0][v12] = v15 * (v17 * m >> 7) >> 7;
					v12++;
					/*v5 = (int *)v12;
					v12 += 4;
					*v5 = v15 * (v17 * m >> 7) >> 7;*/
				}
			}
			else
			{
				for (n = -32768; n < 32768; n += 256)
				{
					S->vol_scale_18[0][v13] = v15 * (v17 * n >> 7) >> 7;
					v13++;
					/*
				  v6 = (int *)v13;
				  v13 += 4;
				  *v6 = v16 * (v17 * n >> 7) >> 7;*/
				  //result = v15 * (v17 * n >> 7) >> 7;
					S->vol_scale_18[0][v12] = v15 * (v17 * n >> 7) >> 7;;
					v12++;
					/*
				  v7 = (int *)v12;
				  v12 += 4;
				  *v7 = v15 * (v17 * n >> 7) >> 7;*/
				}
			}
		}
	}
	else
	{//set fix volume
		S->vol_scale_18[0][0] = v17 * x_BYTE_E4A0C[127 - S->pan_17] / 127;
		//result = v17 * (unsigned __int8)x_BYTE_E4A0C[v14] / 127;
		S->sam_var[274] = v17 * x_BYTE_E4A0C[S->pan_17] / 127;
	}
	//return result;
}

//----- (000A2650) --------------------------------------------------------
int sub_A2650(HDIGDRIVER a1)//283650
{
	int v1; // eax
	Bit32s v2; // eax
	unsigned int v4; // [esp+0h] [ebp-44h]
	unsigned int v5; // [esp+8h] [ebp-3Ch]
	int v6[4]; // [esp+Ch] [ebp-38h]
	//int v7; // [esp+10h] [ebp-34h]//1
	//int v8; // [esp+14h] [ebp-30h]//2
	//int v9; // [esp+18h] [ebp-2Ch]//3
	int v10; // [esp+1Ch] [ebp-28h]
	int v11; // [esp+20h] [ebp-24h]
	unsigned int v12; // [esp+24h] [ebp-20h]
	int v13; // [esp+28h] [ebp-1Ch]
	int v14; // [esp+2Ch] [ebp-18h]
	Bit32s j; // [esp+30h] [ebp-14h]
	int v16; // [esp+34h] [ebp-10h]
	int v17; // [esp+38h] [ebp-Ch]
	int v18; // [esp+3Ch] [ebp-8h]
	Bit32s i; // [esp+40h] [ebp-4h]

	//fix
	v14 = 0;
	//fix
	v11 = a1->playing_21;
	if (v11)
	{
		for (i = 0; i < a1->n_samples_24; i++)
		{
			a1->samples_23[i].sam_var[548] = a1->samples_23[i].status_1;
			if (a1->samples_23[i].status_1 == 4)
				a1->samples_23[i].status_1 = 8;
		}
		sub_A20D0(a1);
		sub_B0C1A(a1);
		//dma - sub_B0B87(a1, 0);//fix
		//dma - sub_B0B87(a1, 1);//fix
	}
	v5 = 2 * x_DWORD_181DAC[7] | x_DWORD_181DAC[8];
	if (v5 <= 3)
	{
		switch (v5)
		{
		case 0u:
			v6[0] = 0;
			v6[1] = 1;
			v6[2] = 2;
			v6[3] = 3;
			break;
		case 1u:
			v6[0] = 1;
			v6[1] = 0;
			v6[2] = 3;
			v6[3] = 2;
			break;
		case 2u:
			v6[0] = 2;
			v6[1] = 3;
			v6[2] = 0;
			v6[3] = 1;
			break;
		case 3u:
			v6[0] = 3;
			v6[1] = 2;
			v6[2] = 1;
			v6[3] = 0;
			break;
		}
	}
	for (i = 0; i < 4; i++)
	{
		if (*(x_BYTE *)(v6[i] + (Bit8u*)&a1->DDT_1))
		{
			a1->hw_format_6 = v6[i];
			break;
		}
	}
	a1->hw_mode_flags_7 = *(x_DWORD *)(14 * a1->hw_mode_flags_7 + (Bit8u*)&a1->DDT_1 + 26);
	if (x_DWORD_181DAC[1] < 1)
	{
		if (!x_DWORD_181DAC[1])
		{
			a1->DMA_rate_5 = *(unsigned __int16 *)(14 * a1->hw_format_6 + (Bit8u*)&a1->DDT_1 + 16);
			goto LABEL_33;
		}
	}
	else
	{
		if (x_DWORD_181DAC[1] == 0)
		{
			a1->DMA_rate_5 = *(unsigned __int16 *)(14 * a1->hw_format_6 + (Bit8u*)&a1->DDT_1 + 18);
			goto LABEL_33;
		}
		if (x_DWORD_181DAC[1] == 2)
		{
			a1->DMA_rate_5 = *(unsigned __int16 *)(14 * a1->hw_format_6 + (Bit8u*)&a1->DDT_1 + 20);
			goto LABEL_33;
		}
	}
	j = x_DWORD_181DAC[1];
	v6[0] = *(unsigned __int16 *)(14 * a1->hw_format_6 + (Bit8u*)&a1->DDT_1 + 16);
	v6[1] = *(unsigned __int16 *)(14 * a1->hw_format_6 + (Bit8u*)&a1->DDT_1 + 18);
	v6[2] = *(unsigned __int16 *)(14 * a1->hw_format_6 + (Bit8u*)&a1->DDT_1 + 20);
	v13 = 0x7FFFFFFF;
	for (i = 0; i < 3; i++)
	{
		v1 = abs(j - v6[i]);
		if (v1 <= v13)
		{
			v13 = abs(j - v6[i]);
			v14 = i;
		}
	}
	a1->DMA_rate_5 = v6[v14];
LABEL_33:
	v4 = a1->hw_format_6;
	if (v4 <= 3)
	{
		switch (v4)
		{
		case 0u:
			a1->channels_per_sample_15 = 1;
			a1->bytes_per_channel_16 = 1;
			break;
		case 1u:
			a1->channels_per_sample_15 = 1;
			a1->bytes_per_channel_16 = 2;
			break;
		case 2u:
			a1->channels_per_sample_15 = 2;
			a1->bytes_per_channel_16 = 1;
			break;
		case 3u:
			a1->channels_per_sample_15 = 2;
			a1->bytes_per_channel_16 = 2;
			break;
		}
	}
	v16 = x_DWORD_181DAC[3] * a1->DMA_rate_5 / 1000;
	a1->half_buffer_size_4 = a1->channels_per_sample_15 * a1->bytes_per_channel_16 * v16;
	v18 = *(unsigned __int16 *)(14 * a1->hw_format_6 + (Bit8u*)&a1->DDT_1 + 22);
	v17 = *(unsigned __int16 *)(14 * a1->hw_format_6 + (Bit8u*)&a1->DDT_1 + 24);
	if (!x_DWORD_181DAC[18])
		goto LABEL_64;
	if (v17 > 2048)
		v17 = 2048;
	/*if (v18 > 2048)
	{
		strcpy_s(x_BYTE_181C90, strlen("Minimum DMA buffer size too large for VDM\n") + 1, "Minimum DMA buffer size too large for VDM\n");
		v10 = 0;
	}
	else*/
	{
	LABEL_64:
		v13 = 0x7FFFFFFF;
		for (j = 8; x_DWORD_181DAC[2] / 2 >= j; j *= 2)//00352DB4
		{
			v2 = abs(j - a1->half_buffer_size_4);
			if (v2 <= v13)
			{
				v13 = abs(j - a1->half_buffer_size_4);
				v14 = j;
			}
		}
		//adress 283b19
		a1->half_buffer_size_4 = v14;//354f64->0000800
		if (a1->half_buffer_size_4 < v18)
			a1->half_buffer_size_4 = v18;
		if (a1->half_buffer_size_4 > v17)
			a1->half_buffer_size_4 = v17;
		v12 = (int)a1->DMA_seg_8;
		if (x_DWORD_181DAC[18])
		{
			v12 >>= 12;
			v12 += 4095;
			v12 &= 0xFF000;
			v12 <<= 12;
		}
		a1->DST_2->DMA_buffer_A = (void*)v12;
		a1->DST_2->DMA_buffer_B = (void*)(a1->half_buffer_size_4 + v12);
		//fix get from sound buffer
		//fix a1->DMA_11_12[0] = (void*)((int)a1->DST_2.DMA_buffer_A & 0xFFFF) + 16 * ((int)a1->DST_2.DMA_buffer_A >> 16);
		//fix a1->DMA_11_12[1] = (void*)((int)a1->DST_2.DMA_buffer_B & 0xFFFF) + 16 * ((int)a1->DST_2.DMA_buffer_B >> 16);
		if (v11)
		{
			for (i = 0; i < a1->n_samples_24; i++)
				a1->samples_23[i].status_1 = a1->samples_23[i].sam_var[548];
			sub_B0C1A(a1);
			//dma sub_B0B87(a1, 0);//fix
			//dma sub_B0B87(a1, 1);//fix
			sub_A2070(a1);
		}
		v10 = 1;
	}
	return v10;
}
// A0D1F: using guessed type x_DWORD abs(x_DWORD);
// 181C90: using guessed type char x_BYTE_181C90;
// 181DB0: using guessed type int x_DWORD_181DB0;
// 181DB4: using guessed type int x_DWORD_181DB4;
// 181DB8: using guessed type int x_DWORD_181DB8;
// 181DC8: using guessed type int x_DWORD_181DC8;
// 181DCC: using guessed type int x_DWORD_181DCC;
// 181DF4: using guessed type int x_DWORD_181DF4;

Bit16s envstring[0x18] = {
	220,7,01,05,-1,-1,-1,-1,
	-1,-1,-1,-1,220,05,01,05,
	-1,-1,-1,-1,-1,-1,-1,-1
};

//----- (000A2C80) --------------------------------------------------------
int sub_A2C80(HDIGDRIVER a1, IO_PARMS* a2)//283c80
{
	Bit16s v3[0x18]; // [esp+0h] [ebp-30h]
	//__int16 v4; // [esp+2h] [ebp-2Eh]
	//__int16 v5; // [esp+4h] [ebp-2Ch]
	//__int16 v6; // [esp+6h] [ebp-2Ah]
	Bit16s v7[4]; // [esp+8h] [ebp-28h]
	VDI_CALL v8; // [esp+18h] [ebp-18h]
	Bit16s v9; // [esp+1Eh] [ebp-12h]
	unsigned int i; // [esp+24h] [ebp-Ch]
	Bit16s *v11; // [esp+28h] [ebp-8h]

	//fix it
	//v4 = 0;
	//v5 = 0;
	//v6 = 0;
	//fix it

	//qmemcpy(v3, (void*)a2, 0x18u); // fix it
	if (a1->drvr_0->VHDR_4->environment_string_16)
	{
		/*
		eax = [ebp+10]//1a7884
		eax=[eax]//1a7758
		eax=[eax+10]//2c380

		eax = [ebp+10]//1a7884
		eax=[eax]//1a7758
		eax=[eax+10]//2c380
		eax=[eax+0c]>>16<<4//2c38

		eax = [ebp+10]//1a7884
		eax=[eax]//1a7758
		eax=[eax+10]//2c380//[eax+0c] je 2c38050e
		eax=[eax+0c]&&ffff//50e
		+ = 2c88e - final adress
		*/
		/*v11 = (__int16 *)((a1->var0_aildrv->AIL_DRIVER_var4_VHDR->environment_string & 0xFFFF)
						+ 16 * (a1->var0_aildrv->AIL_DRIVER_var4_VHDR->environment_string >> 16));*/
						//fixed
		v11 = envstring;//fixed
		if (v3[0] < 0)
			v3[0] = v11[0];
		if (v3[1] < 0)
			v3[1] = v11[1];
		if (v3[2] < 0)
			v3[2] = v11[2];
		if (v3[3] < 0)
			v3[3] = v11[3];
		for (i = 0; i < 4; i++)
		{
			if (v7[i] < 0)
				v7[i] = v11[2 * i + 4];
		}
	}
	qmemcpy((void *)&(a1->drvr_0->VHDR_4->IO), v3, 0x18u);
	if (x_DWORD_181DAC[9])
		v9 = 3;
	else
		v9 = 1;
	return sub_91F70_AIL_call_driver(a1->drvr_0, 772, &v8, 0);
}
// 181DD0: using guessed type int x_DWORD_181DD0;




//----- (000A36B0) --------------------------------------------------------
HDIGDRIVER sub_A36B0(int a1, Bit8u* a2, int a3, IO_PARMS a4)
{
	HDIGDRIVER result; // [esp+4h] [ebp-8h]
	AIL_DRIVER *v7; // [esp+8h] [ebp-4h]

	v7 = sub_93010_AIL_install_driver(/*a1, */a2, a3);
	if (!v7)
		return 0;
	result = sub_A2EA0(v7, a4);
	if (!result)
		sub_93160_AIL_uninstall_driver(v7);
	return result;
}



//----- (000A37C0) --------------------------------------------------------
int sub_A37C0(int a1)
{
	int v2; // [esp+4h] [ebp-8h]
	int i; // [esp+8h] [ebp-4h]

	v2 = 0;
	for (i = 0; i < *(x_DWORD *)(a1 + 96); i++)
	{
		if (*(x_DWORD *)(2196 * i + *(x_DWORD *)(a1 + 92) + 4) == 4)
			++v2;
	}
	return v2;
}

//----- (000A3820) --------------------------------------------------------
HSAMPLE sub_A3820_allocate_sample_handle(HDIGDRIVER dig)//284820
{
	HSAMPLE v1; // ST08_4
	HSAMPLE v3; // [esp+0h] [ebp-Ch]
	int i; // [esp+8h] [ebp-4h]

	sub_91BD0_s_plus();
	for (i = 0; i < dig->n_samples_24 && dig->samples_23[i].status_1 != 1; i++)
		;
	if (i == dig->n_samples_24)
	{
		qmemcpy(&x_BYTE_181C90, (void*)"Out of sample handles\n", 0x17u);
		sub_91BF0_s_minus();
		v3 = 0;
	}
	else
	{
		v1 = &dig->samples_23[i];
		sub_93830_AIL_init_sample(v1);
		sub_91BF0_s_minus();
		v3 = v1;
	}
	return v3;
}
// 181C90: using guessed type char x_BYTE_181C90;

//----- (000A38C0) --------------------------------------------------------
int sub_A38C0_AIL_API_release_sample_handle(int a1)
{
	int result; // eax

	if (a1)
	{
		result = a1;
		*(x_DWORD *)(a1 + 4) = 1;
	}
	return result;
}

//----- (000A38E0) --------------------------------------------------------
void sub_A38E0_init_sample(HSAMPLE S/*S32     format,U32     flags*/)//2848e0
{
	if (S)//fix S
	{
		S->status_1 = 2;
		S->start_2_3[0] = 0;
		S->start_2_3[1] = 0;
		S->start_44mhz = 0;
		S->len_4_5[0] = 0;
		S->len_4_5[1] = 0;
		S->pos_6_7[0] = 0;
		S->pos_6_7[1] = 0;
		S->done_8_9[0] = 0;
		S->done_8_9[1] = 1;
		S->current_buffer_10 = 0;
		S->last_buffer_11 = -2;
		S->loop_count_12 = 1;
		S->format_13 = 0;
		S->flags_14 = 0;
		S->playback_rate_15 = 11025;
		S->volume_16 = x_DWORD_181DAC[5];
		if (S->pos_6_7[0] && S->pos_6_7[0] != 1)
			S->pan_17 = 64;
		else
			S->pan_17 = 0;
		S->sam_var[530] = 0;
		S->sam_var[531] = 0;
		S->sam_var532_EOS_ptr = 0;
		sub_A2110(S);
	}
}
// 181DC0: using guessed type int x_DWORD_181DC0;

//----- (000A3A00) --------------------------------------------------------
Bit32u sub_A3A00_sample_status(HSAMPLE S)
{
	int result; // [esp+0h] [ebp-4h]

	if (S)
		result = S->status_1;
	else
		result = 0;
	return result;
}

void init_hqsound(HSAMPLE S) {
	if (S->start_44mhz)
		free(S->start_44mhz);
	S->start_44mhz = malloc(S->len_4_5[0] * 2 * 4);

	/*SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt, AUDIO_U8, 1, 11025, AUDIO_S16, 1, 44100);
	//SDL_assert(cvt.needed); // obviously, this one is always needed.
	cvt.len = S->len_4_5[0];  // 1024 stereo float32 sample frames.
	//cvt.len_mult = 2 * 4 * 2;
	cvt.buf = (Uint8 *)SDL_malloc(cvt.len * cvt.len_mult);
	memcpy(cvt.buf, S->start_2_3[0], S->len_4_5[0]);
	// read your float32 data into cvt.buf here.
	SDL_ConvertAudio(&cvt);
	memcpy(S->start_44mhz, cvt.buf, cvt.len * 2*4*2);*/
	//free(cvt.buf);
	//ffe8 ffe8 b1ec b1ec 5bf5 5bf5 b000 b000
	//69 8c a6 b4 c0 c4


	Bit16u lastval = ((Bit8u*)S->start_2_3[0])[0] * 256;
	Bit16u actval;
	Bit16s val1, val2, val3, val4;
	for (int i = 0; i < S->len_4_5[0]; i++)
	{
		actval = ((Bit8u*)S->start_2_3[0])[i] * 256;
		val1 = lastval - 0x8000;
		val2 = (lastval*0.85 + actval * 0.15) - 0x8000;
		val3 = (lastval * 0.5 + actval * 0.5) - 0x8000;
		val4 = (lastval * 0.15 + actval * 0.85) - 0x8000;
		(*(Bit16s*)&((Bit8u*)S->start_44mhz)[0 + i * 8]) = val1;
		(*(Bit16s*)&((Bit8u*)S->start_44mhz)[2 + i * 8]) = val2;
		(*(Bit16s*)&((Bit8u*)S->start_44mhz)[4 + i * 8]) = val3;
		(*(Bit16s*)&((Bit8u*)S->start_44mhz)[6 + i * 8]) = val4;
		/*(*(Bit16s*)&((Bit8u*)S->start_44mhz)[8 + i * 16]) = val3;
		(*(Bit16s*)&((Bit8u*)S->start_44mhz)[10 + i * 16]) = val3;
		(*(Bit16s*)&((Bit8u*)S->start_44mhz)[12 + i * 16]) = val4;
		(*(Bit16s*)&((Bit8u*)S->start_44mhz)[14 + i * 16]) = val4;*/
		if (i < S->len_4_5[0] + 1)lastval = actval;
	}
	lastval = actval;
};

//----- (000A3A30) --------------------------------------------------------
void sub_A3A30_set_sample_address(HSAMPLE S, Bit8u* start, Bit32u len)
{
	if (S)
	{
		S->start_2_3[0] = start;
		S->start_2_3[1] = 0;
		S->len_4_5[0] = len;
		S->len_4_5[1] = 0;
		if (hqsound)
			init_hqsound(S);
	}
}

//----- (000A3A70) --------------------------------------------------------
void sub_A3A70_set_sample_type(HSAMPLE S, Bit32s a2, Bit32u a3)//284a70
{
	if (S)
	{
		if (a2 != S->format_13 || (a3 != S->flags_14))
		{
			S->format_13 = a2;
			S->flags_14 = a3;
			sub_A2110(S);
		}
	}
}

//----- (000A3AC0) --------------------------------------------------------
Bit32s sub_A3AC0_get_sample_playback_rate(HSAMPLE S)//284ac0
{
	Bit32s result; // [esp+0h] [ebp-4h]
	if (S)
		result = S->playback_rate_15;
	else
		result = 0;
	return result;
}

//----- (000A3AF0) --------------------------------------------------------
void sub_A3AF0_set_sample_playback_rate(HSAMPLE S, Bit32s playback_rate)//284af0
{
	if (S)
	{
		S->playback_rate_15 = playback_rate;
	}
}

//----- (000A3B10) --------------------------------------------------------
Bit32s sub_A3B10_get_sample_volume(HSAMPLE S)//284b10
{
	Bit32s result; // [esp+0h] [ebp-4h]

	if (S)
		result = S->volume_16;
	else
		result = 0;
	return result;
}

//----- (000A3B40) --------------------------------------------------------
void sub_A3B40_set_sample_volume(HSAMPLE S, Bit32s volume) //284b40
{
	if (S)
	{
		//result = a2;
		if (volume != S->volume_16)
		{
			S->volume_16 = volume;
			sub_A2110(S);
		}
	}
}

//----- (000A3B80) --------------------------------------------------------
Bit32s sub_A3B80_get_sample_volume_pan(HSAMPLE S)//284b80
{
	Bit32s result; // [esp+0h] [ebp-4h]

	if (S)
		result = S->pan_17;
	else
		result = 0;
	return result;
}

//----- (000A3BB0) --------------------------------------------------------
void sub_A3BB0_set_sample_volume_pan(HSAMPLE S, Bit32s volume)//284bb0
{
	if (S)
	{
		if (volume != S->pan_17)
		{
			S->pan_17 = volume;
			sub_A2110(S);
		}
	}
}

//----- (000A3BF0) --------------------------------------------------------
int sub_A3BF0(int a1)//284bf0
{
	int v2; // [esp+0h] [ebp-4h]

	if (a1)
		v2 = *(x_DWORD *)(a1 + 4 * *(x_DWORD *)(a1 + 40) + 24);
	else
		v2 = 0;
	return v2;
}

//----- (000A3C30) --------------------------------------------------------
int sub_A3C30(int a1, int a2)//284c30
{
	int result; // eax

	if (a1)
	{
		result = a2;
		*(x_DWORD *)(4 * *(x_DWORD *)(a1 + 40) + a1 + 24) = a2;
	}
	return result;
}

//----- (000A3C60) --------------------------------------------------------
signed int sub_A3C60(int a1)//284c60
{
	signed int v2; // [esp+0h] [ebp-4h]

	if (a1)
		v2 = *(x_DWORD *)(a1 + 48);
	else
		v2 = -1;
	return v2;
}

//----- (000A3C90) --------------------------------------------------------
void sub_A3C90_set_sample_loop_count(HSAMPLE S, int loop_count)//284c90
{
	if (S)
	{
		S->loop_count_12 = loop_count;
	}
}

void sub_A3CB0_API_AIL_start_sample(HSAMPLE S)//284cb0
{
	if (S)
	{
		if (S->status_1 != 1)
		{
			S->status_1 = 4;
			SOUND_start_sample(S);
		}
	}
}
/*
//----- (000A3CB0) --------------------------------------------------------
void sub_A3CB0_API_AIL_start_sample_orig(HSAMPLE S)//284cb0
{
	if (S)
	{
		if (S->status_1 != 1)
		{
			//if ( *(x_DWORD *)((int)&S->sam_var[S->sam_var[10]] + 16) )
			if (S->sam_var[S->EOS_10 + 4])
			{
				//if ( *(x_DWORD *)((int)&S->sam_var[S->sam_var[10]] + 8) )
				if (S->sam_var[S->EOS_10 + 2])
				{
					S->sam_var[S->EOS_10 + 6] = 0;
					S->status_1 = 4;
					sub_A2070(S->driver_0);
				}
			}
		}
	}
}
*/
//----- (000A3D30) --------------------------------------------------------
void sub_A3D30(HSAMPLE S)//284d30
{
	if (S)
	{
		if (S->status_1 == 4)
		{
			S->status_1 = 8;
		}
	}
}

//----- (000A3D60) --------------------------------------------------------
void sub_A3D60(HSAMPLE S)//284d60
{
	if (S)
	{
		if (S->status_1 == 8)
		{
			S->status_1 = 4;
			sub_A2070(S->driver_0);
		}
	}
}

void sub_A3DA0__API_AIL_end_sample(HSAMPLE S)//284da0
{
	if (S)
	{
		if (S->status_1 != 1)
		{
			if (S->status_1 != 2)
			{
				S->status_1 = 2;
				SOUND_end_sample(S);
			}
		}
	}
}

//----- (000A3DA0) --------------------------------------------------------
void sub_A3DA0__API_AIL_end_sample_orig(HSAMPLE S)//284da0
{
	if (S)
	{
		//result = a1;
		if (S->status_1 != 1)
		{
			//result = a1;
			if (S->status_1 != 2)
			{
				S->status_1 = 2;
				if (S->sam_var[531])
					switch (S->sam_var[531]) {
					case 0: {
						//call(S);
						break;
					}
							//(*(void(**)(int))(S + 2124))(S);
					}
				if (S->sam_var532_EOS_ptr)
					S->sam_var532_EOS_ptr();
				/*switch (S->sam_var532_EOS_ptr) {
				case 0: {
					//call(S);
					break;
				}
						//(*(void(**)(int))(S + 2124))(S);
				}*/

				//result = a1;
				//if ( *(x_DWORD *)(a1 + 2128) )
				//  result = (*(int (**)(int))(a1 + 2128))(a1);
			}
		}
	}
}

//----- (000A3E10) --------------------------------------------------------
int sub_A3E10(HSAMPLE S, int a2)//284e10
{
	int v2; // ST04_4

	if (!S)
		return 0;
	v2 = S->sam_var[530];
	S->sam_var[530] = a2;
	return v2;
}

//----- (000A3E60) --------------------------------------------------------
int sub_A3E60(HSAMPLE S, int a2)//284e60
{
	int v2; // ST04_4

	if (!S)
		return 0;
	v2 = S->sam_var[531];
	S->sam_var[531] = a2;
	return v2;
}

//----- (000A3EB0) --------------------------------------------------------
EOS_callback_type sub_A3EB0_register_EOS_callback(HSAMPLE S, EOS_callback_type EOS)//284eb0
{
	EOS_callback_type result; // ST04_4
	if (!S)
		return 0;
	result = S->sam_var532_EOS_ptr;
	S->sam_var532_EOS_ptr = EOS;
	return result;
}

//----- (000A3F00) --------------------------------------------------------
void sub_A3F00_set_sample_user_data(HSAMPLE S, int index, int a3)//284f00
{
	//int result; // eax

	if (S)
	{
		//result = a3;
		//*(x_DWORD *)(4 * a2 + S + 2132) = a3;
		S->sam_var[533 + index];
	}
	//return result;
}

//----- (000A3F30) --------------------------------------------------------
int sub_A3F30_sample_user_data(HSAMPLE S, int index)//284f30
{
	int v3; // [esp+0h] [ebp-4h]

	if (S)
		v3 = S->sam_var[533 + index];
	else
		v3 = 0;
	return v3;
}

//----- (000A3F70) --------------------------------------------------------
void sub_A3F70_set_digital_master_volume(x_DWORD *a1, int a2)//284f70
{
	int i; // [esp+0h] [ebp-4h]

	a1[26] = a2;
	for (i = 0; ; i++)
	{
		if (i >= a1[24])
			break;
		if (*(x_DWORD *)(2196 * i + a1[23] + 4) != 1)
			sub_A2110((HSAMPLE)(x_DWORD *)(2196 * i + a1[23]));
	}
}


//----- (000A3FE0) --------------------------------------------------------
int sub_A3FE0(int a1)//284fe0
{
	return *(x_DWORD *)(a1 + 104);
}

//----- (000A4000) --------------------------------------------------------
char sub_A4000(x_DWORD *a1, int a2, unsigned int a3)//285000
{
	int v3; // ST04_4
	signed int v5; // [esp+Ch] [ebp-4h]

	if (a3 <= 3)
	{
		switch (a3)
		{
		case 0u:
			v5 = 1;
			break;
		case 1u:
			v5 = 2;
			break;
		case 2u:
			v5 = 2;
			break;
		case 3u:
			v5 = 4;
			break;
		}
	}
	v3 = a2 * (v5 * a1[4] / (a1[16] * a1[15])) / a1[5];
	if (a1[5] != a2 && 2 * a2 != a1[5])
		a1[5];
	return 0;
}

//----- (000A4100) --------------------------------------------------------
void sub_A4100(int *a1, int a2, int a3, int a4)//285100
{
	if (a1)
	{
		a1[a2 + 8] = a4 == 0;
		a1[a2 + 2] = a3;
		a1[a2 + 4] = a4;
		*(x_DWORD *)(&a1[a2] + 24) = 0;
		if (a4)
		{
			if (a1[1] != 4)
			{
				a1[1] = 4;
				sub_A2070((HDIGDRIVER)*a1);
			}
		}
	}
}

//----- (000A41B0) --------------------------------------------------------
signed int sub_A41B0(x_DWORD *a1)//2851b0
{
	int v1; // eax
	unsigned int v3; // [esp+0h] [ebp-8h]

	if (!a1)
		return -1;
	v3 = a1[11];
	if (v3 >= 0xFFFFFFFE)
	{
		if (v3 <= 0xFFFFFFFE)
		{
			a1[9] = 0;
			a1[11] = -1;
			return 0;
		}
		if (v3 == -1)
		{
			a1[11] = a1[10];
			return 1;
		}
	}
	if (a1[11] == a1[10])
		return -1;
	a1[11] = a1[10];
	v1 = a1[10];
	LOBYTE(v1) = v1 ^ 1;
	return v1;
}

//----- (000A4260) --------------------------------------------------------
bool sub_A4260()//285260
{
	bool result; // eax

	if (x_DWORD_E4A90)
	{
		sub_9D650((unsigned int)sub_A1F90, (unsigned int)sub_A4260);
		sub_9D740((unsigned int)x_BYTE_E4A0C, 128);
		sub_9D740((unsigned int)&x_DWORD_E4A8C, 4);
		sub_9D740((unsigned int)&x_DWORD_181E94, 4);
		sub_9D740((unsigned int)&unk_181E80, 12);
		sub_9D740((unsigned int)&x_DWORD_181E9C, 4);
		sub_9D740((unsigned int)&x_DWORD_181EA0, 4);
		sub_9D740((unsigned int)&x_DWORD_181E90, 4);
		sub_9D740((unsigned int)&x_DWORD_181E98, 4);
		result = sub_9D740((unsigned int)&x_DWORD_181E8C, 4);
		x_DWORD_E4A90 = 0;
	}
	return result;
}
// E4A8C: using guessed type int x_DWORD_E4A8C;
// E4A90: using guessed type int x_DWORD_E4A90;
// 181E8C: using guessed type int x_DWORD_181E8C;
// 181E90: using guessed type int x_DWORD_181E90;
// 181E94: using guessed type int x_DWORD_181E94;
// 181E98: using guessed type int x_DWORD_181E98;
// 181E9C: using guessed type int x_DWORD_181E9C;
// 181EA0: using guessed type int x_DWORD_181EA0;

//----- (000A4330) --------------------------------------------------------
void sub_A4330()//285330
{
	//bool result; // eax

	if (!x_DWORD_E4A94)
	{
		sub_9D590_lock_linear_region((Bit8u*)sub_A4330, (unsigned int)sub_A4CB0);
		x_DWORD_E4A94 = 1;
	}
	//return result;
}
// E4A94: using guessed type int x_DWORD_E4A94;

//----- (000A4370) --------------------------------------------------------
int sub_A4370(x_DWORD *a1)//285370
{
	return *a1 >> 8;
}

//----- (000A4390) --------------------------------------------------------
int sub_A4390(int a1)//285390
{
	int result; // eax

	if (*(x_DWORD *)(a1 + 2164))
		(*(void(**)(int))(a1 + 2164))(a1);
	if (*(x_DWORD *)(a1 + 2188) > 0)
		sub_937A0_AIL_release_sample_handle(a1);
	result = a1;
	*(x_DWORD *)(a1 + 2188) = -1;
	return result;
}

//----- (000A43E0) --------------------------------------------------------
void sub_A43E0(HSAMPLE S)//2853e0
{
	x_DWORD *v1; // [esp+4h] [ebp-8h]

	while (1)
	{
		v1 = *(x_DWORD **)(S + 2168);
		if (*(x_BYTE *)v1 <= 9u)
			break;
		*(x_DWORD *)(S + 2168) = (x_DWORD)(char *)v1 + sub_A4370(v1) + 4;
	}
	//JUMPOUT(__CS__, (int*) *(&off_A4400 + *(unsigned __int8 *)v1));
	//fix it!!!
}
// A4400: using guessed type void *off_A4400;

//----- (000A47C0) --------------------------------------------------------
void sub_A47C0_sub_set_sample_file(Bit8u* file_image, HSAMPLE S)//2857c0
{
	int i; // eax
	int j; // eax
	Bit8u* v5; // [esp+0h] [ebp-8h]
	Bit8u* v6; // [esp+4h] [ebp-4h]

	v6 = file_image + 12;
	for (i = _strnicmp((const char*)(file_image + 12), "fmt ", 4); i; i = _strnicmp((const char*)v6, "fmt ", 4))
		v6 += (*(x_DWORD *)(v6 + 4) & 1) + *(x_DWORD *)(v6 + 4) + 8;
	if (*(x_WORD *)(v6 + 10) == 1 && *(x_WORD *)(v6 + 22) == 8)
	{
		sub_93AB0_AIL_set_sample_type(S, 0, 0);
	}
	else if (*(x_WORD *)(v6 + 10) == 2 && *(x_WORD *)(v6 + 22) == 8)
	{
		sub_93AB0_AIL_set_sample_type(S, 2, 0);
	}
	else if (*(x_WORD *)(v6 + 10) == 1 && *(x_WORD *)(v6 + 22) == 16)
	{
		sub_93AB0_AIL_set_sample_type(S, 1, 1);
	}
	else if (*(x_WORD *)(v6 + 10) == 2 && *(x_WORD *)(v6 + 22) == 16)
	{
		sub_93AB0_AIL_set_sample_type(S, 3, 1);
	}
	sub_93D90_AIL_set_sample_playback_rate(S, *(x_DWORD *)(v6 + 12));
	v5 = file_image + 12;
	for (j = _strnicmp((const char*)(file_image + 12), "data", 4); j; j = _strnicmp((const char*)v5, "data", 4))
		v5 += (*(x_DWORD *)(v5 + 4) & 1) + *(x_DWORD *)(v5 + 4) + 8;
	sub_93A10_AIL_set_sample_address(S, v5 + 8, *(x_DWORD *)(v5 + 4));
}
// 99B23: using guessed type x_DWORD strnicmp(x_DWORD, x_DWORD, x_DWORD);

//----- (000A4970) --------------------------------------------------------
HSAMPLE sub_A4970(int a1, Bit8u* a2, int a3)//285970
{
	signed int v5; // [esp+8h] [ebp-8h]
	HSAMPLE v6; // [esp+Ch] [ebp-4h]

	sub_A4330();
	if (!_strnicmp((const char*)a2, "Creative", 8))
	{
		v5 = 0;
	}
	else
	{
		if (_strnicmp((const char*)(a2 + 8), "WAVE", 4))
		{
			qmemcpy(&x_BYTE_181C90, (void*)"Unrecognized digital audio file type\n", 0x26u);
			return 0;
		}
		v5 = 1;
	}
	v6 = sub_93510_AIL_allocate_sample_handle((HDIGDRIVER)a1);
	if (!v6)
		return 0;
	sub_93830_AIL_init_sample(v6);
	v6->sam_var[541] = 0;
	if (v5)
	{
		if (v5 == 1)
		{
			v6->sam_var[547] = 1;
			sub_95140_AIL_register_EOS_callback(v6, sub_A4920);
			sub_A47C0_sub_set_sample_file(a2, v6);
		}
	}
	else
	{
		v6->sam_var[542] = *(unsigned __int16 *)(a2 + 20) + (int)a2;
		v6->sam_var[545] = a3;
		v6->sam_var[546] = a3 == -1;
		v6->sam_var[547] = 1;
		sub_95140_AIL_register_EOS_callback(v6, sub_A47A0);
		sub_A43E0(v6);
	}
	if (v6->sam_var[547] != -1)
		return v6;
	strcpy(x_BYTE_181C90, strlen("Invalid or missing data block\n") + 1, "Invalid or missing data block\n");

	return 0;
}
// 99B23: using guessed type x_DWORD strnicmp(x_DWORD, x_DWORD, x_DWORD);
// A47A0: using guessed type int sub_A47A0();
// A4920: using guessed type int sub_A4920();
// 181C90: using guessed type char x_BYTE_181C90;

//----- (000A4B20) --------------------------------------------------------
Bit32s sub_A4B20_set_sample_file(HSAMPLE S, Bit8u* file_image, Bit32s block)//285b20
{
	signed int v5; // [esp+8h] [ebp-4h]
	if (!_strnicmp((const char*)file_image, "Creative", 8))
	{
		v5 = 0;
	}
	else
	{
		if (_strnicmp((const char*)(file_image + 8), "WAVE", 4))
		{
			qmemcpy(&x_BYTE_181C90, (void*)"Unrecognized digital audio file type\n", 0x26u);
			return 0;
		}
		v5 = 1;
	}
	if (v5)
	{
		if (v5 == 1)
		{
			S->sam_var[547] = 0;
			sub_A47C0_sub_set_sample_file(file_image, S);
		}
	}
	else
	{
		S->sam_var542 = (Bit8u*)(*(unsigned __int16 *)(file_image + 20) + file_image);
		S->sam_var[545] = block;
		S->sam_var[546] = block == -1;
		S->sam_var[547] = 0;
		sub_A43E0(S);
	}
	if (S->sam_var[547] != -1)
		return 1;
	strcpy(x_BYTE_181C90, strlen("Invalid or missing data block\n") + 1, "Invalid or missing data block\n");
	return 0;
}
// 99B23: using guessed type x_DWORD strnicmp(x_DWORD, x_DWORD, x_DWORD);
// 181C90: using guessed type char x_BYTE_181C90;

//----- (000A4C60) --------------------------------------------------------
int sub_A4C60(int a1, int a2)
{
	int v2; // ST04_4

	if (!a1)
		return 0;
	v2 = *(x_DWORD *)(a1 + 2164);
	*(x_DWORD *)(a1 + 2164) = a2;
	return v2;
}

//----- (000A4CB0) --------------------------------------------------------
bool sub_A4CB0()
{
	bool result; // eax

	if (x_DWORD_E4A94)
	{
		result = sub_9D650((unsigned int)sub_A4330, (unsigned int)sub_A4CB0);
		x_DWORD_E4A94 = 0;
	}
	return result;
}
// E4A94: using guessed type int x_DWORD_E4A94;

//----- (000A4CF0) --------------------------------------------------------
void sub_A4CF0()
{
	//bool result; // eax

	if (!x_DWORD_E4B1C)
	{
		sub_9D590_lock_linear_region((Bit8u*)sub_A4CF0, (unsigned int)sub_A9280);
		sub_9D710((Bit8u*)aSample, 128);
		sub_9D710((Bit8u*)&x_DWORD_E4B18, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EB8, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EBC, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EC0, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EC4, 4);
		sub_9D710((Bit8u*)&x_DWORD_181ED4, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EC8, 4);
		sub_9D710((Bit8u*)&x_DWORD_181ECC, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EB4, 4);
		sub_9D710((Bit8u*)&x_DWORD_181ED0, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EDC, 4);
		sub_9D710((Bit8u*)&x_DWORD_181ED8, 4);
		sub_9D710((Bit8u*)&x_DWORD_181EB0, 4);
		x_DWORD_E4B1C = 1;
	}
	//return result;
}
// E4B18: using guessed type int x_DWORD_E4B18;
// E4B1C: using guessed type int x_DWORD_E4B1C;
// 181EB0: using guessed type int x_DWORD_181EB0;
// 181EB4: using guessed type int x_DWORD_181EB4;
// 181EB8: using guessed type int x_DWORD_181EB8;
// 181EBC: using guessed type int x_DWORD_181EBC;
// 181EC0: using guessed type int x_DWORD_181EC0;
// 181EC4: using guessed type int x_DWORD_181EC4;
// 181EC8: using guessed type int x_DWORD_181EC8;
// 181ECC: using guessed type int x_DWORD_181ECC;
// 181ED0: using guessed type int x_DWORD_181ED0;
// 181ED4: using guessed type int x_DWORD_181ED4;
// 181ED8: using guessed type int x_DWORD_181ED8;
// 181EDC: using guessed type int x_DWORD_181EDC;

//----- (000A4E10) --------------------------------------------------------
signed int sub_A4E10(char a1)
{
	unsigned int v2; // [esp+0h] [ebp-8h]

	v2 = a1 & 0xF0;
	if (v2 < 0xB0)
	{
		if (v2 < 0x90)
		{
			if (v2 != 128)
				return 0;
		}
		else if (v2 > 0x90 && v2 != 160)
		{
			return 0;
		}
		return 3;
	}
	if (v2 > 0xB0)
	{
		if (v2 < 0xD0)
		{
			if (v2 != 192)
				return 0;
		}
		else if (v2 > 0xD0)
		{
			if (v2 != 224)
				return 0;
			return 3;
		}
		return 2;
	}
	return 3;
}

//----- (000A4EB0) --------------------------------------------------------
void sub_A4EB0(HMDIDRIVER a1)
{
	//HMDIDRIVER result; // eax
	VDI_CALL v2; // [esp+0h] [ebp-Ch]
	__int16 v3; // [esp+4h] [ebp-8h]

	//result = a1;
	if (a1->var106_aildrv > 0)
	{
		v3 = a1->var106_aildrv;
		sub_91F70_AIL_call_driver(a1->drvr_0, 1282, &v2, 0);
		a1->var106_aildrv = 0;
		//result = a1;
		a1->var107_aildrv = 0;
	}
	//return result;
}

//----- (000A4F10) --------------------------------------------------------
void sub_A4F10(HMDIDRIVER a1, char a2, char a3, char a4)
{
	Bit8u* v4; // ebx
	int v5; // edx
	Bit8u* v6; // ebx
	int v7; // edx
	Bit8u* v8; // ebx
	int v9; // edx
	//HMDIDRIVER result; // eax
	signed int v11; // [esp+0h] [ebp-4h]

	v11 = sub_A4E10(a2);
	if (v11 + a1->var107_aildrv > 0x200)
		sub_A4EB0(a1);
	v4 = (Bit8u*)a1->DST_2;
	v5 = a1->var107_aildrv++;
	v4[v5 + 256] = a2;
	v6 = (Bit8u*)a1->DST_2;
	v7 = a1->var107_aildrv++;
	v6[v7 + 256] = a3;
	if (v11 == 3)
	{
		v8 = (Bit8u*)a1->DST_2;
		v9 = a1->var107_aildrv++;
		v8[v9 + 256] = a4;
	}
	//result = a1;
	a1->var106_aildrv++;
	//return result;
}

//----- (000A4FD0) --------------------------------------------------------
void sub_A4FD0(int a1, int a2, unsigned int a3)
{
	sub_A4EB0((HMDIDRIVER)a1);
	if (a3 <= 0x200)
		;//fix it:memmove(*(x_DWORD *)(a1 + 8) + 256, a2, a3);
	else
		//fix it:memmove(*(x_DWORD *)(a1 + 8) + 256, a2, 512);
		++*(x_DWORD *)(a1 + 424);
	sub_A4EB0((HMDIDRIVER)a1);
}
// AD81F: using guessed type x_DWORD memmove(x_DWORD, x_DWORD, x_DWORD);

//----- (000A5040) --------------------------------------------------------
int sub_A5040(x_DWORD *a1)
{
	char v1; // ST08_1
	signed int v3; // [esp+4h] [ebp-Ch]
	int v4; // [esp+Ch] [ebp-4h]

	v4 = 0;
	v3 = 4;
	do
	{
		v1 = *(x_BYTE *)(*a1)++;
		v4 = v1 & 0x7F | (v4 << 7);
		if (!(v1 & 0x80))
			break;
		--v3;
	} while (v3);
	return v4;
}

//----- (000A50A0) --------------------------------------------------------
unsigned int sub_A50A0(int a1)
{
	return ((a1 & 0xFF000000) >> 24)
		+ ((a1 & 0xFF0000u) >> 8)
		+ ((unsigned __int16)(a1 & 0xFF00) << 8)
		+ ((unsigned __int8)a1 << 24);
}

//----- (000A50F0) --------------------------------------------------------
Bit8u* sub_A50F0(Bit8u* a1, int a2)
{
	Bit8u* v3; // [esp+0h] [ebp-Ch]
	Bit8u* v4; // [esp+4h] [ebp-8h]
	unsigned int v5; // [esp+8h] [ebp-4h]
	Bit8u* i; // [esp+1Ch] [ebp+10h]

	v5 = 0;
	do
	{
		a1 += v5;
		if (strncmp((const char*)a1, "FORM", 4) && strncmp((const char*)a1, "CAT ", 4))
			return 0;
		v5 = sub_A50A0(*(x_DWORD *)(a1 + 4)) + 8;
	} while (strncmp((const char*)(a1 + 8), "XMID", 4));
	if (!strncmp((const char*)a1, "FORM", 4))
	{
		if (a2)
			v3 = 0;
		else
			v3 = a1;
	}
	else
	{
		v4 = v5 + a1;
		for (i = a1 + 12; i < v4; i += sub_A50A0(*(x_DWORD *)(i + 4)) + 8)
		{
			if (!strncmp((const char*)(i + 8), "XMID", 4) && --a2 == -1)
				return i;
		}
		v3 = 0;
	}
	return v3;
}
// 99682: using guessed type x_DWORD strncmp(x_DWORD, x_DWORD, x_DWORD);

//----- (000A5210) --------------------------------------------------------
int sub_A5210(int a1, char a2, unsigned int a3)
{
	int v4; // [esp+0h] [ebp-Ch]
	int v5; // [esp+4h] [ebp-8h]
	unsigned int v6; // [esp+8h] [ebp-4h]

	v6 = a2 & 0xF0;
	v5 = a2 & 0xF;
	if (v6 < 0xC0)
	{
		if (v6 != 176)
			return -1;
		if (a3 < 0x5D)
		{
			if (a3 < 0xA)
			{
				if (a3 < 6)
				{
					if (a3 == 1)
						return *(x_DWORD *)(a1 + 4 * v5 + 640);
				}
				else
				{
					if (a3 <= 6)
						return *(x_DWORD *)(a1 + 4 * v5 + 1088);
					if (a3 == 7)
						return *(x_DWORD *)(a1 + 4 * v5 + 704);
				}
			}
			else
			{
				if (a3 <= 0xA)
					return *(x_DWORD *)(a1 + 4 * v5 + 768);
				if (a3 < 0x40)
				{
					if (a3 == 11)
						return *(x_DWORD *)(a1 + 4 * v5 + 832);
				}
				else
				{
					if (a3 <= 0x40)
						return *(x_DWORD *)(a1 + 4 * v5 + 896);
					if (a3 == 91)
						return *(x_DWORD *)(a1 + 4 * v5 + 960);
				}
			}
		}
		else
		{
			if (a3 <= 0x5D)
				return *(x_DWORD *)(a1 + 4 * v5 + 1024);
			if (a3 < 0x70)
			{
				if (a3 >= 0x6E)
				{
					if (a3 <= 0x6E)
						v4 = *(x_DWORD *)(a1 + 4 * v5 + 192);
					else
						v4 = *(x_DWORD *)(a1 + 4 * v5 + 256);
					return v4;
				}
				if (a3 == 107)
					return *(x_DWORD *)(a1 + 4 * v5 + 320);
			}
			else
			{
				if (a3 <= 0x70)
					return *(x_DWORD *)(a1 + 4 * v5 + 384);
				if (a3 < 0x73)
				{
					if (a3 == 114)
						return *(x_DWORD *)(a1 + 4 * v5 + 448);
				}
				else
				{
					if (a3 <= 0x73)
						return *(x_DWORD *)(a1 + 4 * v5 + 512);
					if (a3 == 119)
						return *(x_DWORD *)(a1 + 4 * v5 + 576);
				}
			}
		}
		return -1;
	}
	if (v6 <= 0xC0)
		return *(x_DWORD *)(a1 + 4 * v5);
	if (v6 != 224)
		return -1;
	return (*(x_DWORD *)(a1 + 4 * v5 + 128) << 7) | *(x_DWORD *)(a1 + 4 * v5 + 64);
}

//----- (000A5530) --------------------------------------------------------
x_DWORD *sub_A5530(int a1, char a2, unsigned int a3, unsigned __int8 a4)
{
	x_DWORD *result; // eax
	int v5; // [esp+0h] [ebp-8h]
	unsigned int v6; // [esp+4h] [ebp-4h]

	v6 = a2 & 0xF0;
	result = (x_DWORD *)(a2 & 0xF);
	v5 = a2 & 0xF;
	if (v6 < 0xC0)
	{
		if (v6 == 176)
		{
			if (a3 < 0x5D)
			{
				if (a3 < 0xA)
				{
					if (a3 < 6)
					{
						if (a3 == 1)
						{
							result = (x_DWORD *)(a1 + 4 * v5);
							result[160] = a4;
						}
					}
					else if (a3 <= 6)
					{
						result = (x_DWORD *)(a1 + 4 * v5);
						result[272] = a4;
					}
					else if (a3 == 7)
					{
						result = (x_DWORD *)(a1 + 4 * v5);
						result[176] = a4;
					}
				}
				else if (a3 <= 0xA)
				{
					result = (x_DWORD *)(a1 + 4 * v5);
					result[192] = a4;
				}
				else if (a3 < 0x40)
				{
					if (a3 == 11)
					{
						result = (x_DWORD *)(a1 + 4 * v5);
						result[208] = a4;
					}
				}
				else if (a3 <= 0x40)
				{
					result = (x_DWORD *)(a1 + 4 * v5);
					result[224] = a4;
				}
				else if (a3 == 91)
				{
					result = (x_DWORD *)(a1 + 4 * v5);
					result[240] = a4;
				}
			}
			else if (a3 <= 0x5D)
			{
				result = (x_DWORD *)(a1 + 4 * v5);
				result[256] = a4;
			}
			else if (a3 < 0x70)
			{
				if (a3 < 0x6E)
				{
					if (a3 == 107)
					{
						result = (x_DWORD *)(a1 + 4 * v5);
						result[80] = a4;
					}
				}
				else if (a3 <= 0x6E)
				{
					result = (x_DWORD *)(a1 + 4 * v5);
					result[48] = a4;
				}
				else
				{
					result = (x_DWORD *)(a1 + 4 * v5);
					result[64] = a4;
				}
			}
			else if (a3 <= 0x70)
			{
				result = (x_DWORD *)(a1 + 4 * v5);
				result[96] = a4;
			}
			else if (a3 < 0x73)
			{
				if (a3 == 114)
				{
					result = (x_DWORD *)(a1 + 4 * v5);
					result[112] = a4;
				}
			}
			else if (a3 <= 0x73)
			{
				result = (x_DWORD *)(a1 + 4 * v5);
				result[128] = a4;
			}
			else if (a3 == 119)
			{
				result = (x_DWORD *)(a1 + 4 * v5);
				result[144] = a4;
			}
		}
	}
	else if (v6 <= 0xC0)
	{
		result = (x_DWORD *)(a1 + 4 * v5);
		*result = (unsigned __int8)a3;
	}
	else if (v6 == 224)
	{
		*(x_DWORD *)(a1 + 4 * v5 + 64) = (unsigned __int8)a3;
		result = (x_DWORD *)(a1 + 4 * v5);
		result[32] = a4;
	}
	return result;
}

//----- (000A5850) --------------------------------------------------------
void sub_A5850(HSEQUENCE S, char a2, unsigned int a3, signed int a4, int a5)//286850
{
	HMDIDRIVER result; // eax
	int v6; // eax
	HMDIDRIVER v7; // [esp+0h] [ebp-14h]
	int v8; // [esp+4h] [ebp-10h]
	int v9; // [esp+8h] [ebp-Ch]
	signed int i; // [esp+Ch] [ebp-8h]
	signed int j; // [esp+Ch] [ebp-8h]
	x_DWORD *v12; // [esp+Ch] [ebp-8h]
	int v13; // [esp+10h] [ebp-4h]

	v7 = S->driver_0;
	v13 = a2 & 0xF0;
	v8 = a2 & 0xF;
	//result = S->chan_map_37[4 * v8];
	v9 = S->chan_map_37[4 * v8];
	if (v13 == 176 || v13 == 192 || v13 == 224)
		result = (HMDIDRIVER)sub_A5530((int)&S->shadow_53, v8 | v13, a3, a4);
	if (v13 != 176)
		goto LABEL_79;
	if (a5)
	{
		//result = (x_DWORD *)(S + 4 * v8);
		if (S->shadow_53.indirect[4 * v8] != -1)
		{
			a4 = S->shadow_53.indirect[4 * v8];
			//result = (x_DWORD *)(S + 4 * v8);
			S->shadow_53.indirect[4 * v8] = -1;
		}
	}
	if (a3 < 0x6F)
	{
		if (a3 < 0x6C)
		{
			if (a3 >= 6)
			{
				if (a3 <= 6)
				{
					sub_A5850(S, v8 | 0xB0, 0x64u, 0, 0);
					sub_A5850(S, v8 | 0xB0, 0x65u, 0, 0);
					sub_A5850(S, v8 | 0xB0, 0x26u, 0, 0);
				}
				else if (a3 == 7)
				{
					a4 = S->volume_14 * a4 * v7->var108_aildrv / 16129;
					if (a4 > 127)
						a4 = 127;
					if (a4 < 0)
						a4 = 0;
				}
			}
			goto LABEL_79;
		}
		if (a3 <= 0x6C)
		{
			if (S->prefix_callback_7)
				S->shadow_53.indirect[4 * v8] = (*(int(**)(HSEQUENCE, int, signed int))(S->prefix_callback_7))(S, v8, a4);
			goto LABEL_79;
		}
		if (a3 <= 0x6D)
		{
			/*result = */sub_97480_AIL_branch_index(S, a4);
		}
		else if (a4 < 64)
		{
			//result = &v7[v9];
			if (v7->lock[v9] == 1)
			{
				sub_A5F30((int*)S, v8);
				sub_980D0_AIL_release_channel((int32*)v7, v9 + 1);
				/*result = (x_DWORD *)*/sub_98170_AIL_map_sequence_channel((int32*)S, v8 + 1, v8 + 1);
			}
		}
		else
		{
			//result = &v7[v9];
			if (v7->lock[v9] != 1)
			{
				result = (HMDIDRIVER)(x_DWORD *)sub_97F90_AIL_lock_channel((int32*)v7);
				v12 = (int32*)result;
				if (result)
				{
					sub_98170_AIL_map_sequence_channel((int32*)S, v8 + 1, (int)result);
					result = (HMDIDRIVER)(x_DWORD *)S;
					v7->var23_aildrvx[(int)v12] = S;
				}
			}
		}
	}
	else if (a3 <= 0x6F)
	{
		//result = &v7[v9];
		if (v7->lock[v9] != 1)
		{
			if (a4 >= 64)
			{
				//result = &v7[v9];
				v7->lock[v9] = 2;
			}
			else
			{
				//result = &v7[v9];
				v7->lock[v9] = 0;
			}
		}
	}
	else if (a3 < 0x75)
	{
		if (a3 < 0x73)
			goto LABEL_79;
		if (a3 <= 0x73)
		{
			*(x_DWORD *)(S + 4 * v8 + 724) = *(unsigned __int8 *)(a4 + *(x_DWORD *)(S + 24));
		LABEL_79:
			if (v7->lock[v9] != 1 || v7->locker[v9] == S)
			{
				if (v13 == 144)
				{
					v7->notes[v9]++;
				}
				else if (v13 == 128)
				{
					v7->notes[v9]--;
				}
				v7->user[v9] = S;
				if (v13 != 144 || S->shadow_53.c_mute[4 * v8] < 64)
				{
					if (!v7->event_trap
						|| (result = (HMDIDRIVER)(x_DWORD *)((int(*)(HMDIDRIVER, HSEQUENCE, int, unsigned int, signed int))v7->event_trap)(
							v7,
							S,
							v9 | v13,
							a3,
							(int)a4)) == 0)
					{
						/*result = (x_DWORD *)*/sub_A4F10(v7, v9 | v13, a3, a4);
					}
				}
			}
			return;
		}
		for (i = 0; i < 4; i++)
		{
			//result = (x_DWORD *)(S + 4 * i);
			if (S->FOR_loop_count_33[4 * i] == -1)
				break;
			//result = (HMDIDRIVER)(x_DWORD *)i;
		}
		if (i != 4)
		{
			S->FOR_loop_count_33[4 * i] = a4;
			//result = *(x_DWORD **)(S + 20);
			S->FOR_ptrs[4 * i] = S->EVNT_ptr_5;
		}
	}
	else if (a3 <= 0x75)
	{
		if (a4 >= 64)
		{
			for (j = 3; j >= 0; j--)
			{
				//result = (x_DWORD *)(S + 4 * j);
				if (S->FOR_loop_count_33[4 * j] != -1)
					break;
				//result = (x_DWORD *)j;
			}
			if (j != -1)
			{
				if (S->FOR_loop_count_33[4 * j])
				{
					//v6 = a1 + 4 * j;
					if (--S->FOR_loop_count_33[4 * j])
					{
						//result = (x_DWORD *)S;
						S->EVNT_ptr_5 = (Bit8u*)S->FOR_ptrs[4 * j];
					}
					else
					{
						//result = (x_DWORD *)(S + 4 * j);
						S->FOR_loop_count_33[4 * j] = -1;
					}
				}
				else
				{
					//result = (x_DWORD *)S;
					S->EVNT_ptr_5 = (Bit8u*)S->FOR_loop_count_33[4 * j];
				}
			}
		}
	}
	else if (a3 <= 0x76)
	{
		*(x_DWORD *)(S + 92) = 0;
		*(x_DWORD *)(S + 96) = 0;
		*(x_DWORD *)(S + 108) = 0;
		*(x_DWORD *)(S + 108) -= *(x_DWORD *)(S + 104);
		//result = (x_DWORD *)S;
		if (S->beat_callback_9)
			/*result = (x_DWORD *)*/(*(int(**)(HMDIDRIVER, HSEQUENCE, x_DWORD, x_DWORD))(S->beat_callback_9))(v7, S, 0, 0);
	}
	else
	{
		if (a3 != 119)
			goto LABEL_79;
		//result = (x_DWORD *)S;
		if (S->trigger_callback_8_32)
			/*result = (x_DWORD *)*/(*(int(**)(HSEQUENCE, int, signed int))(S->trigger_callback_8_32))(S, v8, a4);
	}
}

//----- (000A5E50) --------------------------------------------------------
void sub_A5E50(HSEQUENCE S)
{
	int v1; // eax
	int v3; // [esp+0h] [ebp-8h]
	signed int i; // [esp+4h] [ebp-4h]

	v3 = 0;
	for (i = 0; i < 32; i++)
	{
		if (S->seq_342[i] != -1)
		{
			sub_A5850(S, S->seq_342[i] | 0x80, S->seq_374[i], 0, 0);
			S->seq_342[i] = -1;
			v3++;
		}
	}
	S->seq_341 = 0;
	/*LOBYTE(v1) = */sub_A4EB0((HMDIDRIVER)S->driver_0);
	if (v3)
	{
		v1 = sub_92160();
		if (!v1)
			/*LOBYTE(v1) = */sub_920D0_AIL_delay(3);
	}
	//return v1;
}

//----- (000A5F30) --------------------------------------------------------
void sub_A5F30(int *a1, int a2)
{
	signed int i; // [esp+0h] [ebp-4h]

	for (i = 0; i < 32; i++)
	{
		if (a1[i + 342] == a2)
		{
			sub_A5850((HSEQUENCE)a1, a1[i + 342] | 0x80, a1[i + 374], 0, 0);
			a1[i + 342] = -1;
		}
	}
	return sub_A4EB0((HMDIDRIVER)*a1);
}

//----- (000A5FD0) --------------------------------------------------------
void sub_A5FD0(int a1, int a2)
{
	x_DWORD *result; // eax

	if (*(x_DWORD *)(a1 + 4 * a2 + 660) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0x72u, *(x_DWORD *)(a1 + 4 * a2 + 660), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 212) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xC0, *(x_DWORD *)(a1 + 4 * a2 + 212), 0, 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 340) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xE0, *(x_DWORD *)(a1 + 4 * a2 + 276), *(x_DWORD *)(a1 + 4 * a2 + 340), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 532) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0x6Bu, *(x_DWORD *)(a1 + 4 * a2 + 532), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 468) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0x6Fu, *(x_DWORD *)(a1 + 4 * a2 + 468), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 596) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0x70u, *(x_DWORD *)(a1 + 4 * a2 + 596), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 852) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 1u, *(x_DWORD *)(a1 + 4 * a2 + 852), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 916) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 7u, *(x_DWORD *)(a1 + 4 * a2 + 916), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 980) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0xAu, *(x_DWORD *)(a1 + 4 * a2 + 980), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 1044) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0xBu, *(x_DWORD *)(a1 + 4 * a2 + 1044), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 1108) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0x40u, *(x_DWORD *)(a1 + 4 * a2 + 1108), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 1172) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0x5Bu, *(x_DWORD *)(a1 + 4 * a2 + 1172), 0);
	if (*(x_DWORD *)(a1 + 4 * a2 + 1236) != -1)
		sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 0x5Du, *(x_DWORD *)(a1 + 4 * a2 + 1236), 0);
	result = (x_DWORD *)(a1 + 4 * a2);
	if (result[325] != -1)
		/*result = (int32*)*/sub_A5850((HSEQUENCE)a1, a2 | 0xB0, 6u, *(x_DWORD *)(a1 + 4 * a2 + 1300), 0);
}

//----- (000A6370) --------------------------------------------------------
void sub_A6370(HSEQUENCE a1)//287370
{
	//x_DWORD *result; // eax
	signed int i; // [esp+0h] [ebp-4h]
	signed int j; // [esp+0h] [ebp-4h]
	signed int k; // [esp+0h] [ebp-4h]

	for (i = 0; i < 16; i++)
		a1->chan_map_37[i] = i;
	memset((void*)&a1->shadow_53, -1, sizeof(CTRL_LOG));
	for (j = 0; j < 4; j++)
		a1->FOR_loop_count_33[j] = -1;
	for (k = 0; k < 32; k++)
		a1->seq_342[k] = -1;
	a1->seq_341 = 0;
	a1->interval_count_12 = 0;
	a1->beat_count_23 = 0;
	a1->measure_count_24 = -1;
	a1->beat_fraction_27 = 0;
	a1->time_fraction_26 = 0;
	a1->time_numerator_25 = 4;
	a1->time_per_beat_28 = 8000000;
	//result = a1;
	a1->interval_num_13 = 0;
	//return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);

//----- (000A6490) --------------------------------------------------------
void sub_A6490(HSEQUENCE S)
{
	//x_DWORD *result; // eax

	sub_A6370(S);
	//result = a1;
	S->EVNT_ptr_5 = (Bit8u*)S->EVNT_4 + 8;
	//return result;
}

//----- (000A64C0) --------------------------------------------------------
void sub_A64C0(HSEQUENCE S)
{
	for (Bit32s i = 0; i < 16; i++)
	{
		if (S->shadow_53.pan[i] != -1)
			sub_A5850(S, i | 0xB0, 7u, S->shadow_53.pan[i], 0);
	}
}

void sub_A6530(HMDIDRIVER a1)//287530
{
	//int result; // eax
	//SEQUENCE v2; // eax
	int v3; // eax
	HSEQUENCE v4; // eax
	HSEQUENCE v5; // eax
	HSEQUENCE v6; // eax
	HSEQUENCE v7; // esi
	unsigned __int8 *v8; // edx

	//result = (int)a1;
	if (!a1->disable_5 && !x_DWORD_E4B18)
	{
		x_DWORD_E4B18 = 1;
		x_DWORD_181EC4 = a1->n_sequences_7;
		for (x_DWORD_181EB8 = a1->sequences_6; ; x_DWORD_181EB8 += 1816)
		{
			if (!x_DWORD_181EC4)
			{
				/*result = */sub_A4EB0(a1);
				x_DWORD_E4B18 = 0;
				//return result;
				return;
			}
			if (*(_DWORD *)(x_DWORD_181EB8 + 4) == 4)
			{
				x_DWORD_181ED4 = 0;
				++*(_DWORD *)(x_DWORD_181EB8 + 52);
				*(_DWORD *)(x_DWORD_181EB8 + 88) += *(_DWORD *)(x_DWORD_181EB8 + 72);
				while (*(_DWORD *)(x_DWORD_181EB8 + 88) >= 100)
				{
					*(_DWORD *)(x_DWORD_181EB8 + 88) -= 100;
					if (*(_DWORD *)(x_DWORD_181EB8 + 1364) > 0)
					{
						for (x_DWORD_181EBC = 0; x_DWORD_181EBC < 32; x_DWORD_181EBC++)
						{
							if (*(_DWORD *)(x_DWORD_181EB8 + 4 * x_DWORD_181EBC + 1368) != -1)
							{
								//v2 = x_DWORD_181EB8[4 * x_DWORD_181EBC];
								if (--x_DWORD_181EB8->note_time[x_DWORD_181EBC] <= 0)
								{
									v3 = *(_DWORD *)(x_DWORD_181EB8 + 4 * x_DWORD_181EBC + 1368);
									LOBYTE(v3) = v3 | 0x80;
									sub_A5850(x_DWORD_181EB8, v3, *(_DWORD *)(4 * x_DWORD_181EBC + x_DWORD_181EB8 + 1496), 0, 0);
									*(_DWORD *)(x_DWORD_181EB8 + 4 * x_DWORD_181EBC + 1368) = -1;
									v4 = x_DWORD_181EB8;
									if (!--*(_DWORD *)(v4 + 1364))
										break;
								}
							}
						}
					}
					v5 = x_DWORD_181EB8;
					if (--*(_DWORD *)(v5 + 48) <= 0)
					{
						while (1)
						{
							while (1)
							{
								x_DWORD_181ED0 = **(unsigned __int8 **)(x_DWORD_181EB8 + 20);
								if ((unsigned int)x_DWORD_181ED0 < 0x80 || x_DWORD_181ED4)
								{
									if (!x_DWORD_181ED4)
									{
										v8 = (unsigned __int8 *)(*(_DWORD *)(x_DWORD_181EB8 + 20))++;
										*(_DWORD *)(x_DWORD_181EB8 + 48) = *v8;
									}
									goto LABEL_62;
								}
								if ((unsigned int)x_DWORD_181ED0 >= 0xF7)
									break;
								if (x_DWORD_181ED0 != 240)
									goto LABEL_49;
							LABEL_48:
								x_DWORD_181EB0 = *(_DWORD *)(x_DWORD_181EB8 + 20) + 1;
								x_DWORD_181ED8 = sub_A5040(&x_DWORD_181EB0);
								x_DWORD_181ED8 += x_DWORD_181EB0 - *(_DWORD *)(x_DWORD_181EB8 + 20);
								sub_A4FD0((int)a1, *(_DWORD *)(x_DWORD_181EB8 + 20), x_DWORD_181ED8);
								*(_DWORD *)(x_DWORD_181EB8 + 20) += x_DWORD_181ED8;
							}
							if ((unsigned int)x_DWORD_181ED0 <= 0xF7)
								goto LABEL_48;
							if (x_DWORD_181ED0 == 255)
							{
								x_DWORD_181EDC = *(unsigned __int8 *)(*(_DWORD *)(x_DWORD_181EB8 + 20) + 1);
								*(_DWORD *)(x_DWORD_181EB8 + 20) += 2;
								x_DWORD_181ED8 = sub_A5040((int32*)x_DWORD_181EB8 + 20);
								if ((unsigned int)x_DWORD_181EDC < 0x51)
								{
									if (x_DWORD_181EDC == 47)
									{
										x_DWORD_181ED4 = 1;
										if (*(_DWORD *)(x_DWORD_181EB8 + 44)
											&& (v6 = x_DWORD_181EB8, --*(_DWORD *)(v6 + 44), !*(_DWORD *)(v6 + 44)))
										{
											sub_95DE0_AIL_stop_sequence(x_DWORD_181EB8);
											*(_DWORD *)(x_DWORD_181EB8 + 4) = 2;
											if (*(_DWORD *)(x_DWORD_181EB8 + 40))
												(*(void(__cdecl **)(HSEQUENCE))(x_DWORD_181EB8 + 40))(x_DWORD_181EB8);
										}
										else
										{
											*(_DWORD *)(x_DWORD_181EB8 + 20) = *(_DWORD *)(x_DWORD_181EB8 + 16) + 8;
											*(_DWORD *)(x_DWORD_181EB8 + 92) = 0;
											*(_DWORD *)(x_DWORD_181EB8 + 96) = -1;
											*(_DWORD *)(x_DWORD_181EB8 + 108) = 0;
											if (*(_DWORD *)(x_DWORD_181EB8 + 36))
												(*(void(__cdecl **)(_DWORD, HSEQUENCE, _DWORD, _DWORD))(x_DWORD_181EB8 + 36))(
													*(_DWORD *)x_DWORD_181EB8,
													x_DWORD_181EB8,
													0,
													0);
										}
									}
								}
								else if ((unsigned int)x_DWORD_181EDC <= 0x51)
								{
									x_DWORD_181ECC = *(unsigned __int8 *)(*(_DWORD *)(x_DWORD_181EB8 + 20) + 2)
										+ (**(unsigned __int8 **)(x_DWORD_181EB8 + 20) << 16)
										+ (*(unsigned __int8 *)(*(_DWORD *)(x_DWORD_181EB8 + 20) + 1) << 8);
									*(_DWORD *)(x_DWORD_181EB8 + 112) = 16 * x_DWORD_181ECC;
								}
								else if (x_DWORD_181EDC == 88)
								{
									*(_DWORD *)(x_DWORD_181EB8 + 100) = **(unsigned __int8 **)(x_DWORD_181EB8 + 20);
									x_DWORD_181ECC = *(unsigned __int8 *)(*(_DWORD *)(x_DWORD_181EB8 + 20) + 1) - 2;
									x_DWORD_181EC8 = 16000000 / x_DWORD_181DAC[11];
									if (x_DWORD_181ECC >= 0)
									{
										*(_DWORD *)(x_DWORD_181EB8 + 104) = x_DWORD_181EC8 << x_DWORD_181ECC;
									}
									else
									{
										x_DWORD_181ECC = -x_DWORD_181ECC;
										*(_DWORD *)(x_DWORD_181EB8 + 104) = x_DWORD_181EC8 >> x_DWORD_181ECC;
									}
									*(_DWORD *)(x_DWORD_181EB8 + 108) = 0;
									*(_DWORD *)(x_DWORD_181EB8 + 108) -= *(_DWORD *)(x_DWORD_181EB8 + 104);
									*(_DWORD *)(x_DWORD_181EB8 + 92) = 0;
									++*(_DWORD *)(x_DWORD_181EB8 + 96);
									if (*(_DWORD *)(x_DWORD_181EB8 + 36))
										(*(void(__cdecl **)(_DWORD, HSEQUENCE, _DWORD, _DWORD))(x_DWORD_181EB8 + 36))(
											*(_DWORD *)x_DWORD_181EB8,
											x_DWORD_181EB8,
											*(_DWORD *)(x_DWORD_181EB8 + 92),
											*(_DWORD *)(x_DWORD_181EB8 + 96));
								}
								*(_DWORD *)(x_DWORD_181EB8 + 20) += x_DWORD_181ED8;
							}
							else
							{
							LABEL_49:
								x_DWORD_181EB4 = x_DWORD_181ED0 & 0xF;
								x_DWORD_181ED0 &= 0xF0u;
								sub_A5850(
									x_DWORD_181EB8,
									**(unsigned __int8 **)(x_DWORD_181EB8 + 20),
									*(unsigned __int8 *)(*(_DWORD *)(x_DWORD_181EB8 + 20) + 1),
									*(unsigned __int8 *)(*(_DWORD *)(x_DWORD_181EB8 + 20) + 2),
									1);
								if (x_DWORD_181ED0 == 144)
								{
									for (x_DWORD_181EBC = 0;
										x_DWORD_181EBC < 32 && *(_DWORD *)(x_DWORD_181EB8 + 4 * x_DWORD_181EBC + 1368) != -1;
										++x_DWORD_181EBC)
									{
										;
									}
									if (x_DWORD_181EBC == 32)
									{
										qmemcpy(x_BYTE_181C90, (void*)"Internal note queue overflow\n\0", 0x1Eu);
										sub_95DE0_AIL_stop_sequence(x_DWORD_181EB8);
										//result = x_DWORD_181EB8;
										*(_DWORD *)(x_DWORD_181EB8 + 4) = 2;
										x_DWORD_E4B18 = 0;
										//return result;
										return;
									}
									++*(_DWORD *)(x_DWORD_181EB8 + 1364);
									*(_DWORD *)(4 * x_DWORD_181EBC + x_DWORD_181EB8 + 1368) = x_DWORD_181EB4;
									*(_DWORD *)(4 * x_DWORD_181EBC + x_DWORD_181EB8 + 1496) = *(unsigned __int8 *)(*(_DWORD *)(x_DWORD_181EB8 + 20)
										+ 1);
									*(_DWORD *)(x_DWORD_181EB8 + 20) += 3;
									*(_DWORD *)(x_DWORD_181EB8 + 4 * x_DWORD_181EBC + 1624) = sub_A5040((int32*)x_DWORD_181EB8 + 20);
								}
								else
								{
									v7 = x_DWORD_181EB8;
									*(_DWORD *)(v7 + 20) += sub_A4E10(**(unsigned __int8 **)(x_DWORD_181EB8 + 20));
								}
							}
						}
					}
				LABEL_62:
					if (!x_DWORD_181ED4)
					{
						*(_DWORD *)(x_DWORD_181EB8 + 108) += *(_DWORD *)(x_DWORD_181EB8 + 104);
						if (*(_DWORD *)(x_DWORD_181EB8 + 108) >= *(_DWORD *)(x_DWORD_181EB8 + 112))
						{
							*(_DWORD *)(x_DWORD_181EB8 + 108) -= *(_DWORD *)(x_DWORD_181EB8 + 112);
							if (++*(_DWORD *)(x_DWORD_181EB8 + 92) >= *(_DWORD *)(x_DWORD_181EB8 + 100))
							{
								*(_DWORD *)(x_DWORD_181EB8 + 92) = 0;
								++*(_DWORD *)(x_DWORD_181EB8 + 96);
							}
							if (*(_DWORD *)(x_DWORD_181EB8 + 36))
							{
								sub_97330_AIL_sequence_position((int32*)x_DWORD_181EB8, &x_DWORD_181EBC, &x_DWORD_181EC0);
								(*(void(__cdecl **)(_DWORD, HSEQUENCE, int, int))(x_DWORD_181EB8 + 36))(
									*(_DWORD *)x_DWORD_181EB8,
									x_DWORD_181EB8,
									x_DWORD_181EBC,
									x_DWORD_181EC0);
							}
						}
					}
				}
				if (!x_DWORD_181ED4)
				{
					if (*(_DWORD *)(x_DWORD_181EB8 + 56) != *(_DWORD *)(x_DWORD_181EB8 + 60))
					{
						*(_DWORD *)(x_DWORD_181EB8 + 64) += *(_DWORD *)(*(_DWORD *)x_DWORD_181EB8 + 16);
						do
						{
							if (*(_DWORD *)(x_DWORD_181EB8 + 64) < *(_DWORD *)(x_DWORD_181EB8 + 68))
								break;
							*(_DWORD *)(x_DWORD_181EB8 + 64) -= *(_DWORD *)(x_DWORD_181EB8 + 68);
							if (*(_DWORD *)(x_DWORD_181EB8 + 60) <= *(_DWORD *)(x_DWORD_181EB8 + 56))
								--*(_DWORD *)(x_DWORD_181EB8 + 56);
							else
								++*(_DWORD *)(x_DWORD_181EB8 + 56);
						} while (*(_DWORD *)(x_DWORD_181EB8 + 56) != *(_DWORD *)(x_DWORD_181EB8 + 60));
						if (!(*(_BYTE *)(x_DWORD_181EB8 + 52) & 7))
							sub_A64C0(x_DWORD_181EB8);
					}
					if (*(_DWORD *)(x_DWORD_181EB8 + 72) != *(_DWORD *)(x_DWORD_181EB8 + 76))
					{
						*(_DWORD *)(x_DWORD_181EB8 + 80) += *(_DWORD *)(*(_DWORD *)x_DWORD_181EB8 + 16);
						do
						{
							if (*(_DWORD *)(x_DWORD_181EB8 + 80) < *(_DWORD *)(x_DWORD_181EB8 + 84))
								break;
							*(_DWORD *)(x_DWORD_181EB8 + 80) -= *(_DWORD *)(x_DWORD_181EB8 + 84);
							if (*(_DWORD *)(x_DWORD_181EB8 + 76) <= *(_DWORD *)(x_DWORD_181EB8 + 72))
								--*(_DWORD *)(x_DWORD_181EB8 + 72);
							else
								++*(_DWORD *)(x_DWORD_181EB8 + 72);
						} while (*(_DWORD *)(x_DWORD_181EB8 + 72) != *(_DWORD *)(x_DWORD_181EB8 + 76));
					}
				}
			}
			--x_DWORD_181EC4;
		}
	}
	//return result;
	return;
}

//----- (000A6E00) --------------------------------------------------------
int sub_A6E00_sound_proc25(HMDIDRIVER a1, IO_PARMS* a2)//287e00
{
	IO_PARMS v3; // [esp+0h] [ebp-24h]
	/*__int16 v4; // [esp+2h] [ebp-22h]
	__int16 v5; // [esp+4h] [ebp-20h]
	__int16 v6; // [esp+6h] [ebp-1Eh]*/
	//int v7; // [esp+8h] [ebp-1Ch]
	unsigned int i; // [esp+1Ch] [ebp-8h]
	IO_PARMS v9; // [esp+20h] [ebp-4h]


	qmemcpy(&v3, (void*)a2, 0x18u);
	if (1/*a1->var0_aildrv->AIL_DRIVER_var4_VHDR->environment_string*//**(x_WORD *)(*(x_DWORD *)(*a1 + 16) + 16)*/)//fixed
	{
		/*v9 = (__int16 *)((a1->var0_aildrv->AIL_DRIVER_var4_VHDR->VDI_HDR_var12) & 0xFFFF
					   + 16 * a1->var0_aildrv->AIL_DRIVER_var4_VHDR->VDI_HDR_var12 >> 16);
		*/
		v9.IO = 0x220;
		v9.IRQ = -1;
		v9.DMA_8_bit = -1;
		v9.DMA_16_bit = -1;
		v9.IO_reserved[0] = -1;
		v9.IO_reserved[1] = -1;
		v9.IO_reserved[2] = -1;
		v9.IO_reserved[3] = -1;//fixed
		if (v3.IO < 1)
			v3.IO = v9.IO;
		if (v3.IRQ < 1)
			v3.IRQ = v9.IRQ;
		if (v3.DMA_8_bit < 1)
			v3.DMA_8_bit = v9.DMA_8_bit;
		if (v3.DMA_16_bit < 1)
			v3.DMA_16_bit = v9.DMA_16_bit;
		for (i = 0; i < 4; i++)
		{
			if (v3.IO_reserved[i] < 1)
				v3.IO_reserved[i] = v9.IO_reserved[i];
		}
	}
	qmemcpy((void *)&a1->drvr_0->VHDR_4->IO, &v3, 0x18u);
	return sub_91F70_AIL_call_driver(a1->drvr_0, 772, 0, 0);
}

//----- (000A6FB0) --------------------------------------------------------
HMDIDRIVER sub_A6FB0_sound_proc26(AIL_DRIVER* a1, IO_PARMS *a2)//287fb0
{
	__int64 v2; // rtt
	unsigned int v3; // eax
	IO_PARMS v5; // [esp+0h] [ebp-3Ch]
	VDI_CALL v6; // [esp+18h] [ebp-24h]
	//unsigned __int16 v7; // [esp+1Ah] [ebp-22h]
	//unsigned __int16 v8; // [esp+1Ch] [ebp-20h]
	//unsigned __int16 v9; // [esp+1Eh] [ebp-1Eh]
	HMDIDRIVER v10; // [esp+24h] [ebp-18h]
	Bit8u* v11; // [esp+28h] [ebp-14h]
	const char* v11x = ".OPL\0";
	HMDIDRIVER v12; // [esp+2Ch] [ebp-10h]
	int v13; // [esp+30h] [ebp-Ch]
	int i; // [esp+34h] [ebp-8h]
	x_BYTE *v15; // [esp+38h] [ebp-4h]
	void *v16; // [esp+50h] [ebp+14h]
	const void *v17; // [esp+50h] [ebp+14h]

	//fix it
	v15 = 0;
	v16 = 0;
	v10 = 0;
	//fix it

	sub_A4CF0();
	v12 = (HMDIDRIVER)sub_9D450_malloc4(sizeof(_MDI_DRIVER));
	if (v12)
	{
		v12->drvr_0 = a1;
		if (v12->drvr_0->type_5 == 1)
		{
			sub_91F70_AIL_call_driver(v12->drvr_0, 769, 0, &v6);
			//v12->var1_aildrv = sound_buffer[v6.AX];//v6.AX + 16 * v6.DX;
			v12->DST_2 = (MDI_DST*)sound_buffer[0/*v6.BX*/];//v6.BX + 16 * v6.CX;
			//v11 = sound_buffer[*v12->var1_aildrv];//(x_BYTE *)((*(x_DWORD *)v12[1] & 0xFFFF) + 16 * (*(x_DWORD *)v12[1] >> 16));

			v11 = 0;
			if (v11)
			{
				if (*v11)
				{
					//_dupenv_s((char**)v15,(size_t*)sizeof(v11),(char*)v11);
					v15 = mygetenv((const char*)v11);
					if (v15)
					{
						if (*v15)
							strncpy((char*)v12->DST_2, strlen((const char*)v12->DST_2) + 1, v15, 128);
					}
				}
			}
			//v11 = (Bit8u*)v12->var0_aildrv->AIL_DRIVER_var4_VHDR->mdiapplendix;//(x_BYTE *)((*(x_DWORD *)(v12[1] + 4) & 0xFFFF) + 16 * (*(x_DWORD *)(v12[1] + 4) >> 16));

			if (v11x && *v11x)
			{
				strcpy((char*)v12->DST_2->GTL_filename, strlen((const char*)aSample) + 1, aSample);
				strcpy((char*)v12->DST_2->GTL_filename + strlen((const char*)aSample), strlen((const char*)v11x) + 1, (const char*)v11x);
			}
			else
			{
				*(x_BYTE *)(v12->DST_2 + 128) = 0;
			}
			memset(&unk_181D90, -1, 24);
			v13 = 0;
			if (a2)
			{
				qmemcpy(&unk_181D90, (void*)a2, 0x18u);
				if (sub_A6E00_sound_proc25(v12, a2))
				{
					v13 = 1;
					qmemcpy(&v5, (void*)a2, 0x18u);
				}
			}
			if (!v13)
			{
				v16 = sub_92ED0_AIL_get_IO_environment(v12->drvr_0);
				if (v16)
				{
					qmemcpy(&unk_181D90, v16, 0x18u);
					if (sub_A6E00_sound_proc25(v12, (IO_PARMS*)v16))
					{
						v13 = 1;
						qmemcpy(&v5, v16, 0x18u);
					}
				}
			}
			if (!v13 && x_DWORD_181DAC[17] == 1)
			{
				for (i = 0; *(unsigned __int16 *)v12->drvr_0->VHDR_4->environment_string_16 > i; i++)
				{
					v17 = (const void *)(24 * i
						+ (v12->drvr_0->VHDR_4->VDI_HDR_var12 & 0xFFFF)
						+ 16 * v12->drvr_0->VHDR_4->VDI_HDR_var12);
					if (!i)
						qmemcpy(&unk_181D90, (void*)v17, 0x18u);
					if (sub_A6E00_sound_proc25(v12, (IO_PARMS*)v17))
					{
						v13 = 1;
						qmemcpy(&v5, (void*)v17, 0x18u);
						break;
					}
				}
			}
			if (v13)
			{
				qmemcpy(&unk_181D90, &v5, 0x18u);
				sub_91F70_AIL_call_driver(v12->drvr_0, 773, 0, 0);
				v12->drvr_0->initialized_6 = 1;
				sub_91F70_AIL_call_driver(v12->drvr_0, 1281, 0, &v6);
				if (v6.AX)
				{
					v12->n_sequences_7 = x_DWORD_181DAC[12];
					v12->sequences_6 = (HSEQUENCE)sub_9D450_malloc4(sizeof(SEQUENCE) * v12->n_sequences_7);
					if (v12->sequences_6)
					{
						for (i = 0; i < v12->n_sequences_7; i++)
						{
							v12->sequences_6[i].status_1 = 1;
							v12->sequences_6[i].driver_0 = v12;
						}
						v12->var104_aildrv = 0;
						v12->var105_aildrv = 0;
						v12->var106_aildrv = 0;
						v12->var107_aildrv = 0;
						LODWORD(v2) = 1000000;
						HIDWORD(v2) = 1000000 >> 31;
						v12->interval_time_4 = v2 / x_DWORD_181DAC[11];
						v12->disable_5 = 0;
						v12->var108_aildrv = 127;
						/*for ( i = 0; i < 16; i++ )
						{
						  v12[i + 8] = 0;
						  v12[i + 24] = 0;
						  v12[i + 40] = 0;
						  v12[i + 56] = 0;
						  v12[i + 72] = 0;
						  v12[i + 88] = 0;
						}*/
						v3 = sub_92600_AIL_register_timer(sub_A6530);
						v12->timer_3 = v3;
						if (v12->timer_3 == -1)
						{
							qmemcpy(&x_BYTE_181C90, (void*)"Out of timer handles\n", 0x16u);
							sub_91F70_AIL_call_driver(v12->drvr_0, 774, 0, 0);
							v12->drvr_0->initialized_6 = 0;
							sub_9D490_free4((void*)v12->sequences_6, 1816 * v12->n_sequences_7);
							sub_9D490_free4(v12, 468);
							v10 = 0;
						}
						else
						{
							sub_92740_AIL_set_timer_user(v12->timer_3, (signed __int32)v12);
							v12->drvr_0->destructor_9 = sub_A6F30;
							v12->drvr_0->descriptor_10 = v12;
							for (i = 0; i < 16; i++)
							{
								sub_A4F10(v12, i | 0xB0, 114, 0);
								sub_A4F10(v12, i | 0xC0, 0, 0);
								sub_A4F10(v12, i | 0xE0, 0, 64);
								sub_A4F10(v12, i | 0xB0, 112, 0);
								sub_A4F10(v12, i | 0xB0, 1, 0);
								sub_A4F10(v12, i | 0xB0, 7, x_DWORD_181DAC[13]);
								sub_A4F10(v12, i | 0xB0, 10, 64);
								sub_A4F10(v12, i | 0xB0, 11, 127);
								sub_A4F10(v12, i | 0xB0, 64, 0);
								sub_A4F10(v12, i | 0xB0, 91, 40);
								sub_A4F10(v12, i | 0xB0, 93, 0);
								sub_A4F10(v12, i | 0xB0, 100, 0);
								sub_A4F10(v12, i | 0xB0, 101, 0);
								sub_A4F10(v12, i | 0xB0, 38, 0);
								sub_A4F10(v12, i | 0xB0, 6, x_DWORD_181DAC[16]);
								sub_A4EB0(v12);
								if (!(i & 3))
									sub_920D0_AIL_delay(3);
							}
							sub_92930_AIL_set_timer_frequency(v12->timer_3, x_DWORD_181DAC[11]);
							sub_92BA0_AIL_start_timer(v12->timer_3);
							v10 = v12;
						}
					}
					else
					{
						qmemcpy(&x_BYTE_181C90, (void*)"Could not allocate SEQUENCE structures\n", 0x28u);
						sub_91F70_AIL_call_driver(v12->drvr_0, 774, 0, 0);
						v12->drvr_0->initialized_6 = 0;
						sub_9D490_free4(v12, 468);
						v10 = 0;
					}
				}
				else
				{
					qmemcpy(&x_BYTE_181C90, (void*)"Could not initialize instrument manager\n", 0x29u);
					sub_91F70_AIL_call_driver(v12->drvr_0, 774, 0, 0);
					v12->drvr_0->initialized_6 = 0;
					sub_9D490_free4(v12, 468);
					v10 = 0;
				}
			}
			else
			{
				qmemcpy(&x_BYTE_181C90, (void*)"XMIDI sound hardware not found\n", 0x20u);
				sub_9D490_free4(v12, 468);
				v10 = 0;
			}
		}
		else
		{
			qmemcpy(&x_BYTE_181C90, (void*)".MDI driver required\n", 0x16u);
			sub_9D490_free4(v12, 468);
			v10 = 0;
		}
	}
	else
	{
		qmemcpy(&x_BYTE_181C90, (void*)"Could not allocate memory for driver\n", 0x26u);
		v10 = 0;
	}

	return v10;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 99D46: using guessed type x_DWORD strcpy(x_DWORD, x_DWORD);
// 99D84: using guessed type x_DWORD strcat(x_DWORD, x_DWORD);
// 99E33: using guessed type x_DWORD getenv(x_DWORD);
// A6530: using guessed type int sub_A6530();
// A6F30: using guessed type int sub_A6F30();
// AD871: using guessed type x_DWORD strncpy(x_DWORD, x_DWORD, x_DWORD);
// 181C90: using guessed type char x_BYTE_181C90;
// 181DD8: using guessed type int x_DWORD_181DD8;
// 181DDC: using guessed type int x_DWORD_181DDC;
// 181DE0: using guessed type int x_DWORD_181DE0;
// 181DEC: using guessed type int x_DWORD_181DEC;
// 181DF0: using guessed type int x_DWORD_181DF0;

//----- (000A77D0) --------------------------------------------------------
HMDIDRIVER sub_A77D0_AIL_API_install_MDI_INI(char* filename, IO_PARMS *IO)//2887d0
{
	int v3; // eax
	HMDIDRIVER v5; // [esp+0h] [ebp-10h]
	Bit8u* v6; // [esp+4h] [ebp-Ch]
	HMDIDRIVER v7; // [esp+8h] [ebp-8h]
	AIL_DRIVER *v8; // [esp+Ch] [ebp-4h]

	v6 = sub_9DEA0_read_file(filename, 0);
	if (v6)
	{
		v3 = sub_9DE20_get_file_lenght(filename);
		v8 = sub_93010_AIL_install_driver(/*(int)a1,*/ v6, v3);
		x_free((void*)v6);
		if (v8)
		{
			v7 = sub_A6FB0_sound_proc26(v8, IO);
			if (!v7)
				sub_93160_AIL_uninstall_driver((AIL_DRIVER*)v8);
			v5 = v7;
		}
		else
		{
			v5 = 0;
		}
	}
	else
	{
		strcpy(x_BYTE_181C90, strlen("Driver file not found\n") + 1, "Driver file not found\n");
		v5 = 0;
	}
	return v5;
}
// E3E34: using guessed type int (*off_E3E34_freex)(int);
// 181C90: using guessed type char x_BYTE_181C90;

//----- (000A7880) --------------------------------------------------------
HMDIDRIVER sub_A7880_sound_proc27(int a1, Bit8u* a2, int a3, IO_PARMS* a4)
{
	HMDIDRIVER v6; // [esp+4h] [ebp-8h]
	AIL_DRIVER *v7; // [esp+8h] [ebp-4h]

	v7 = sub_93010_AIL_install_driver(/*a1, */a2, a3);
	if (!v7)
		return 0;
	v6 = sub_A6FB0_sound_proc26(v7, a4);
	if (!v6)
		sub_93160_AIL_uninstall_driver((AIL_DRIVER*)v7);
	return v6;
}

//----- (000A78F0) --------------------------------------------------------
Bit32s sub_A78F0_AIL_API_install_MDI_INI(/*int a1, */HMDIDRIVER *mdi) //2888f0
{
	AIL_INI v3; // [esp+0h] [ebp-11Ch]
	//char v4; // [esp+80h] [ebp-9Ch]
	//char v5; // [esp+100h] [ebp-1Ch]
	int result; // [esp+118h] [ebp-4h]

	/*char v8x[200];
	IO_PARMS v9x = {};*/

	if (sub_92190_AIL_read_INI(&v3, (char*)"MDI.INI"/*,v8x,v9x*/))
	{
		*mdi = sub_95850_AIL_install_MDI_driver_file(v3.driver_name, &v3.IO);
		if (*mdi)
			result = 0;
		else
			result = 2;
	}
	else
	{
		qmemcpy(&x_BYTE_181C90, (void*)"Unable to open file MDI.INI\n", 0x1Du);
		result = 1;
	}
	return result;
}
// 181C90: using guessed type char x_BYTE_181C90;

//----- (000A7970) --------------------------------------------------------
int sub_A7970_AIL_API_uninstall_MDI_driver(HMDIDRIVER mdi)//288970
{
	return sub_93160_AIL_uninstall_driver(mdi->drvr_0);
	return 0;
}

//----- (000A7990) --------------------------------------------------------
Bit32s sub_A7990_AIL_API_MDI_driver_type(HMDIDRIVER mdi)//288990
{
	int v3; // [esp+4h] [ebp-4h]
	char *v4; // [esp+4h] [ebp-4h]

	const char* v4help = ".OPL";//fix;

	if (mdi->drvr_0->VHDR_4->driver_version_8 >= 0x112u)
	{
		v3 = mdi->drvr_0->VHDR_4->VDI_HDR_var46;
		if (!_stricmp((const char*)v3, "Tandy 3-voice music"))
			return 3;
		if (!_stricmp((const char*)v3, "IBM internal speaker music"))
			return 3;
	}
	/*v4 = (x_BYTE *)((*(x_DWORD *)(mdi[1] + 4) & 0xFFFF) + 16 * (*(x_DWORD *)(mdi[1] + 4) >> 16));
	if ( !v4 || !*v4 )
	  return 0;*/ // fix
	v4 = (char*)v4help;
	if (!_stricmp(v4, ".AD"))
		return mdi->drvr_0->size_3 <= 0x5000u;
	if (!_stricmp(v4, ".OPL"))
		return 2;
	return 0;
}
// 98F5D: using guessed type x_DWORD stricmp(x_DWORD, x_DWORD);

//----- (000A7AA0) --------------------------------------------------------
void sub_A7AA0_AIL_API_set_GTL_filename_prefix(Bit8u* a1)//288aa0
{
	int samplesize; // eax
	int i; // [esp+0h] [ebp-4h]

	strcpy(aSample, 512, (const char*)a1);
	samplesize = strlen(aSample) - 1;
	for (i = samplesize; i; i--)
	{
		if (aSample[i] == 92)
		{
			samplesize = strlen(aSample) - 1;
			if (samplesize == i)
			{
				samplesize = i;
				aSample[i] = 0;
			}
			return;
		}
		if (aSample[i] == 46)
		{
			samplesize = i;
			aSample[i] = 0;
			return;
		}
		samplesize = i;
	}
	return;
}
// 99D46: using guessed type x_DWORD strcpy(x_DWORD, x_DWORD);
// 99D6B: using guessed type x_DWORD strlen(x_DWORD);
/*
sub_91BD0_s_plus();
	for (i = 0; i < dig->var24_aildrv && dig->var23_aildrvx[i].status_1 != 1; i++)
		;
	if (i == dig->var24_aildrv)
	{
		qmemcpy(&x_BYTE_181C90, (void*)"Out of sample handles\n", 0x17u);
		sub_91BF0_s_minus();
		v3 = 0;
	}
	else
	{
		v1 = &dig->var23_aildrvx[i];
		sub_93830_AIL_init_sample(v1);
		sub_91BF0_s_minus();
		v3 = v1;
	}
	return v3;
*/
//----- (000A7B30) --------------------------------------------------------
HSEQUENCE sub_A7B30_AIL_API_allocate_sequence_handle(HMDIDRIVER mdi)//288b30
{
	HSEQUENCE v1; // ST08_4
	HSEQUENCE v3; // [esp+0h] [ebp-Ch]
	int i; // [esp+8h] [ebp-4h]

	//fix it
	v3 = 0;
	//fix it

	sub_91BD0_s_plus();
	for (i = 0; i < mdi->n_sequences_7 && mdi->sequences_6[i].status_1 != 1; i++)
		;
	if (i == mdi->n_sequences_7)
	{
		qmemcpy(&x_BYTE_181C90, (void*)"Out of sequence handles\n", 0x19u);
		sub_91BF0_s_minus();
		v3 = 0;
	}
	else
	{
		v1 = &mdi->sequences_6[i];
		v1->status_1 = 2;
		sub_A6370(v1);
		v1->EOS_10 = 0;
		sub_91BF0_s_minus();
		v3 = v1;
	}
	return v3;
}
// 181C90: using guessed type char x_BYTE_181C90;

//----- (000A7BF0) --------------------------------------------------------
void sub_A7BF0_sound_proc33(HSEQUENCE S)//288bf0
{
	//int *result; // eax

	//fix it
	//result = 0;
	//fix itsub_95C00_AIL_init_sequence

	if (S)
	{
		sub_95DE0_AIL_stop_sequence(S);
		//result = S;
		S->status_1 = 1;
	}
	//return result;
}

Bit32s sub_A7C20_AIL_API_init_sequence(HSEQUENCE S, void* start, Bit32s sequence_num, Bit32u track) {
	//S->ICA_6 = 0;
	//S->prefix_callback_7 = 0;
	//S->trigger_callback_8_32 = 0;
	//S->beat_callback_9 = 0;
	//S->EOS_10 = 0;
	S->loop_count_11 = 1;
	S->volume_14 = x_DWORD_181DAC[13];
	S->volume_target_15 = x_DWORD_181DAC[13];
	S->volume_period_17 = 0;
	S->volume_accum_16 = 0;
	/*S->tempo_percent_18 = 100;
	S->tempo_target_19 = 100;
	S->tempo_period_21 = 0;
	S->tempo_accum_20 = 0;
	S->tempo_error_22 = 0;*/
	S->sequence_num = sequence_num;
	SOUND_init_MIDI_sequence(x_DWORD_E3810_music_data, x_DWORD_E3808_music_header, track - 1);
	return 1;
}

//----- (000A7C20) --------------------------------------------------------
Bit32s sub_A7C20_AIL_API_init_sequence_orig(HSEQUENCE S, void* start, Bit32s sequence_num)
{
	VDI_CALL v4; // [esp+0h] [ebp-30h]
	Bit32u v5; // [esp+2h] [ebp-2Eh]
	int v6; // [esp+Ch] [ebp-24h]
	int v7; // [esp+10h] [ebp-20h]
	unsigned int v8; // [esp+14h] [ebp-1Ch]
	x_WORD *v9; // [esp+18h] [ebp-18h]
	unsigned int v10; // [esp+1Ch] [ebp-14h]
	unsigned int i; // [esp+20h] [ebp-10h]
	unsigned int v12; // [esp+24h] [ebp-Ch]
	Bit8u* v13; // [esp+28h] [ebp-8h]
	Bit8u* v14; // [esp+2Ch] [ebp-4h]
	//int v14i;//delete

	//fix it
	v5 = 0;
	v14 = 0;
	//fix it

	if (!S)
		return 0;
	S->status_1 = 2;
	v14 = sub_A50F0((Bit8u*)start, sequence_num);
	if (v14)
	{
		v10 = sub_A50A0(*(x_DWORD *)(v14 + 4)) + 8;
		v13 = v10 + v14;
		v14 += 12;
		S->TIMB_2 = 0;
		S->RBRN_3 = 0;
		S->EVNT_4 = 0;
		while (v14 < v13)
		{
			if (!strncmp((const char*)v14, "TIMB", 4))
				S->TIMB_2 = v14;
			if (!strncmp((const char*)v14, "RBRN", 4))
				S->RBRN_3 = (Bit8u*)v14;
			if (!strncmp((const char*)v14, "EVNT", 4))
				S->EVNT_4 = v14;
			v14 += sub_A50A0(*(x_DWORD *)(v14 + 4)) + 8;
		}
		if (S->EVNT_4)
		{
			S->ICA_6 = 0;
			S->prefix_callback_7 = 0;
			S->trigger_callback_8_32 = 0;
			S->beat_callback_9 = 0;
			S->EOS_10 = 0;
			S->loop_count_11 = 1;
			sub_A6490(S);
			S->volume_14 = x_DWORD_181DAC[13];
			S->volume_target_15 = x_DWORD_181DAC[13];
			S->volume_period_17 = 0;
			S->volume_accum_16 = 0;
			S->tempo_percent_18 = 100;
			S->tempo_target_19 = 100;
			S->tempo_period_21 = 0;
			S->tempo_accum_20 = 0;
			S->tempo_error_22 = 0;
			if (S->TIMB_2)
			{
				memmove(&unk_181EE0, (void*)S->TIMB_2, 512);
				v9 = (x_WORD*)&unk_181EE0;
				if (S->driver_0->var105_aildrv)
				{
					v12 = 0;
					while ((unsigned __int16)v9[4] > v12)
					{
						v7 = v9[v12 + 5] & 0xFF;
						v8 = (v9[v12 + 5] & 0xFF00u) >> 8;
						if ((*(int(**)(_MDI_DRIVER*, unsigned int, int))(S->driver_0->var105_aildrv))(S->driver_0, v8, v7))
						{
							for (i = v12 + 1; (unsigned __int16)v9[4] > i; i++)
								v9[i + 4] = v9[i + 5];
							--v9[4];
							if (*((x_BYTE *)v9 + 5) >= 2u)
							{
								*((x_BYTE *)v9 + 5) -= 2;
							}
							else
							{
								*((x_BYTE *)v9 + 5) -= 2;
								--*((x_BYTE *)v9 + 4);
							}
						}
						else
						{
							v12++;
						}
					}
				}
				if (v9[4])
				{
					if (sub_92160())
					{
						strcpy(x_BYTE_181C90, strlen("No timbres loaded\n") + 1, "No timbres loaded\n");
						v6 = -1;
					}
					else
					{
						S->driver_0->disable_5++;
						sub_A4EB0(S->driver_0);
						memmove((void*)(&S->driver_0->DST_2->MIDI_data), v9, 512);
						sub_91F70_AIL_call_driver(S->driver_0->drvr_0, 1283, 0, &v4);
						S->driver_0->disable_5--;
						if (v4.AX != -1)
						{
							v6 = 1;
						}
						else
						{
							sprintf(x_BYTE_181C90, 512, "Driver could not install timbre bank %u, patch %u\n", v5 >> 8, v5);
							v6 = -1;
						}
					}
				}
				else
				{
					v6 = 1;
				}
			}
			else
			{
				v6 = 1;
			}
		}
		else
		{
			qmemcpy(&x_BYTE_181C90, (void*)"Invalid XMIDI sequence\n", 0x18u);
			v6 = 0;
		}
	}
	else
	{
		qmemcpy(&x_BYTE_181C90, (void*)"Invalid XMIDI sequence\n", 0x18u);
		v6 = 0;
	}
	return v6;
}
// 8E3D5: using guessed type x_DWORD sprintf_s(x_DWORD, const char *, ...);
// 99682: using guessed type x_DWORD strncmp(x_DWORD, x_DWORD, x_DWORD);
// AD81F: using guessed type x_DWORD memmove(x_DWORD, x_DWORD, x_DWORD);
// 181C90: using guessed type char x_BYTE_181C90;
// 181DE0: using guessed type int x_DWORD_181DE0;


void sub_A8010_AIL_API_start_sequence(HSEQUENCE S, Bit32u track)//289010
{
	if (S)
	{
		if (S->status_1 != 1)
		{
			sub_95DE0_AIL_stop_sequence(S);
			SOUND_start_sequence(track - 1);
			S->status_1 = 4;
		}
	}

}

//----- (000A8010) --------------------------------------------------------
void sub_A8010_AIL_API_start_sequence_orig(HSEQUENCE S)//289010
{
	//int *result; // eax

	//fix it
	//result = 0;
	//fix it

	if (S)
	{
		//result = S;
		if (S->status_1 != 1)
		{
			sub_95DE0_AIL_stop_sequence(S);
			sub_A6490(S);
			//result = a1;
			S->status_1 = 4;
		}
	}
	//return result;
}

void sub_A8050_AIL_API_stop_sequence(HSEQUENCE S)//289050
{
	int v2; // [esp+0h] [ebp-Ch]
	signed int i; // [esp+4h] [ebp-8h]
	HMDIDRIVER v4; // [esp+8h] [ebp-4h]

	if (S)
	{
		if (S->status_1 == 4)
		{
			S->status_1 = 8;
			SOUND_pause_sequence(S->sequence_num);
		}
	}
}

//----- (000A8050) --------------------------------------------------------
void sub_A8050_AIL_API_stop_sequence_orig(HSEQUENCE S)//289050
{
	//int result; // eax
	int v2; // [esp+0h] [ebp-Ch]
	signed int i; // [esp+4h] [ebp-8h]
	HMDIDRIVER v4; // [esp+8h] [ebp-4h]

	if (S)
	{
		//result = (int)S;
		if (S->status_1 == 4)
		{
			S->status_1 = 8;
			sub_A5E50(S);
			//result = *a1;
			v4 = (HMDIDRIVER)S->driver_0;
			for (i = 0; i < 16; i++)
			{
				v2 = S->chan_map_37[i];
				if (S->seq_277[i] >= 64)
					sub_A4F10((HMDIDRIVER)v4, v2 | 0xB0, 64, 0);
				if (S->seq_117[i] >= 64)
					v4->DST_2->MIDI_data[v2] = 0;//v4[v2 + 8]
				if (S->seq_149[i] >= 64)
					sub_A4F10((HMDIDRIVER)v4, v2 | 0xB0, 112, 0);
				if (S->seq_101[i] >= 64)
					sub_980D0_AIL_release_channel((int32*)v4, v2 + 1);
				//result = i;
			}
		}
	}
	//return result;
}

void sub_A8180_AIL_API_resume_sequence(HSEQUENCE S)//289180
{
	if (S && S->status_1 == 8)
	{
		S->status_1 = 4;
		SOUND_resume_sequence(S->sequence_num);
	}
}

//----- (000A8180) --------------------------------------------------------
void sub_A8180_AIL_API_resume_sequence_orig(x_DWORD *a1)//289180
{
	signed int v1; // [esp+0h] [ebp-10h]
	int v2; // [esp+4h] [ebp-Ch]
	signed int i; // [esp+8h] [ebp-8h]
	int j; // [esp+8h] [ebp-8h]
	x_DWORD *v5; // [esp+Ch] [ebp-4h]

	if (a1 && a1[1] == 8)
	{
		v5 = (x_DWORD *)*a1;
		for (i = 0; i < 16; i++)
		{
			if (a1[i + 101] >= 64)
			{
				v2 = sub_97F90_AIL_lock_channel(v5) - 1;
				if (v2 == -1)
					v1 = i;
				else
					v1 = v2;
				a1[i + 37] = v1;
			}
		}
		for (j = 0; j < 16; j++)
			sub_A5FD0((int)a1, j);
		a1[1] = 4;
	}
}

void sub_A8250_AIL_API_end_sequence(HSEQUENCE S)//289250
{
	if (S)
	{
		if (S->status_1 != 1)
		{
			sub_95DE0_AIL_stop_sequence(S);
			S->status_1 = 2;
			SOUND_stop_sequence(S->sequence_num);
		}
	}
}

//----- (000A8250) --------------------------------------------------------
void sub_A8250_AIL_API_end_sequence_orig(HSEQUENCE S)//289250
{
	//int result; // eax

	if (S)
	{
		//result = a1;
		if ((int)S->status_1 != 1)
		{
			sub_95DE0_AIL_stop_sequence(S);
			S->status_1 = 2;
			//result = a1;
			if (S->EOS_10)
				/*result =*/ (*(int(**)(HSEQUENCE))(S->EOS_10))(S);
		}
	}
	//return result;
}

//----- (000A82A0) --------------------------------------------------------
int sub_A82A0(int a1, int a2)//2892a0
{
	int result; // eax

	if (a1)
	{
		result = a2;
		*(x_DWORD *)(a1 + 44) = a2;
	}
	return result;
}

//----- (000A82C0) --------------------------------------------------------
void sub_A82C0(x_DWORD *a1, int a2, int a3)//2892c0
{
	if (a1)
	{
		++*(x_DWORD *)(*a1 + 20);
		a1[19] = a2;
		if (a1[18] == a1[19])
		{
			--*(x_DWORD *)(*a1 + 20);
		}
		else
		{
			if (a3)
			{
				a1[21] = 1000 * a3 / abs(a1[18] - a1[19]);
				a1[20] = 0;
			}
			else
			{
				a1[18] = a1[19];
			}
			--*(x_DWORD *)(*a1 + 20);
		}
	}
}
// A0D1F: using guessed type x_DWORD abs(x_DWORD);

//----- (000A8360) --------------------------------------------------------
void sub_A8360_AIL_API_set_sequence_volume(HSEQUENCE S, Bit32s volume, Bit32s milliseconds)//289360
{
	if (S)
	{
		S->driver_0->disable_5++;
		S->volume_target_15 = volume;
		if (S->volume_14 == S->volume_target_15)
		{
			S->driver_0->disable_5--;
		}
		else
		{
			if (milliseconds)
			{
				S->volume_period_17 = 1000 * milliseconds / abs(S->volume_14 - S->volume_target_15);
				S->volume_accum_16 = 0;
			}
			else
			{
				S->volume_14 = S->volume_target_15;
			}
			sub_A64C0(S);
			S->driver_0->disable_5--;
		}
	}
}
// A0D1F: using guessed type x_DWORD abs(x_DWORD);

//----- (000A8410) --------------------------------------------------------
int sub_A8410_AIL_API_sequence_status(HSEQUENCE S)
{
	int v2; // [esp+0h] [ebp-4h]

	if (S)
		v2 = S->status_1;
	else
		v2 = 0;
	return v2;
}

//----- (000A8440) --------------------------------------------------------
signed int sub_A8440(int a1)
{
	signed int v2; // [esp+0h] [ebp-4h]

	if (a1)
		v2 = *(x_DWORD *)(a1 + 44);
	else
		v2 = -1;
	return v2;
}

//----- (000A8470) --------------------------------------------------------
int sub_A8470(int a1)
{
	int v2; // [esp+0h] [ebp-4h]

	if (a1)
		v2 = *(x_DWORD *)(a1 + 72);
	else
		v2 = 0;
	return v2;
}

//----- (000A84A0) --------------------------------------------------------
int sub_A84A0(int a1)
{
	int v2; // [esp+0h] [ebp-4h]

	if (a1)
		v2 = *(x_DWORD *)(a1 + 56);
	else
		v2 = 0;
	return v2;
}

//----- (000A84D0) --------------------------------------------------------
void sub_A84D0_AIL_API_set_XMIDI_master_volume(HMDIDRIVER mdi, Bit32s master_volume)
{
	Bit32s result; // eax
	int v3; // [esp+0h] [ebp-8h]
	HSEQUENCE v4; // [esp+4h] [ebp-4h]

	result = mdi->master_volume;
	if (result != master_volume)
	{
		mdi->master_volume = master_volume;
		mdi->disable_5++;
		v3 = mdi->n_sequences_7;
		v4 = mdi->sequences_6;
		while (v3)
		{
			if (*(x_DWORD *)(v4 + 4) == 4)
				sub_A64C0(v4);
			v3--;
			v4 += 1816;
		}
		mdi->disable_5--;
	}
}

//----- (000A8550) --------------------------------------------------------
int sub_A8550(int a1)
{
	return *(x_DWORD *)(a1 + 432);
}

//----- (000A8570) --------------------------------------------------------
int sub_A8570_sound_proc39(int *a1, __int16 a2, __int16 a3)
{
	VDI_CALL v4; // [esp+0h] [ebp-10h]
	__int16 v5; // [esp+4h] [ebp-Ch]

	v5 = (a2 << 8) | a3;
	return sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1284, &v4, 0);
}

//----- (000A85B0) --------------------------------------------------------
VDI_CALL sub_A85B0_sound_proc40(int *a1, int a2, int a3)
{
	VDI_CALL result; // [esp+0h] [ebp-10h]
	Bit32u v5; // [esp+2h] [ebp-Eh]

	//fix it
	v5 = 0;
	//fix it

	if (a1[105] && ((int(*)(int *, int, int))a1[105])(a1, a2, a3))
	{
		result.AX = 1;
		return result;
	}
	x_BYTE_E4B2A = a3;
	x_BYTE_E4B2B = a2;
	++a1[5];
	sub_A4EB0((HMDIDRIVER)a1);
	memmove((void*)(a1[2] + 256), aTimb_0, 12);
	sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1283, 0, &result);
	--a1[5];
	if (!result.AX)
		sprintf(x_BYTE_181C90, 512, "Driver could not install timbre bank %u, patch %u\n", v5 >> 8, v5);
	return result;
}
// 8E3D5: using guessed type x_DWORD sprintf_s(x_DWORD, const char *, ...);
// AD81F: using guessed type x_DWORD memmove(x_DWORD, x_DWORD, x_DWORD);
// E4B2A: using guessed type char x_BYTE_E4B2A;
// E4B2B: using guessed type char x_BYTE_E4B2B;
// 181C90: using guessed type char x_BYTE_181C90;

//----- (000A8690) --------------------------------------------------------
int sub_A8690_sound_proc41(int *a1, __int16 a2, __int16 a3)
{
	VDI_CALL v4; // [esp+0h] [ebp-Ch]
	__int16 v5; // [esp+4h] [ebp-8h]
	__int16 v6; // [esp+6h] [ebp-6h]

	v5 = (a2 << 8) | a3;
	v6 = 1;
	return sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1285, &v4, 0);
}

//----- (000A86D0) --------------------------------------------------------
int sub_A86D0_sound_proc42(int *a1, __int16 a2, __int16 a3)
{
	VDI_CALL v4; // [esp+0h] [ebp-Ch]
	__int16 v5; // [esp+4h] [ebp-8h]
	__int16 v6; // [esp+6h] [ebp-6h]

	v5 = (a2 << 8) | a3;
	v6 = 0;
	return sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1285, &v4, 0);
}

//----- (000A8710) --------------------------------------------------------
int sub_A8710(int a1)
{
	int v2; // [esp+4h] [ebp-8h]
	int i; // [esp+8h] [ebp-4h]

	v2 = 0;
	for (i = 0; i < *(x_DWORD *)(a1 + 28); i++)
	{
		if (*(x_DWORD *)(1816 * i + *(x_DWORD *)(a1 + 24) + 4) == 4)
			++v2;
	}
	return v2;
}

//----- (000A8770) --------------------------------------------------------
int sub_A8770(int a1, char a2, unsigned int a3)
{
	int v4; // [esp+0h] [ebp-4h]

	if (a1)
		v4 = sub_A5210(a1 + 212, (a2 - 1) | 0xB0, a3);
	else
		v4 = -1;
	return v4;
}

//----- (000A87C0) --------------------------------------------------------
int sub_A87C0(int a1, int a2)
{
	int v4; // [esp+4h] [ebp-8h]
	signed int i; // [esp+8h] [ebp-4h]

	if (!a1)
		return 0;
	v4 = 0;
	for (i = 0; i < 16; i++)
	{
		if (a2 - 1 == *(x_DWORD *)(4 * i + a1 + 1368))
			++v4;
	}
	return v4;
}

//----- (000A8830) --------------------------------------------------------
int sub_A8830(x_DWORD *a1, x_DWORD *a2, x_DWORD *a3)
{
	int result; // eax
	int v4; // [esp+0h] [ebp-10h]
	int i; // [esp+4h] [ebp-Ch]
	int v6; // [esp+8h] [ebp-8h]
	int v7; // [esp+Ch] [ebp-4h]

	if (a1)
	{
		++*(x_DWORD *)(*a1 + 20);
		v7 = a1[23];
		v6 = a1[24];
		v4 = a1[27];
		for (i = 0; i < x_DWORD_181DAC[14]; i++)
		{
			v4 += a1[26];
			if (v4 >= a1[28])
			{
				v4 -= a1[28];
				if (++v7 >= a1[25])
				{
					v7 = 0;
					++v6;
				}
			}
		}
		if (v6 < 0)
			v6 = 0;
		if (a3)
			*a3 = v6;
		if (a2)
			*a2 = v7;
		result = *a1;
		--*(x_DWORD *)(result + 20);
	}
	return result;
}
// 181DE4: using guessed type int x_DWORD_181DE4;

//----- (000A8900) --------------------------------------------------------
x_DWORD *sub_A8900(HSEQUENCE S, int a2)
{
	x_DWORD *result; // eax
	int v3; // [esp+0h] [ebp-Ch]
	signed int i; // [esp+4h] [ebp-8h]
	signed int v5; // [esp+8h] [ebp-4h]
	signed int j; // [esp+8h] [ebp-4h]

	//fix it
	result = 0;
	//fix it

	if (S)
	{
		result = (x_DWORD*)S;
		if (S->RBRN_3)
		{
			v5 = *(signed __int16 *)(&S->RBRN_3[8]);
			v3 = S->RBRN_3[10];
			for (i = 0; i < v5 && *(unsigned __int16 *)(v3 + 6 * i) != a2; i++)
				;
			result = (x_DWORD *)i;
			if (i != v5)
			{
				//S->EVNT_ptr_5 = (int)*(x_DWORD *)(6 * i + v3 + 2) + /*(Bit8u*)&*/S->EVNT_4+8; // fix it
			  //result = S;
				S->interval_count_12 = 0;
				if (!x_DWORD_181DAC[15])
				{
					for (j = 0; j < 4; j++)
					{
						S->FOR_loop_count_33[j] = -1;
						//result = (x_DWORD *)j;
					}
				}
			}
		}
	}
	return result;
}
// 181DE8: using guessed type int x_DWORD_181DE8;

//----- (000A89E0) --------------------------------------------------------
int sub_A89E0(int a1, int a2)
{
	int v2; // ST04_4

	if (!a1)
		return 0;
	v2 = *(x_DWORD *)(a1 + 28);
	*(x_DWORD *)(a1 + 28) = a2;
	return v2;
}

//----- (000A8A20) --------------------------------------------------------
xCALLBACK sub_A8A20_AIL_API_trigger_callback(HSEQUENCE S, xCALLBACK a2)
{
	xCALLBACK v2; // ST04_4

	if (!S)
		return 0;
	v2 = S->trigger_callback_8_32;
	S->trigger_callback_8_32 = a2;
	return v2;
}

//----- (000A8A60) --------------------------------------------------------
int sub_A8A60(int a1, int a2)
{
	int v2; // ST04_4

	if (!a1)
		return 0;
	v2 = *(x_DWORD *)(a1 + 40);
	*(x_DWORD *)(a1 + 40) = a2;
	return v2;
}

//----- (000A8AA0) --------------------------------------------------------
int sub_A8AA0(int a1, int a2)
{
	int v2; // ST04_4

	if (!a1)
		return 0;
	v2 = *(x_DWORD *)(a1 + 36);
	*(x_DWORD *)(a1 + 36) = a2;
	return v2;
}

//----- (000A8AE0) --------------------------------------------------------
int sub_A8AE0(int a1, int a2)
{
	int v2; // ST04_4

	v2 = *(x_DWORD *)(a1 + 416);
	*(x_DWORD *)(a1 + 416) = a2;
	return v2;
}

//----- (000A8B20) --------------------------------------------------------
int sub_A8B20(int a1, int a2)
{
	int v2; // ST04_4

	v2 = *(x_DWORD *)(a1 + 420);
	*(x_DWORD *)(a1 + 420) = a2;
	return v2;
}

//----- (000A8B60) --------------------------------------------------------
int sub_A8B60(int a1, int a2, int a3)
{
	int result; // eax

	if (a1)
	{
		result = a3;
		*(x_DWORD *)(4 * a2 + a1 + 1752) = a3;
	}
	return result;
}

//----- (000A8B90) --------------------------------------------------------
int sub_A8B90(int a1, int a2)
{
	int v3; // [esp+0h] [ebp-4h]

	if (a1)
		v3 = *(x_DWORD *)(a1 + 4 * a2 + 1752);
	else
		v3 = 0;
	return v3;
}

//----- (000A8BD0) --------------------------------------------------------
int sub_A8BD0(int a1, int a2)
{
	int result; // eax

	if (a1)
	{
		result = a2;
		*(x_DWORD *)(a1 + 24) = a2;
	}
	return result;
}

//----- (000A8BF0) --------------------------------------------------------
int sub_A8BF0(x_DWORD *a1)
{
	int v2; // [esp+0h] [ebp-18h]
	int v3; // [esp+4h] [ebp-14h]
	signed int v4; // [esp+8h] [ebp-10h]
	signed int v5; // [esp+Ch] [ebp-Ch]
	signed int k; // [esp+10h] [ebp-8h]
	signed int i; // [esp+14h] [ebp-4h]
	signed int j; // [esp+14h] [ebp-4h]
	int v9; // [esp+14h] [ebp-4h]

	++a1[5];
	v5 = -1;
	v4 = 0x7FFFFFFF;
	for (i = 8; i >= 1; i--)
	{
		if (i != 9 && a1[i + 8] != 1 && a1[i + 8] != 2 && a1[i + 88] < v4)
		{
			v4 = a1[i + 88];
			v5 = i;
		}
	}
	if (v5 == -1)
	{
		for (j = 8; j >= 1; j--)
		{
			if (j != 9 && a1[j + 8] != 1 && a1[j + 88] < v4)
			{
				v4 = a1[j + 88];
				v5 = j;
			}
		}
	}
	if (v5 == -1)
	{
		--a1[5];
		v2 = 0;
	}
	else
	{
		sub_A4F10((HMDIDRIVER)a1, v5 | 0xB0, 64, 0);
		v9 = a1[7];
		v3 = a1[6];
		while (v9)
		{
			if (*(x_DWORD *)(v3 + 4) != 1)
			{
				for (k = 0; k < 32; k++)
				{
					if (*(x_DWORD *)(v3 + 4 * k + 1368) != -1 && *(x_DWORD *)(v3 + 4 * *(x_DWORD *)(v3 + 4 * k + 1368) + 148) == v5)
					{
						sub_A5850((HSEQUENCE)v3, *(unsigned int *)(v3 + 4 * k + 1368) | 0x80, *(x_DWORD *)(v3 + 4 * k + 1496), 0, 0);
						*(x_DWORD *)(v3 + 4 * k + 1368) = -1;
					}
				}
			}
			--v9;
			v3 += 1816;
		}
		a1[v5 + 72] = a1[v5 + 8];
		a1[v5 + 8] = 1;
		a1[v5 + 24] = 0;
		a1[v5 + 40] = a1[v5 + 56];
		--a1[5];
		v2 = v5 + 1;
	}
	return v2;
}

//----- (000A8EA0) --------------------------------------------------------
int sub_A8EA0(x_DWORD *a1, int a2)
{
	int result; // eax
	int v3; // [esp+0h] [ebp-10h]
	int v4; // [esp+4h] [ebp-Ch]
	signed int i; // [esp+8h] [ebp-8h]
	int v6; // [esp+Ch] [ebp-4h]

	v4 = a2 - 1;
	result = (int)&a1[a2 - 1];
	if (*(x_DWORD *)(result + 32) == 1)
	{
		++a1[5];
		a1[v4 + 8] = a1[v4 + 72];
		a1[v4 + 56] = a1[v4 + 40];
		sub_A4F10((HMDIDRIVER)a1, v4 | 0xB0, 64, 0);
		v6 = a1[7];
		v3 = a1[6];
		while (v6)
		{
			if (*(x_DWORD *)(v3 + 4) != 1)
			{
				for (i = 0; i < 32; i++)
				{
					if (*(x_DWORD *)(v3 + 4 * i + 1368) != -1 && *(x_DWORD *)(v3 + 4 * *(x_DWORD *)(v3 + 4 * i + 1368) + 148) == v4)
					{
						sub_A5850((HSEQUENCE)v3, *(unsigned int *)(v3 + 4 * i + 1368) | 0x80, *(x_DWORD *)(v3 + 4 * i + 1496), 0, 0);
						*(x_DWORD *)(v3 + 4 * i + 1368) = -1;
					}
				}
			}
			--v6;
			v3 += 1816;
		}
		if (a1[v4 + 40])
		{
			if (*(x_DWORD *)(a1[v4 + 40] + 4) != 1)
				sub_A5FD0(a1[v4 + 40], v4);
		}
		result = (int)a1;
		--*(x_DWORD *)(result + 20);
	}
	return result;
}

//----- (000A9080) --------------------------------------------------------
int sub_A9080(x_DWORD *a1, int a2, int a3)
{
	int result; // eax

	if (a1)
	{
		a1[a2 + 36] = a3 - 1;
		result = 4 * a3 + *a1;
		if (*(x_DWORD *)(result + 28) == 1)
		{
			result = *(x_DWORD *)(4 * a3 + *a1 + 92);
			if ((x_DWORD *)result != a1)
			{
				result = (int)a1;
				*(x_DWORD *)(*a1 + 4 * a3 + 92) = (x_DWORD)a1;
			}
		}
	}
	return result;
}

//----- (000A9100) --------------------------------------------------------
int sub_A9100(int a1, int a2)
{
	int v3; // [esp+0h] [ebp-4h]

	if (a1)
		v3 = *(x_DWORD *)(a1 + 4 * a2 + 144) + 1;
	else
		v3 = 0;
	return v3;
}

void sub_A9140_AIL_API_send_channel_voice_message(HMDIDRIVER mdi, HSEQUENCE S, Bit32s status, Bit32s data_1, Bit32s data_2)//28a140
{
	//test_midi_play((Bit8u*)mdi->DST_2->MIDI_data);
}

//----- (000A9140) --------------------------------------------------------
void sub_A9140_AIL_API_send_channel_voice_message_orig(HMDIDRIVER mdi, HSEQUENCE S, Bit32s status, Bit32s data_1, Bit32s data_2)//28a140
{
	//x_DWORD *result; // eax

	mdi->disable_5++;
	if (S)
	{
		sub_A5850(S, status, data_1, data_2, 0);
	}
	else
	{
		if ((status & 0xF0) == 176 && data_1 == 7)
		{
			data_2 = mdi->master_volume * data_2 / 127;
			if (data_2 > 127)
				data_2 = 127;
			if (data_2 < 0)
				data_2 = 0;
		}
		if (!mdi->event_trap || !((int(*)(HMDIDRIVER, x_DWORD, int, unsigned int, int))mdi->event_trap)(mdi, 0, status, data_1, data_2))
			sub_A4F10(mdi, status, data_1, data_2);
	}
	sub_A4EB0(mdi);
	mdi->disable_5--;
}



//----- (000B096C) --------------------------------------------------------
int sub_B096C(int *a1)
{
	int *v1; // ebp
	int *v2; // ecx
	x_DWORD *v3; // edi
	signed int v4; // eax
	unsigned __int64 v5; // rax
	signed int v6; // eax
	char *v7; // edx
	unsigned int v8; // eax
	int v9; // ebx
	x_DWORD *v10; // edi
	int v11; // ebx
	unsigned int v12; // esi
	int v13; // ebx
	int result; // eax
	int v15; // ebx
	int *v16; // [esp-Ch] [ebp-18h]
	int v17; // [esp-8h] [ebp-14h]
	int v18; // [esp-4h] [ebp-10h]
	int savedregs; // [esp+Ch] [ebp+0h]

	v1 = &savedregs;
	v18 = (unsigned __int16)__DS__;
	v17 = (unsigned __int16)__ES__;
	v2 = a1;
	v3 = (x_DWORD *)*a1;
	v4 = 0;
	if (*(x_DWORD *)(*a1 + 24) == 2 || v3[6] == 3)
		v4 = 1;
	if (a1[13] == 2 || a1[13] == 3)
	{
		v4 |= 2u;
		if (a1[14] & 2)
			v4 |= 4u;
	}
	if (a1[13] == 1 || a1[13] == 3)
		v4 |= 8u;
	if ((!(v4 & 2) || v4 & 1) && a1[16] == 127 && v3[26] == 127 && a1[17] == 64 || (v4 |= 0x40u, v4 & 8))
	{
		if (a1[14] & 1)
			v4 |= 0x10u;
	}
	x_DWORD_E4E1C = v4;
	LODWORD(v5) = 0;
	HIDWORD(v5) = a1[15];
	if (SHIDWORD(v5) <= 0)
		HIDWORD(v5) = 1;
	v6 = v5 / (v3[5] << 16);
	v7 = (char *)(v6 - 0x10000);
	if (v6 <= 0x10000)
		v7 = (char*)0x10000 - v6;
	if ((int)v7 > x_DWORD_181DAC[6])
	{
		LODWORD(qx_WORD_E4E30) = v6 << 16;
		v8 = (unsigned int)v6 >> 16;
		v9 = v8 + 1;
		if (x_DWORD_E4E1C & 2)
		{
			v8 *= 2;
			v9 *= 2;
		}
		if (x_DWORD_E4E1C & 8)
		{
			v8 *= 2;
			v9 *= 2;
		}
		HIDWORD(qx_WORD_E4E30) = v8;
		x_DWORD_E4E38 = v9;
		x_DWORD_E4E1C |= 0x20u;
	}
	x_DWORD_E4E28 = v3[20];
	x_DWORD_E4E2C = v3[19] + x_DWORD_E4E28;
	v10 = (x_DWORD *)x_DWORD_E4E28;
	do
	{
		v11 = v2[10];
		x_DWORD_E4E20 = v2[v11 + 6] + v2[v11 + 2];
		x_DWORD_E4E24 = v2[v11 + 4] + v2[v11 + 2];
		if (v2[530])
		{
			v16 = v2;
			((void(__stdcall *)(int *))v2[530])(v2);
		}
		v12 = x_DWORD_E4E20;
		v16 = v1;
		((void(*)(int, x_DWORD *, x_BYTE *))off_AED40[x_DWORD_E4E1C])(
			0,
			v10,
			(x_BYTE *)x_DWORD_E4E20);
		v1 = v16;
		v2 = (int *)v16[2];
		v13 = v2[10];
		result = v12 - v2[v13 + 2];
		v2[v13 + 6] = result;
		if (v12 < x_DWORD_E4E24)
			break;
		if (v2[531])
		{
			v16 = v2;
			result = ((int(__stdcall *)(int *))v2[531])(v2);
			v2 = v16;
		}
		if (v2[12])
		{
			if (v2[12] == 1)
			{
				v15 = v2[10] ^ 1;
				if (v2[v15 + 8])
				{
					v2[1] = 2;
					if (v2[532])
					{
						v16 = v2;
						result = ((int(__stdcall *)(int *))v2[532])(v2);
					}
					return result;
				}
				if (!v2[v15 + 4] || v2[v15 + 6])
					return result;
				v2[10] = v15;
			}
			else
			{
				--v2[12];
			}
		}
		v2[v2[10] + 6] = 0;
	} while ((unsigned int)v10 < x_DWORD_E4E2C);
	return result;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// AED40: using guessed type int (*off_AED40[2])();
// E4E1C: using guessed type int x_DWORD_E4E1C;
// E4E20: using guessed type int x_DWORD_E4E20;
// E4E24: using guessed type int x_DWORD_E4E24;
// E4E28: using guessed type int x_DWORD_E4E28;
// E4E2C: using guessed type int x_DWORD_E4E2C;
// E4E30: using guessed type __int64 qx_WORD_E4E30;
// E4E38: using guessed type int x_DWORD_E4E38;
// 181DC4: using guessed type int x_DWORD_181DC4;

//----- (000B0B87) --------------------------------------------------------
int sub_B0B87(HDIGDRIVER a1, int a2)
{
	int v2; // eax
	int *v3; // esi
	int v4; // ecx
	Bit8u* v5; // edi
	int v6; // eax

	//fix
	v5 = 0;
	//fix

	v2 = a1->hw_mode_flags_7 & 0xF;
	if (a1->hw_format_6 == 2 || a1->hw_format_6 == 3)
		v2 |= 0x10u;
	if (a1->hw_format_6 == 1 || a1->hw_format_6 == 3)
		v2 |= 0x20u;
	if (a1->n_active_samples_25 > 1u)
		v2 |= 0x40u;
	x_DWORD_E4E1C = v2;
	v3 = (int *)a1->build_buffer_20;
	v4 = a1->samples_per_buffer_18;
	if (a1->hw_mode_flags_7 & 0x20)
	{
		x_DWORD_E4E1C |= 8u;
		v5 = (Bit8u*)(a2 * (a1->half_buffer_size_4 >> 1) + (int)a1->DMA_11_12[0]);//fix
	}
	else
	{
		//v5 = (x_BYTE *)a1[a2 + 11];//fix it!
		v5 = (Bit8u*)a1->DMA_11_12[a2];//fix it

		if (a1->hw_mode_flags_7 & 8)
			v6 = a1->half_buffer_size_4;
	}
	return ((int(*)(int, x_BYTE *, int *))off_AEB40[x_DWORD_E4E1C])(v4, (int8*)v5, v3);
}
// AEB40: using guessed type int (*off_AEB40[2])();
// E4E1C: using guessed type int x_DWORD_E4E1C;

//----- (000B0C1A) --------------------------------------------------------
void sub_B0C1A(HDIGDRIVER a1)
{
	/*Bit8u* v1; // edi
	unsigned int v3; // ST00_4

	v1 = (Bit8u*)a1->build_buffer_20;
	v3 = a1->build_size_19;
	memset(v1, 0, a1->build_size_19 & 3);
	memset(&v1[v3 & 3], 0, 4 * (v3 >> 2));*/
	//fix it
}

//----- (000AEF40) --------------------------------------------------------
int sub_AEF40(int a1, x_BYTE *a2, int *a3)
{
	int v3; // eax
	int result; // eax

	do
	{
		v3 = *a3;
		++a3;
		result = v3 ^ 0x8000;
		*a2++ = BYTE1(result);
		--a1;
	} while (a1);
	return result;
}

//----- (000AEF51) --------------------------------------------------------
int sub_AEF51(int a1, x_BYTE *a2, int *a3)
{
	int result; // eax

	do
	{
		result = *a3;
		++a3;
		*a2++ = BYTE1(result);
		--a1;
	} while (a1);
	return result;
}

//----- (000AFB7E) --------------------------------------------------------
int sub_AFB7E(int result, x_DWORD *a2, x_BYTE *a3)
{
	do
	{
		if ((unsigned int)a3 >= x_DWORD_E4E24)
			break;
		BYTE1(result) = *a3;
		result ^= 0x8000u;
		++a3;
		*a2 += (signed __int16)result;
		++a2;
	} while ((unsigned int)a2 < x_DWORD_E4E2C);
	return result;
}
// E4E24: using guessed type int x_DWORD_E4E24;
// E4E2C: using guessed type int x_DWORD_E4E2C;

//----- (000AFB9F) --------------------------------------------------------
int sub_AFB9F(int result, x_DWORD *a2, x_BYTE *a3)
{
	do
	{
		if ((unsigned int)a3 >= x_DWORD_E4E24)
			break;
		BYTE1(result) = *a3;
		result ^= 0x8000u;
		++a3;
		*a2 += (signed __int16)result;
		a2[1] += (signed __int16)result;
		a2 += 2;
	} while ((unsigned int)a2 < x_DWORD_E4E2C);
	return result;
}
// E4E24: using guessed type int x_DWORD_E4E24;
// E4E2C: using guessed type int x_DWORD_E4E2C;

//----- (000A9280) --------------------------------------------------------
bool sub_A9280()
{
	bool result; // eax

	if (x_DWORD_E4B1C)
	{
		sub_9D650((unsigned int)sub_A4CF0, (unsigned int)sub_A9280);
		sub_9D740((unsigned int)aSample, 128);
		sub_9D740((unsigned int)&x_DWORD_E4B18, 4);
		sub_9D740((unsigned int)&x_DWORD_181EB8, 4);
		sub_9D740((unsigned int)&x_DWORD_181EBC, 4);
		sub_9D740((unsigned int)&x_DWORD_181EC4, 4);
		sub_9D740((unsigned int)&x_DWORD_181ED4, 4);
		sub_9D740((unsigned int)&x_DWORD_181EC8, 4);
		sub_9D740((unsigned int)&x_DWORD_181ECC, 4);
		sub_9D740((unsigned int)&x_DWORD_181EB4, 4);
		sub_9D740((unsigned int)&x_DWORD_181ED0, 4);
		sub_9D740((unsigned int)&x_DWORD_181EDC, 4);
		sub_9D740((unsigned int)&x_DWORD_181ED8, 4);
		result = sub_9D740((unsigned int)&x_DWORD_181EB0, 4);
		x_DWORD_E4B1C = 0;
	}
	return result;
}
// E4B18: using guessed type int x_DWORD_E4B18;
// E4B1C: using guessed type int x_DWORD_E4B1C;
// 181EB0: using guessed type int x_DWORD_181EB0;
// 181EB4: using guessed type int x_DWORD_181EB4;
// 181EB8: using guessed type int x_DWORD_181EB8;
// 181EBC: using guessed type int x_DWORD_181EBC;
// 181EC4: using guessed type int x_DWORD_181EC4;
// 181EC8: using guessed type int x_DWORD_181EC8;
// 181ECC: using guessed type int x_DWORD_181ECC;
// 181ED0: using guessed type int x_DWORD_181ED0;
// 181ED4: using guessed type int x_DWORD_181ED4;
// 181ED8: using guessed type int x_DWORD_181ED8;
// 181EDC: using guessed type int x_DWORD_181EDC;

//----- (0009F280) --------------------------------------------------------
int sub_9F280(int *a1)//a1 bude nejaky driver
{
	VDI_CALL v2; // [esp+0h] [ebp-10h]
	__int16 v3; // [esp+4h] [ebp-Ch]
	__int16 v4; // [esp+6h] [ebp-Ah]
	__int16 v5; // [esp+8h] [ebp-8h]
	int v6; // [esp+Ch] [ebp-4h]

	//fix it
	v4 = 0;
	v5 = 0;
	//fix it

	v3 = 16;
	sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1286, &v2, &v2);
	if (v2.AX == 0)//fix
		v6 = -1;
	else
		v6 = v4 << (v5 + 16);
	return v6;
}

//----- (0009F2E0) --------------------------------------------------------
VDI_CALL sub_9F2E0(int *a1, int a2, unsigned __int16 a3, unsigned __int16 a4)
{
	signed __int64 v4; // rax
	unsigned __int16 v5; // ax
	VDI_CALL v7; // [esp+0h] [ebp-10h]
	__int16 v8; // [esp+4h] [ebp-Ch]
	__int16 v9; // [esp+6h] [ebp-Ah]
	__int16 v10; // [esp+8h] [ebp-8h]

	if (!*(x_DWORD *)x_BYTE_E3E54 && !*(x_WORD *)&x_BYTE_E3E54[4])
	{
		v4 = sub_9F110(4);
		*(x_WORD *)&x_BYTE_E3E54[4] = WORD2(v4);
		*(x_DWORD *)x_BYTE_E3E54 = v4;
		if (!(x_DWORD)v4 && !*(x_WORD *)&x_BYTE_E3E54[4])
		{
			v7.AX = -1;
			return v7;
		}
	}
	__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(*(unsigned int *)x_BYTE_E3E54, a4);
	v5 = sub_9F220(a3);
	__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(*(x_DWORD *)x_BYTE_E3E54 + 2, v5);
	v8 = 17;
	v9 = sub_9F220(*(unsigned __int16 *)&x_BYTE_E3E54[4]);
	v10 = 0;
	sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1286, &v7, &v7);
	return v7;
}

//----- (0009F3D0) --------------------------------------------------------
__int64 sub_9F3D0(int *a1, int a2, unsigned __int16 a3, unsigned __int16 a4)
{
	signed __int64 v4; // rax
	unsigned __int16 v5; // ax
	int v6; // ebx
	VDI_CALL v8; // [esp+0h] [ebp-14h]
	__int16 v9; // [esp+4h] [ebp-10h]
	__int16 v10; // [esp+6h] [ebp-Eh]
	__int16 v11; // [esp+8h] [ebp-Ch]
	__int64 v12; // [esp+Ch] [ebp-8h]

	if (*(x_DWORD *)x_BYTE_E3E54
		|| *(x_WORD *)&x_BYTE_E3E54[4]
		|| (v4 = sub_9F110(4), *(x_WORD *)&x_BYTE_E3E54[4] = WORD2(v4), (*(x_DWORD *)x_BYTE_E3E54 = v4) != 0)
		|| *(x_WORD *)&x_BYTE_E3E54[4])
	{
		__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
		__writegsx_WORD(*(unsigned int *)x_BYTE_E3E54, a4);
		v5 = sub_9F220(a3);
		__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
		__writegsx_WORD(*(x_DWORD *)x_BYTE_E3E54 + 2, v5);
		v9 = 23;
		v10 = sub_9F220(*(unsigned __int16 *)&x_BYTE_E3E54[4]);
		v11 = 0;
		sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1286, &v8, &v8);
		if (v8.AX != NULL)
		{
			WORD2(v12) = 0;
			LODWORD(v12) = 0;
		}
		else
		{
			v6 = v11;
			WORD2(v12) = sub_9F1D0(v10);
			LODWORD(v12) = v6;
		}
	}
	else
	{
		WORD2(v12) = 0;
		LODWORD(v12) = 0;
	}
	return v12;
}

//----- (0009F4F0) --------------------------------------------------------
VDI_CALL sub_9F4F0(int *a1, int a2, unsigned __int16 a3, unsigned __int16 a4)
{
	signed __int64 v4; // rax
	unsigned __int16 v5; // ax
	VDI_CALL result; // [esp+0h] [ebp-10h]
	__int16 v8; // [esp+4h] [ebp-Ch]
	__int16 v9; // [esp+6h] [ebp-Ah]
	__int16 v10; // [esp+8h] [ebp-8h]

	if (!*(x_DWORD *)x_BYTE_E3E54 && !*(x_WORD *)&x_BYTE_E3E54[4])
	{
		v4 = sub_9F110(4);
		*(x_WORD *)&x_BYTE_E3E54[4] = WORD2(v4);
		*(x_DWORD *)x_BYTE_E3E54 = v4;
		if (!(x_DWORD)v4 && !*(x_WORD *)&x_BYTE_E3E54[4])
		{
			result.AX = -1;
			return result;
		}
	}
	__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(*(unsigned int *)x_BYTE_E3E54, a4);
	v5 = sub_9F220(a3);
	__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(*(x_DWORD *)x_BYTE_E3E54 + 2, v5);
	v8 = 24;
	v9 = sub_9F220(*(unsigned __int16 *)&x_BYTE_E3E54[4]);
	v10 = 0;
	sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1286, &result, &result);
	return result;
}

//----- (0009F5E0) --------------------------------------------------------
VDI_CALL sub_9F5E0(int *a1, int a2, unsigned __int16 a3, unsigned __int16 a4)
{
	signed __int64 v4; // rax
	unsigned __int16 v5; // ax
	VDI_CALL result; // [esp+0h] [ebp-10h]
	__int16 v8; // [esp+4h] [ebp-Ch]
	__int16 v9; // [esp+6h] [ebp-Ah]
	__int16 v10; // [esp+8h] [ebp-8h]

	if (!*(x_DWORD *)x_BYTE_E3E54 && !*(x_WORD *)&x_BYTE_E3E54[4])
	{
		v4 = sub_9F110(4);
		*(x_WORD *)&x_BYTE_E3E54[4] = WORD2(v4);
		*(x_DWORD *)x_BYTE_E3E54 = v4;
		if (!(x_DWORD)v4 && !*(x_WORD *)&x_BYTE_E3E54[4])
		{
			result.AX = -1;
			return result;
		}
	}
	__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(*(unsigned int *)x_BYTE_E3E54, a4);
	v5 = sub_9F220(a3);
	__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(*(x_DWORD *)x_BYTE_E3E54 + 2, v5);
	v8 = 25;
	v9 = sub_9F220(*(unsigned __int16 *)&x_BYTE_E3E54[4]);
	v10 = 0;
	sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1286, &result, &result);
	return result;
}

//----- (0009F6D0) --------------------------------------------------------
VDI_CALL sub_9F6D0(int *a1, __int16 a2)
{
	VDI_CALL result; // [esp+0h] [ebp-10h]
	__int16 v4; // [esp+4h] [ebp-Ch]
	__int16 v5; // [esp+6h] [ebp-Ah]

	sub_9F170(*(int *)x_BYTE_E3E54, *(unsigned __int16 *)&x_BYTE_E3E54[4]);
	*(x_WORD *)&x_BYTE_E3E54[4] = 0;
	*(x_DWORD *)x_BYTE_E3E54 = 0;
	v4 = 18;
	v5 = a2;
	sub_91F70_AIL_call_driver((AIL_DRIVER*)*a1, 1286, &result, &result);
	return result;
}

//----- (0009F110) --------------------------------------------------------
signed __int64 sub_9F110(int a1)
{
	int v2; // [esp+0h] [ebp-24h]
	int v3; // [esp+4h] [ebp-20h]
	int v4; // [esp+Ch] [ebp-18h]
	int v5; // [esp+18h] [ebp-Ch]
	unsigned int v6; // [esp+20h] [ebp-4h]

	//fix it
	v4 = 0;
	v5 = 0;
	//fix it

	v2 = 256;
	v3 = (a1 + 15) >> 4;
	//int386(49, (REGS*)&v2, (REGS*)&v2);
	if (v5)
		LOWORD(v6) = 0;
	else
		LOWORD(v6) = v4;
	return __PAIR__(v6, 0);
}
// 98D52: using guessed type x_DWORD int386(x_DWORD, x_DWORD, x_DWORD);

void __writegsx_WORD(unsigned long Offset, unsigned short Data) { stub_fix_it(); };

//----- (0009F220) --------------------------------------------------------
int sub_9F220(int a1)
{
	int v2; // [esp+0h] [ebp-20h]
	int v3; // [esp+4h] [ebp-1Ch]
	unsigned __int16 v4; // [esp+8h] [ebp-18h]
	unsigned __int16 v5; // [esp+Ch] [ebp-14h]
	int v6; // [esp+18h] [ebp-8h]
	int v7; // [esp+1Ch] [ebp-4h]

	//fix it
	v4 = 0;
	//fix it

	//fix it
	v5 = 0;
	v6 = 0;
	//fix it

	v2 = 6;
	v3 = a1;
	//int386(49, (REGS*)&v2, (REGS*)&v2);
	if (v6)
		v7 = -1;
	else
		v7 = ((signed int)v5 >> 4) + (v4 << 12);
	return v7;
}
// 98D52: using guessed type x_DWORD int386(x_DWORD, x_DWORD, x_DWORD);

//----- (0009F1D0) --------------------------------------------------------
int sub_9F1D0(int a1)
{
	int v2; // [esp+0h] [ebp-20h]
	int v3; // [esp+4h] [ebp-1Ch]
	int v4; // [esp+18h] [ebp-8h]
	int v5; // [esp+1Ch] [ebp-4h]

	//fix it
	v4 = 0;
	//fix it

	v2 = 2;
	v3 = a1;
	//int386(49, (REGS*)&v2, (REGS*)&v2);
	if (v4)
		v5 = -1;
	else
		v5 = (unsigned __int16)v2;
	return v5;
}
// 98D52: using guessed type x_DWORD int386(x_DWORD, x_DWORD, x_DWORD);

//----- (0009F170) --------------------------------------------------------
int sub_9F170(int a1, unsigned __int16 a2)
{
	int v3; // [esp+0h] [ebp-20h]
	int v4; // [esp+Ch] [ebp-14h]
	int v5; // [esp+18h] [ebp-8h]
	int v6; // [esp+1Ch] [ebp-4h]

	//fix it
	v5 = 0;
	//fix it

	v3 = 257;
	v4 = a2;
	//int386(49, (REGS*)&v3, (REGS*)&v3);
	if (v5)
		v6 = -1;
	else
		v6 = 0;
	return v6;
}
// 98D52: using guessed type x_DWORD int386(x_DWORD, x_DWORD, x_DWORD);

int sub_AEF40() { stub_fix_it(); return 0; };
int sub_AEF51() { stub_fix_it(); return 0; };

int sub_AFB7E() { stub_fix_it(); return 0; };
int sub_AFB9F() { stub_fix_it(); return 0; };


