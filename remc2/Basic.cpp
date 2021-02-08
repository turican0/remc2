#include "Basic.h"

char gameDataPath[MAX_PATH];
char cdDataPath[MAX_PATH];
char bigGraphicsPath[MAX_PATH];

uint8_t x_DWORD_17ECA0[4608]; // weak

int x_DWORD_E3E2C = 0; // weak

//inputs
__int16 x_WORD_180744_mouse_right_button; // weak//351744
__int16 x_WORD_180746_mouse_left_button; // weak//351746

//----- (00083E80) --------------------------------------------------------
void sub_83E80_freemem4(uint8_t* a1)//264e80
{
	uint8_t* v1; // eax
	char v2; // bl
	int* i; // ebx

	if (a1)
	{
		v1 = (uint8_t*)&x_DWORD_17ECA0;
		v2 = 0;
		while (v1)
		{
			if ((int)a1 == v1[0])//fix
			{
				v2 = 1;
				*((x_BYTE*)v1 + 16) = 0;
				break;
			}
			v1 = (uint8_t*)(int*)v1[2];
		}
		if (v2 == 1)
		{
			for (i = (int*)&x_DWORD_17ECA0; i; i = (int*)i[2])
			{
				if (!*((x_BYTE*)i + 16))
					sub_84000((int)i);
			}
		}
		//sub_85350(); //fix
	}
}
// 17ECA0: using guessed type int x_DWORD_17ECA0;

//----- (00084000) --------------------------------------------------------
int sub_84000(int a1)//265000
{
	int result; // eax
	int v2; // edx
	int v3; // ebx

	result = a1;
	if (*(x_DWORD*)(a1 + 12))
	{
		*(x_BYTE*)(a1 + 16) = 0;
		v2 = *(x_DWORD*)(a1 + 12);
		if (*(x_BYTE*)(a1 + 17) == *(x_BYTE*)(v2 + 17) && !*(x_BYTE*)(v2 + 16))
		{
			v3 = *(x_DWORD*)(a1 + 8);
			if (v3)
				*(x_DWORD*)(v3 + 12) = v2;
			*(x_DWORD*)(*(x_DWORD*)(a1 + 12) + 8) = *(x_DWORD*)(a1 + 8);
			*(x_DWORD*)(*(x_DWORD*)(a1 + 12) + 4) += *(x_DWORD*)(a1 + 4);
			*(x_DWORD*)(a1 + 4) = 0;
		}
	}
	return result;
}

x_DWORD x_outp(x_DWORD, char) { stub_fix_it(); return 0; };// weak
x_DWORD x_inp(x_DWORD) { stub_fix_it(); return 0; };// weak
void stub_fix_it() { allert_error(); };

void dbgfprintf(FILE* file, const char* format, ...) {
	//void dbgfprintf(FILE* file,char* str) {
	fprintf(file, format);
	fprintf(file, "\n");
}

void* sub_83CD0_malloc2(size_t a1)//264cd0
{
	return malloc(a1);
}

void qmemcpy(void* a, void* b, size_t c) {
	memcpy(a, b, c);
};

//----- (0009D490) --------------------------------------------------------
int sub_9D490_free4(void* a1, int a2)//27e490
{
	int result; // eax

	if (a1)
	{
		//result = off_E3E34_freex(a1);
		result = x_free(a1);
	}
	return result;
	//fix it
	//return 0;
}
// E3E34: using guessed type int (*off_E3E34_freex)(int);

int x_free(void* ptr) { free(ptr); return 0; };

void __writegsx_WORD(unsigned long Offset, unsigned short Data) { stub_fix_it(); };
void __writegsx_DWORD(unsigned long Offset, unsigned long Data) { stub_fix_it(); };
unsigned long __readgsx_DWORD(unsigned long Offset) { stub_fix_it(); return 0; };
unsigned char __readgsx_BYTE(unsigned long Offset) { stub_fix_it(); return 0; };
unsigned short __readgsx_WORD(unsigned long Offset) { stub_fix_it(); return 0; };
unsigned     int x__readeflags(void) { stub_fix_it(); return 0; };
void x__writeeflags(unsigned Value) { stub_fix_it(); };
unsigned int x__getcallerseflags(void) { stub_fix_it(); return 0; };

x_DWORD dos_read(FILE*, char, x_DWORD, x_DWORD, x_DWORD) { stub_fix_it(); return 0; };// weak

//----- (0009D770) --------------------------------------------------------
int sub_9D770(char* a1, char a2)//27e770
{
	char v3; // [esp+0h] [ebp-E0h]
	int v4; // [esp+40h] [ebp-A0h]
	unsigned int v5; // [esp+44h] [ebp-9Ch]
	int v6; // [esp+ACh] [ebp-34h]
	int v7; // [esp+C4h] [ebp-1Ch]
	int v8; // [esp+C8h] [ebp-18h]
	unsigned int i; // [esp+CCh] [ebp-14h]
	FILE* v10; // [esp+D0h] [ebp-10h]
	int v11; // [esp+D4h] [ebp-Ch]
	int v12; // [esp+D8h] [ebp-8h]
	int v13; // [esp+DCh] [ebp-4h]

	//fix it
	v4 = 0;
	v5 = 0;
	//fix it

	v13 = 8224;
	v7 = 0;
	if (a2 & 1)
	{
		v10 = (FILE*)a1;
	}
	else
	{
		v10 = x_open(a1, 512);
		if (v10 == NULL)
			return 0;
	}
	v12 = sub_9D380(v10, 60, a2, (int)&v8, 4);
	v12 = sub_9D380(v10, v8, a2, (int)&v13, 2);
	if (!strcmp((const char*)&v13, "LX"))
	{
		sub_9D380(v10, v8, a2, (int)&v3, 172);
		v12 = v4 + v8;
		for (i = 0; i < v5; i++)
		{
			v12 = sub_9D380(v10, v12, a2, (int)&v6, 24);
			v7 += v6;
		}
		if (!(a2 & 1))
			DataFileIO::Close((FILE*)v10);
		v7 += 15 * v5;
		v11 = v7;
	}
	else
	{
		DataFileIO::Close((FILE*)v10);
		v11 = 0;
	}
	return v11;
}
// 9A050: using guessed type x_DWORD strcmp(x_DWORD, x_DWORD);
// A0855: using guessed type x_DWORD close(x_DWORD);

//----- (0009DE20) --------------------------------------------------------
signed int sub_9DE20_get_file_lenght(char* a1)//27ee20
{
	signed int v2; // [esp+0h] [ebp-Ch]
	signed int v3; // [esp+4h] [ebp-8h]
	FILE* v4; // [esp+8h] [ebp-4h]

	x_DWORD_E3E2C = 0;
	v4 = x_open(a1, 512);
	if (v4 == NULL)
	{
		x_DWORD_E3E2C = 3;
		v2 = -1;
	}
	else
	{
		v3 = DataFileIO::FileLengthBytes(v4);
		if (v3 == -1)
			x_DWORD_E3E2C = 5;
		DataFileIO::Close(v4);
		v2 = v3;
	}
	return v2;
}
// 988DA: using guessed type x_DWORD filelength(x_DWORD);
// A0855: using guessed type x_DWORD close(x_DWORD);
// E3E2C: using guessed type int x_DWORD_E3E2C;

FILE* x_open(char* path, int pmodex) {
	return myopent(path, (char*)"rb");
};

//----- (0009D380) --------------------------------------------------------
int sub_9D380(FILE* a1, int a2, char a3, int a4, int a5)//27e380
{
	if (a3 & 1)
	{
		memcpy((void*)a4, (const void*)(a2 + a1), a5);
	}
	else
	{
		DataFileIO::Seek(a1, a2, 0);
		DataFileIO::Read(a1, (uint8_t*)a4, a5);
	}
	return a5 + a2;
}
// 99DBD: using guessed type x_DWORD memcpy(x_DWORD, x_DWORD, x_DWORD);
// A0863: using guessed type x_DWORD read(x_DWORD, x_DWORD, x_DWORD);
// AA7C0: using guessed type x_DWORD lseek(x_DWORD, x_DWORD, char);

char x_toupper(char inputchar) {
	return toupper(inputchar);
}; //weak

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
	memset((void*)*v3, 0, v4);//problÃ©m
	return (void*)*v3;*/
	return malloc(a1);
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 17E0A4: using guessed type int x_DWORD_17E0A4[];
// 17ECA0: using guessed type int x_DWORD_17ECA0;

x_DWORD x_tolower(x_DWORD) { stub_fix_it(); return 0; };// weak

//basic graphics

char x_BYTE_D41CE = 0; // weak
uint8_t* x_DWORD_F01E4; // weak
uint16_t x_WORD_F01F4; // weak
char x_BYTE_D41C1 = 0; // weak
char* x_DWORD_D41D0 = 0; // weak
__int16 x_WORD_E36D4 = 0; // weak
char x_BYTE_EB3B6; // weak
type_x_DWORD_17DE38str x_DWORD_17DE38str;
uint8_t* x_DWORD_E9C38_smalltit;

void sub_2EC30_clear_img_mem()//20fc30
{
	if (x_BYTE_D41CE)
		memset((void*)x_DWORD_F01E4, 0, x_WORD_F01F4);
}

//----- (0002EB60) --------------------------------------------------------
void sub_2EB60()//20fb60
{
	//int result; // eax

	if (!x_BYTE_D41CE)
	{
		x_BYTE_D41CE = 1;
		x_BYTE_D41C1 = 1;
		x_DWORD_F01E4 = &pdwScreenBuffer[0xfb40];//line 320x201 - after normal image
		x_WORD_F01F4 = 0x3200;
		sub_2EC30_clear_img_mem();
		x_DWORD_D41D0 = 0;
	}
}

//----- (0002EBB0) --------------------------------------------------------
void sub_2EBB0_draw_text_with_border_630x340(char* a1)//20fbb0
{
	if (x_BYTE_D41CE)
	{
		x_DWORD_D41D0 = a1;
		x_WORD_E36D4 = 64;
		pdwScreenBuffer += 0x26C0;
		/*result = */sub_7FCB0_draw_text_with_border(/*64,*/ a1, 0, 630, 340, 5, x_BYTE_EB3B6, 0);
		x_WORD_E36D4 = 0;
		pdwScreenBuffer -= 0x26C0;
	}
	//return result;
}

//----- (0007FCB0) --------------------------------------------------------
int sub_7FCB0_draw_text_with_border(/*int a1,*/ char* a2, int32_t a3, int32_t a4, int a5, uint8_t a6, unsigned __int8 a7, uint32_t a8)//260cb0
{
	int v8; // esi
	signed __int16 j; // di
	uint8_t v10; // eax
	int v11; // ebx
	uint8_t v12; // al
	int v13; // edi
	__int16 v14; // ST0C_2
	int v15; // eax
	int v16; // eax
	signed __int16 i; // di
	uint8_t v18; // eax
	uint8_t* v19; // ecx
	__int16 v20; // di
	uint8_t v21; // eax
	char v22; // al
	char v23; // ah
	char v24; // cl
	__int16 v25; // ax
	int v26; // esi
	unsigned __int8 v27; // al
	int v28; // esi
	__int16 v29; // ST0C_2
	int v30; // eax
	uint8_t* v31; // ecx
	signed __int16 v32; // ax
	__int16 v33; // si
	int v34; // eax
	__int16 v35; // ST0C_2
	__int16 v36; // ST08_2
	signed __int16 v37; // di
	uint8_t* v38; // ecx
	__int16 v39; // si
	unsigned __int16 v40; // ax
	__int16 v41; // di
	int v42; // eax
	int v43; // eax
	int v44; // eax
	unsigned __int8 v45; // al
	int v46; // esi
	__int16 v47; // ST0C_2
	int v48; // edi
	__int16 v49; // ST0C_2
	int v50; // eax
	int v51; // ebx
	unsigned __int8 v52; // al
	__int16 v53; // ST0C_2
	int v54; // ST08_4
	int v55; // eax
	signed __int16 v56; // si
	signed __int16 v57; // si
	x_BYTE* v58; // ecx
	int v59; // eax
	__int16 v60; // ax
	int v61; // edi
	int v62; // eax
	int v63; // ebx
	uint8_t* v64; // ecx
	signed __int16 v65; // di
	__int16 v66; // si
	int v67; // eax
	signed __int16 v68; // si
	__int16 v69; // ST0C_2
	__int16 v70; // ST08_2
	int v71; // eax
	x_BYTE* v72; // ecx
	signed __int16 l; // si
	__int16 v74; // ax
	int v75; // esi
	int v76; // eax
	int v77; // ebx
	uint8_t* v78; // ecx
	signed __int16 v79; // si
	__int16 v80; // ax
	int v81; // eax
	signed __int16 v82; // si
	__int16 v83; // ST08_2
	int v84; // eax
	uint8_t v86; // [esp-4h] [ebp-FCh]
	char* v86b; // [esp-4h] [ebp-FCh]
	char v87[180]; // [esp+0h] [ebp-F8h]
	int v88; // [esp+B4h] [ebp-44h]
	int v89; // [esp+B8h] [ebp-40h]
	int v90; // [esp+BCh] [ebp-3Ch]
	int v91; // [esp+C0h] [ebp-38h]
	//int v92; // [esp+C4h] [ebp-34h]
	int32_t v93; // [esp+C8h] [ebp-30h]
	int32_t v94; // [esp+CCh] [ebp-2Ch]
	int v95; // [esp+D0h] [ebp-28h]
	int v96; // [esp+D4h] [ebp-24h]
	unsigned __int16 v97; // [esp+D8h] [ebp-20h]
	int v98; // [esp+DCh] [ebp-1Ch]
	int v99; // [esp+E0h] [ebp-18h]
	int32_t k; // [esp+E4h] [ebp-14h]
	int32_t v101; // [esp+E8h] [ebp-10h]
	int v102; // [esp+ECh] [ebp-Ch]
	int v103; // [esp+F0h] [ebp-8h]
	int32_t v104; // [esp+F4h] [ebp-4h]

	int a1 = 0;//fix

	//debug
	//loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&v87, 0x354e80,180);//zbytecne
	/*loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&a6, 0x354f6c, 1);
	loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&a4, 0x354f64, 4);
	loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&a3, 0x354f60, 4);
	loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)&a8, 0x354f74, 4);*/
	/*loadfromsnapshot2((char*)"0160-00260CB0", (uint8_t*)x_DWORD_17DED4, 0x34eed4, 1000);
	a2 = (uint8_t*)malloc(0x10000);
	loadfromsnapshot((char*)"0160-00260CB0", (uint8_t*)a2, 0x354f5c, 0x10000);*/
	//a5 = 0x12c;

	//debug

	//fix it
	v86 = a7;
	//fix it

	v8 = a5;
	v98 = a5;
	v102 = a5;
	v95 = 0;
	v103 = 0;
	v104 = 0;
	memset(v87, 0, 180);
	if (a6)
	{
		if (a6 == 2 || a6 == 5)
		{
			v104 = a4 - a3;
			if (a8)
			{
				v103 = xy_DWORD_17DED4_spritestr[275].width;
				v90 = xy_DWORD_17DED4_spritestr[275].width;
				v94 = v104 / v90;
				if (v104 % v90)
				{
					v16 = v103 * (v94++ + 1);
					v104 = v16;
					a4 = v16 + a3 - xy_DWORD_17DED4_spritestr[274].width;
				}
				for (i = 0; i < v104; i += v103)
					sub_7C120_draw_bitmap_640(i + a3, a5, xy_DWORD_17DED4_spritestr[275]);
				//HIWORD(v18) = HIWORD(xy_DWORD_17DED4_spritestr);
				v18 = xy_DWORD_17DED4_spritestr[275].height;
				v8 = v18 + a5;
				v19 = (pdwScreenBuffer + a3 + 640 * (v18 + a5));
				for (v93 = 0; v93 < xy_DWORD_17DED4_spritestr[274].height; v93++)
				{
					for (v20 = 0; v20 < a4 - (v90 + a3); v20++)
					{
						v90 = xy_DWORD_17DED4_spritestr[274].width;
						HIBYTE(v97) = 15;
						LOBYTE(v97) = *v19;
						v19++;
						*(v19 - 1) = x_DWORD_17DE38str.x_DWORD_17DE3C[v97];
					}
					v19 += 640 - v20;
				}
				sub_7C120_draw_bitmap_640(a3, v8, xy_DWORD_17DED4_spritestr[274]);
				sub_7C120_draw_bitmap_640(a4 - xy_DWORD_17DED4_spritestr[274].width, v8, xy_DWORD_17DED4_spritestr[274]);
			}
			if (a8)
			{
				//HIWORD(v21) = HIWORD(xy_DWORD_17DED4_spritestr);
				v21 = xy_DWORD_17DED4_spritestr[274].height;
				a1 = v21 + v8;
				v98 += v21;
			}
			else
			{
				v98 = v8;
			}
		}
	}
	else
	{//ramecek s textem
		v104 = a4 - a3;//adress 260cf1
		v88 = a8;
		//v92 = (int)xy_DWORD_17DED4_spritestr;
		v103 = xy_DWORD_17DED4_spritestr[a8].width;//10
		if ((a4 - a3) % v103)
		{
			v104 = ((a4 - a3) / v103 + 1) * v103;
			a4 = v104 + a3 - xy_DWORD_17DED4_spritestr[(v88 - 1)].width;
		}
		for (j = 0; j < v104; j += v103)//ramecek - horni vodorovna linka
			sub_7C120_draw_bitmap_640(j + a3, a5, xy_DWORD_17DED4_spritestr[a8]);//25d120

//HIWORD(v10) = HIWORD(xy_DWORD_17DED4_spritestr);
		v10 = xy_DWORD_17DED4_spritestr[a8].height;//adress 260da7
		v11 = v10 + a5;
		v98 += v10;
		v12 = sub_5BE80_test_pallette(x_DWORD_17DE38str.x_DWORD_17DE38, 0, 0, v86);
		v86 = v12;
		v13 = 6 * (a8 - 1);
		v14 = xy_DWORD_17DED4_spritestr[v13 / 6].height;
		v91 = v11;
		v90 = a3;
		sub_7C140_draw_text_background(a3, v11, v104, v14, v12);//prvni cerna linka
		sub_7C120_draw_bitmap_640(v90, v91, xy_DWORD_17DED4_spritestr[v13 / 6]);//prvni levy svisly dilek
		sub_7C120_draw_bitmap_640(a4, v91, xy_DWORD_17DED4_spritestr[v13 / 6]);//prvni pravy svisly dilek
		//HIWORD(v15) = HIWORD(xy_DWORD_17DED4_spritestr);
		v15 = xy_DWORD_17DED4_spritestr[v13 / 6].height;
		a1 = v15 + v11;
		v98 += v15;
	}
	v99 = a3;
	if (!a6)//adress 261023
		v98 = v8 + 12;
	v96 = 0;
	v101 = 0;
	for (k = 0; ; k++)
	{
		v22 = 0;
		if (a6)
		{
			v24 = a2[k];
			if (v24 == ' ' || v24 == 0)
			{
			LABEL_38:
				v22 = 1;
				goto LABEL_39;
			}
		}
		else
		{
			v23 = a2[k];
			if (v23 == ' ' || v23 == 0 || v23 == ',' || v23 == '-' || v23 == '.')
				goto LABEL_38;
		}
	LABEL_39:
		if (v22)//space in text adress 26107c
		{
			if (v101)
			{
				if (a6 && a6 != 4 && a6 != 5)//adress 261091
				{
					sub_6FC50(1/*v86*/);
					v25 = sub_6FC10_letter_width();
				}
				else
				{
					v25 = xy_DWORD_17DEC0_spritestr[65].width;
				}
				v89 = k;
				v26 = v99;
				if (v99 + v25 * (k - v96) <= a4 - 3 * v25)//adress 2610c2
				{
					v86b = &v87[strlen(v87)];
					qmemcpy(v86b, &a2[v101 + 1], v89 - v101);
				}
				else//width is higher then line size
				{
					if (a6 && a6 != 4)
					{
						if (a6 == 2)
						{
							sub_6FC50(1/*v86*/);
							v31 = &pdwScreenBuffer[v26 + 640 * a1];
							for (v32 = 0; v32 < xy_DWORD_17DED4_spritestr[274].height; v32++)
							{
								for (v33 = 0; v33 < a4 - (v99 - xy_DWORD_17DED4_spritestr[274].width); v33++)
								{
									HIBYTE(v97) = 15;
									LOBYTE(v97) = *v31;
									v31++;
									*(v31 - 1) = x_DWORD_17DE38str.x_DWORD_17DE3C[v97];
								}
								v31 += 640 - v33;
							}
							sub_7C120_draw_bitmap_640(v99, a1, xy_DWORD_17DED4_spritestr[274]);
							sub_7C120_draw_bitmap_640(a4, a1, xy_DWORD_17DED4_spritestr[274]);
							//HIWORD(v34) = HIWORD(xy_DWORD_17DED4_spritestr);
							v34 = xy_DWORD_17DED4_spritestr[274].height;
							a1 += v34;
							v86 = a7;
							v35 = v98;
							v36 = a4 - 2 * sub_6FC10_letter_width();
							sub_6FC80_pre_draw_text(v87, v99 + xy_DWORD_17DED4_spritestr[275].width, v36, v35, a7);
						}
						else if (a6 == 5)
						{
							if (a8)
							{
								v38 = &pdwScreenBuffer[v99 + 640 * a1];
								for (v37 = 0; v37 < xy_DWORD_17DED4_spritestr[274].height; v37++)
								{
									for (v39 = 0; v39 < a4 - (xy_DWORD_17DED4_spritestr[274].width + v99); v39++)
									{
										HIBYTE(v40) = 15;
										LOBYTE(v40) = *v38;
										v38++;
										*(v38 - 1) = x_DWORD_17DE38str.x_DWORD_17DE3C[v40];
									}
									v38 += 640 - v39;
								}
								v41 = v99;
								sub_7C120_draw_bitmap_640(v99, a1, xy_DWORD_17DED4_spritestr[274]);
								v86 = *xy_DWORD_17DED4_spritestr[274].data;
								sub_7C120_draw_bitmap_640(a4 - xy_DWORD_17DED4_spritestr[274].width, a1, xy_DWORD_17DED4_spritestr[274]);
								//HIWORD(v42) = HIWORD(xy_DWORD_17DED4_spritestr);
								v42 = xy_DWORD_17DED4_spritestr[274].height;
								a1 += v42;
								//2613b3
								sub_7FAE0_draw_text(v87, v41, a4, v98, 0/*v86*/);
							}
							else
							{
								sub_7FAE0_draw_text(v87, v99, a4, v98, v86);
							}
						}
						else
						{
							sub_6FC80_pre_draw_text(v87, v99, a4, v98, a7);
						}
					}
					else
					{
						if (!a6)
						{
							v27 = sub_5BE80_test_pallette(x_DWORD_17DE38str.x_DWORD_17DE38, 0, 0, v86);
							v86 = v27;
							v28 = 6 * (a8 - 1);
							v29 = xy_DWORD_17DED4_spritestr[v28 / 6].height;
							v90 = v99;
							sub_7C140_draw_text_background(v99, a1, v104, v29, v27);
							sub_7C120_draw_bitmap_640(v90, a1, xy_DWORD_17DED4_spritestr[v28 / 6]);
							sub_7C120_draw_bitmap_640(a4, a1, xy_DWORD_17DED4_spritestr[v28 / 6]);
							//HIWORD(v30) = HIWORD(xy_DWORD_17DED4_spritestr);
							v30 = xy_DWORD_17DED4_spritestr[v28 / 6].height;
							a1 += v30;
						}
						//"click here" 12a 1e2 138 00
						sub_7FAE0_draw_text(v87, v99, a4, v98, 0/*v86*/);//adress 261197 (80197)
					}
					if (a6 && a6 != 4 && a6 != 5)
					{
						LOWORD(v44) = sub_6FC30_get34_height();
						v98 += v44 + 2;
					}
					else
					{
						//HIWORD(v43) = HIWORD(x_DWORD_17DEC0);
						v43 = xy_DWORD_17DEC0_spritestr[65].height;
						v98 += v43;
					}
					memset(v87, 0, 180);
					v86b = v87;
					qmemcpy(v87, &a2[v101 + 1], k - v101);
					v96 = v101;
				}
			}
			else
			{
				v86b = v87;
				qmemcpy(v87, a2, k + 1);//copy first text word
			}
			v101 = k;
		}
		if (!a2[k])
			break;
	}
	if (!v95)//adress 2614e2 (804e2) discoverwhich
	{
		if (a6 && a6 != 4)
		{
			sub_6FC50(1/*v86*/);
			if (a6 == 2)
			{
				sub_6FC50(1/*v86*/);
				v58 = (x_BYTE*)(v99 + 640 * a1);
				v59 = (int)pdwScreenBuffer;
				for (v57 = 0; v57 < xy_DWORD_17DED4_spritestr[274].height; v57++)
				{
					v58 += v59;
					for (v60 = 0; v60 < a4 - (v99 - v91); v60++)
					{
						v91 = xy_DWORD_17DED4_spritestr[274].width;
						HIBYTE(v97) = 15;
						LOBYTE(v97) = *v58;
						v58++;
						*(v58 - 1) = x_DWORD_17DE38str.x_DWORD_17DE3C[v97];
					}
					v59 = 640 - v60;
				}
				v61 = v99;
				sub_7C120_draw_bitmap_640(v99, a1, xy_DWORD_17DED4_spritestr[274]);
				sub_7C120_draw_bitmap_640(a4, a1, xy_DWORD_17DED4_spritestr[274]);
				//HIWORD(v62) = HIWORD(xy_DWORD_17DED4_spritestr);
				v62 = xy_DWORD_17DED4_spritestr[274].height;
				v63 = v62 + a1;
				sub_6FC50(1/*v86*/);
				v64 = &pdwScreenBuffer[640 * v63 + v61];
				for (v65 = 0; v65 < xy_DWORD_17DED4_spritestr[274].height; v65++)
				{
					for (v66 = 0; v66 < a4 - (v99 - v91); v66++)
					{
						v91 = xy_DWORD_17DED4_spritestr[274].width;
						if (v66 >= a4 - (v99 - v91))
							break;
						HIBYTE(v97) = 15;
						LOBYTE(v97) = *v64;
						v64++;
						*(v64 - 1) = x_DWORD_17DE38str.x_DWORD_17DE3C[v97];
					}
					v64 += 640 - v66;
				}
				sub_7C120_draw_bitmap_640(v99, v63, xy_DWORD_17DED4_spritestr[274]);
				sub_7C120_draw_bitmap_640(a4, v63, xy_DWORD_17DED4_spritestr[274]);
				//HIWORD(v67) = HIWORD(xy_DWORD_17DED4_spritestr);
				v67 = xy_DWORD_17DED4_spritestr[274].height;
				v68 = 0;
				a1 = v67 + v63;
				while (v68 < v104)
				{
					sub_7C120_draw_bitmap_640(v68 + a3, a1, xy_DWORD_17DED4_spritestr[275]);
					v68 += v103;
				}
				v86 = a7;
				v69 = v98;
				v70 = a4 - 2 * sub_6FC10_letter_width();
				sub_6FC80_pre_draw_text(v87, v99 + xy_DWORD_17DED4_spritestr[275].width, v70, v69, a7);
			}
			else if (a6 == 5)
			{
				if (a8)
				{
					v71 = 640 * a1 + v99;
					v72 = (x_BYTE*)pdwScreenBuffer;
					for (l = 0; l < xy_DWORD_17DED4_spritestr[274].height; l++)
					{
						v72 += v71;
						v74 = 0;
						for (v74 = 0; v74 < a4 - (xy_DWORD_17DED4_spritestr[274].width + v99); v74++)
						{
							HIBYTE(v97) = 15;
							LOBYTE(v97) = *v72;
							v72++;
							*(v72 - 1) = x_DWORD_17DE38str.x_DWORD_17DE3C[v97];
						}
						v71 = 640 - v74;
					}
					v75 = v99;
					sub_7C120_draw_bitmap_640(v99, a1, xy_DWORD_17DED4_spritestr[274]);
					v86 = *xy_DWORD_17DED4_spritestr[274].data;
					sub_7C120_draw_bitmap_640(a4 - xy_DWORD_17DED4_spritestr[274].width, a1, xy_DWORD_17DED4_spritestr[274]);
					//HIWORD(v76) = HIWORD(xy_DWORD_17DED4_spritestr);
					v76 = xy_DWORD_17DED4_spritestr[274].height;
					v77 = v76 + a1;
					v78 = &pdwScreenBuffer[640 * v77 + v75];
					v79 = 0;
					for (v79 = 0; v79 < xy_DWORD_17DED4_spritestr[274].height; v79++)
					{
						v80 = 0;
						for (v80 = 0; v80 < a4 - (xy_DWORD_17DED4_spritestr[274].width + v99); v80++)
						{
							HIBYTE(v97) = 15;
							LOBYTE(v97) = *v78;
							v78++;
							*(v78 - 1) = x_DWORD_17DE38str.x_DWORD_17DE3C[v97];
						}
						v78 += 640 - v80;
					}
					sub_7C120_draw_bitmap_640(v99, v77, xy_DWORD_17DED4_spritestr[274]);
					v86 = *xy_DWORD_17DED4_spritestr[274].data;
					sub_7C120_draw_bitmap_640(a4 - xy_DWORD_17DED4_spritestr[274].width, v77, xy_DWORD_17DED4_spritestr[274]);
					//HIWORD(v81) = HIWORD(xy_DWORD_17DED4_spritestr);
					v81 = xy_DWORD_17DED4_spritestr[274].height;

					a1 = v81 + v77;
					for (v82 = 0; v82 < v104; v82 += v103)
					{
						v83 = v82 + a3;
						sub_7C120_draw_bitmap_640(v83, a1, xy_DWORD_17DED4_spritestr[275]);
					}
					sub_7FAE0_draw_text(v87, v99 + xy_DWORD_17DED4_spritestr[275].width, a4, v98, 0);
				}
				else
				{
					sub_7FAE0_draw_text(v87, v99, a4, v98, v86);
				}
			}
			else
			{
				sub_6FC80_pre_draw_text(v87, v99, a4, v98, a7);
			}
		}
		else
		{
			if (!a6)
			{
				v45 = sub_5BE80_test_pallette(x_DWORD_17DE38str.x_DWORD_17DE38, 0, 0, v86);
				v86 = v45;
				v46 = 6 * (a8 - 1);
				v47 = xy_DWORD_17DED4_spritestr[v46 / 6].height;
				v91 = v104;
				v48 = v99;
				v90 = a1;
				sub_7C140_draw_text_background(v99, a1, v104, v47, v45);//draw next black row
				sub_7C120_draw_bitmap_640(v48, v90, xy_DWORD_17DED4_spritestr[v46 / 6]);//draw next left column
				v49 = v90;
				v90 = a4;
				sub_7C120_draw_bitmap_640(a4, v49, xy_DWORD_17DED4_spritestr[v46 / 6]);//draw next right column
				//HIWORD(v50) = HIWORD(xy_DWORD_17DED4_spritestr);
				v50 = xy_DWORD_17DED4_spritestr[v46 / 6].height;
				v51 = v50 + a1;
				v52 = sub_5BE80_test_pallette(x_DWORD_17DE38str.x_DWORD_17DE38, 0, 0, v86);//?
				v86 = v52;
				v53 = xy_DWORD_17DED4_spritestr[v46 / 6].height;
				v54 = v91;
				v91 = v51;
				sub_7C140_draw_text_background(v48, v51, v54, v53, v52);//draw next black row
				sub_7C120_draw_bitmap_640(v48, v91, xy_DWORD_17DED4_spritestr[v46 / 6]);//draw next left column
				sub_7C120_draw_bitmap_640(v90, v91, xy_DWORD_17DED4_spritestr[v46 / 6]);//draw next right column
				//HIWORD(v55) = HIWORD(xy_DWORD_17DED4_spritestr);
				v55 = xy_DWORD_17DED4_spritestr[v46 / 6].height;
				v56 = 0;
				a1 = v55 + v51;
				while (v56 < v104)
				{
					sub_7C120_draw_bitmap_640(v56 + v99, a1, xy_DWORD_17DED4_spritestr[a8]);//draw bottom row
					v56 += v103;
				}
			}
			sub_7FAE0_draw_text(v87, v99, a4, v98, 0/*v86*/);//draw text(with pallette?) 
		}
	}
	if (a6)
		return v98 - v102;
	//HIWORD(v84) = HIWORD(xy_DWORD_17DED4_spritestr);
	v84 = xy_DWORD_17DED4_spritestr[(a8 + 2)].height;

	//debug
	//compare_with_snapshot((char*)"0x45678", (uint8_t*)&v8, 0x4589, 4);
	//debug

	return a1 - v102 + v84;
}

//----- (0007C120) --------------------------------------------------------
void sub_7C120_draw_bitmap_640(int16_t posx, int16_t posy, posistruct_t tempstr)//25d120
{
	sub_2BB40_draw_bitmap(posx, posy, tempstr);//ebp
}

//----- (00076260) --------------------------------------------------------
void sub_76260_read_intro_pallette(uint8_t a1)
{
	uint8_t* v0; // ebx
	int v1; // esi
	//unsigned int result; // eax
	unsigned __int16 v3; // di
	uint16_t v4; // [esp+0h] [ebp-Ch]
	int32_t v5; // [esp+4h] [ebp-8h]
	unsigned __int8 v6; // [esp+8h] [ebp-4h]

	v0 = unk_17D838;
	v1 = 0;
	/*result = */sub_75D70((uint8_t*)&v4, 2u);
	if (v4 > 0u)
	{
		do
		{
			sub_75D70(&v6, 1u);
			v0 += 3 * v6;
			v5 = 0;
			/*result = */sub_75D70((uint8_t*)&v5, 1u);
			if (!v5)
				v5 = 256;
			for (v3 = 0; v3 < v5; v3++)//mybe read pallette
			{
				sub_75D70(v0, 3u);
				v0 += 3;
			}
			v1++;
		} while (v1 < v4);
	}
	//return result;
}

//----- (0005BE80) --------------------------------------------------------
uint8_t sub_5BE80_test_pallette(uint8_t* pallette, uint8_t a2, uint8_t a3, uint8_t a4)//23ce80
{
	//uint8_t *v4; // eax
	uint16_t count_of_colors; // edx
	int16_t v6; // ecx
	//uint16_t v7; // edx
	int16_t v9; // esi
	int16_t v10; // esi
	int16_t v11; // edi
	//unsigned int v12; // [esp+0h] [ebp-8h]
	uint8_t result; // [esp+4h] [ebp-4h]

	//fix it
	result = 0;
	//fix it

	//v4 = pallette;//1a7358
	uint16_t pallette_index = 0;
	if (x_WORD_180660_VGA_type_resolution & 6)
		count_of_colors = 16;
	else
		count_of_colors = 256;
	v6 = 9999;
	//v12 = count_of_colors;
	//v7 = 0;
	//if (count_of_colors > 0)
	//{
	for (uint16_t i = 0; i < count_of_colors; i++)
	{
		v9 = a3 - pallette[pallette_index + 1];//eax[1]
		v10 = (a2 - pallette[pallette_index]) * (a2 - pallette[pallette_index]) + v9 * v9;
		v11 = a4 - pallette[pallette_index + 2];
		if (v10 + v11 * v11 < v6)
		{
			v6 = v10 + v11 * v11;
			result = i;
		}
		//v7++;
		pallette_index += 3;
	} //while (v7 < count_of_colors);
//}
	return result;
}
// 180660: using guessed type __int16 x_WORD_180660_VGA_type_resolution;

//----- (0007C140) --------------------------------------------------------
void sub_7C140_draw_text_background(int16_t x1, int16_t y1, int16_t x2, int16_t y2, unsigned __int8 a5)//25d140
{//black line in text
	int16_t x1d; // si
	int16_t y1d; // bx
	int16_t x2d; // eax
	int16_t y2d; // dx
	//int16_t v9; // cx

	x1d = x1;
	y1d = y1;
	x2d = x2;
	y2d = y2;
	if (x1 < 640 && y1 < 480)//neni vetsi nez obrazovka
	{
		if (y1 + y2 > 480)
			y2d = 480 - y1;
		if (x1 + x2 > 640)
			x2d = 640 - x1;
		if (x2d >= 1 && y2d >= 1)
		{
			if (y1 < 0)
			{
				y2d += y1;
				x1d = 0;
			}
			if ((x1 & 0x8000u) != 0)
			{
				x2d += x1;
				x1d = 0;
			}
			//v9 = x2d;
			//x2d += x1d;
			if (x2d + x1d >= 1)
			{
				//x2d = y1d;
				if (y1d + y2d >= 1)
					sub_2BC80(x1d, y1d, x2d, y2d, a5);
			}
		}
	}
}

//----- (0006FC50) --------------------------------------------------------
void sub_6FC50(__int16 a1)//250c50
{
	//int result; // eax
	posistruct_t* v2; // edx

	//result = a1;
	v2 = *x_DWORD_E9B20[a1].posistruct;
	if (!v2->data)
		v2 = *x_DWORD_E9B20[0].posistruct;
	x_DWORD_EA3D4 = v2;
	//return result * 4;
}
// E9B20: using guessed type int x_DWORD_E9B20[];
// EA3D4: using guessed type int x_DWORD_EA3D4;

//----- (0006FC10) --------------------------------------------------------
uint8_t sub_6FC10_letter_width()//250c10
{
	if (help_VGA_type_resolution != 8)//fixed
		return x_DWORD_EA3D4[33].width;
	else
		return posistruct7[33].width;
}
// EA3D4: using guessed type int x_DWORD_EA3D4;

//----- (0006FC80) --------------------------------------------------------
unsigned int sub_6FC80_pre_draw_text(char* a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5)//00250C80
{
	unsigned int v5; // kr04_4
	int v6; // esi
	int v7; // eax
	__int16 v9; // [esp+0h] [ebp-4h]

	v9 = a3 - a2;
	v5 = strlen(a1) + 1;
	if (v5 == 1)
	{
		v6 = a2 + v9 / 2;
	}
	else
	{
		v6 = a2 + v9 / 2 - (v5 - 1) * sub_6FC10_letter_width() / 2;
		sub_2BC10_draw_text(a1, v6, a4, a5);
	}
	v7 = sub_6FC10_letter_width();
	return v6 + v7 * strlen(a1);
}

//----- (000417A0) --------------------------------------------------------
void sub_417A0_install_pal_and_mouse_minmax()//2227a0
{
	//sub_90810();
	sub_41A90_VGA_pallette_install(*xadatapald0dat2.var28_begin_buffer);
	sub_6EF10_set_mouse_minmax(0, 640, 0, 400);
}
// EA3D8: using guessed type int *xadatapald0dat2.var28_begin_buffer;

//----- (0002EC90) --------------------------------------------------------
void sub_2EC90(char a1)//20fc90
{
	//char result; // al

	if (x_BYTE_D41CE)
	{
		//result = a1;
		x_BYTE_EB3B6 = a1;
		if (x_DWORD_D41D0)
			/*result = */sub_2EBB0_draw_text_with_border_630x340(x_DWORD_D41D0);
	}
	//return result;
}
// D41CE: using guessed type char x_BYTE_D41CE;
// D41D0: using guessed type int x_DWORD_D41D0;
// EB3B6: using guessed type char x_BYTE_EB3B6;

//----- (0007FAE0) --------------------------------------------------------
uint32_t sub_7FAE0_draw_text(char* text, int16_t a2, int16_t a3, int16_t posy, uint8_t a5)//260ae0
{
	uint32_t helpstrlen; // kr04_4
	int16_t v6; // bx
	int32_t posx; // ebx

	helpstrlen = strlen(text);
	v6 = a3 - a2;//ebx[ebx+1c] - 1e2 edx[ebp+18] - 12a
	if (helpstrlen == 1)
	{
		posx = a2 + v6 / 2;
	}
	else
	{
		posx = v6 / 2 + a2 - xy_DWORD_17DEC0_spritestr[65].width * helpstrlen / 2;
		sub_7FB90_draw_text(text, posx, posy, a5);//"clisk here to" 13d 138 0
	}
	return posx + xy_DWORD_17DEC0_spritestr[65].width * strlen(text);
}
// 17DEC0: using guessed type int (int)x_DWORD_17DEC0;

//int debugcounter_271478 = 0;
//----- (00090478) --------------------------------------------------------
void sub_90478_VGA_Blit320()//271478
{
#ifdef DEBUG_SEQUENCES
	uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;
	if (debugafterload)
	{
		//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debugcounter_47560, 0x70000, 0x10000, &origbyte20, &remakebyte20);
		//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debugcounter_47560, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
		//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debugcounter_47560, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
		//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debugcounter_47560, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
		//comp20 = compare_with_sequence((char*)"0022860F-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debugcounter_47560, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);

		//comp20 = compare_with_sequence_D41A0((char*)"0022860F-00356038", (uint8_t*)&D41A0_BYTESTR_0, 0x356038, debugcounter_271478, 224790, &origbyte20, &remakebyte20);

		//comp20 = compare_with_sequence_array_E2A74((char*)"0022860F-002B3A74", (uint8_t*)&array_E2A74, 0x2b3a74, debugcounter_271478, 0xc4e, 0xc4e, &origbyte20, &remakebyte20);
		//if(debugcounter_271478>5)
		//comp20 = compare_with_sequence((char*)"0022860F-003AA0A4", pdwScreenBuffer, 0x3aa0a4, debugcounter_271478, 320 * 200, 320 * 200, &origbyte20, &remakebyte20);

		debugcounter_271478++;
	}
#endif DEBUG_SEQUENCES
	if (!x_BYTE_E3766)
		sub_8CACD_draw_cursor2();
#ifndef debug_hide_graphics
	VGA_Blit(320, 200, pdwScreenBuffer);
#endif
	//if(dos_key_vect_9)dos_key_vect_9();
	//VGA_mouse_clear_keys();

	//set speed
	long actmillis = mygetthousandths();
	long newdelay = speedGame - (actmillis - oldmillis);//max millis is 20 millis
	if (newdelay < 0)newdelay = 0;
	if (newdelay > speedGame)newdelay = speedGame;
	mydelay(newdelay);//set speed
	oldmillis = actmillis;
	//set speed

	/*
	qmemcpy(&loc_A0000_vga_buffer, (void *)pdwScreenBuffer, 0xFA00u);
	if ( !x_BYTE_E3766 )
	  sub_8CB1F();*/
}
// E3766: using guessed type char x_BYTE_E3766;
// 180628: using guessed type int pdwScreenBuffer;

//language
char* x_DWORD_E9C4C_langindexbuffer[1000]; // idb