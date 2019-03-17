#include "engine_support.h"


#ifdef USE_DOSBOX
extern DOS_Device *DOS_CON;
#endif //USE_DOSBOX


//delete after finalization
Bit8u* readbuffer;

char* printbuffer;//char* buffer; // [esp+0h] [ebp-2h]
char* printbuffer2;//char v11; // [esp+40h] [ebp+3Eh]
/*Bit32s x_DWORD_D41A4_x6x = 0;
Bit32s x_DWORD_D41A4_x8x = 0;
Bit32s x_DWORD_D41A4_xAx = -1;
Bit32s x_DWORD_D41A4_xBx = -1;
Bit32s x_DWORD_D41A4_xCx = -1;
Bit32s x_DWORD_D41A4_xDx = -1;
char x_DWORD_D41A4_xB6 = 'C';//2A1644 b6=182
char x_DWORD_D41A4_x16x = '5';//2A15A4 16=22
Bit8u x_DWORD_D41A4_x17x = 0;
Bit8u x_DWORD_D41A4_x18x = 0;
char x_DWORD_D41A4_x19x=0;//2A51BD 19=25
Bit8u x_DWORD_D41A4_x1Ex = 0;
Bit8u x_DWORD_D41A4_x2Bx=0;
Bit8u x_DWORD_D41A4_x2Dx = 0;
Bit32s x_DWORD_D41A4_x33x = 0;
Bit32s x_DWORD_D41A4_x59x = 0;
Bit32s x_DWORD_D41A4_x79x = 0;
Bit32s x_DWORD_D41A4_x7Ax = 0;
Bit32s x_DWORD_D41A4_x7Bx = 0;
Bit32s x_DWORD_D41A4_x7Cx = 0;
Bit32s x_DWORD_D41A4_x7Dx = 0;
Bit32s x_DWORD_D41A4_x7Fx = 0;
//char* char_355198 = "8R5";
Bit32s x_DWORD_D41A4_xB2 = 0;
Bit32s x_DWORD_D41A4_xB4 = 0;
Bit32s x_DWORD_D41A4_xB7 = 0;
Bit32s x_DWORD_D41A4_xBA = 0;
Bit32s x_DWORD_D41A4_xC0 = 0;
Bit32s x_DWORD_D41A4_xCE = 0;
Bit32s x_DWORD_D41A4_xCF = 0;
Bit32s x_DWORD_D41A4_xCD = 0;
Bit32s x_DWORD_D41A4_xD0 = 0;
Bit32s x_DWORD_D41A4_xD8=0;
Bit32s x_DWORD_D41A4_xDC = 0;
Bit32s x_DWORD_D41A4_xE0 = -1;
Bit32s x_DWORD_D41A4_xE1 = -1;
Bit32s x_DWORD_D41A4_xE2=-1;
Bit32s x_DWORD_D41A4_xE6 = -1;
Bit32s x_DWORD_D41A4_xF2 = -1;
Bit32s x_DWORD_D41A4_xF6 = -1;
Bit32s x_DWORD_D41A4_x100 = -1;
Bit32s x_DWORD_D41A4_x235 = -1;
Bit32s x_DWORD_D41A4_x749 = -1;
Bit32s x_DWORD_D41A4_x8CF = -1;
Bit32s x_DWORD_D41A4_x94C = -1;
Bit32s x_DWORD_D41A4_x954 = -1;
Bit32s x_DWORD_D41A4_x21AA = -1;
Bit32s x_DWORD_D41A4_x00 = -1;
Bit32s x_DWORD_D41A4_x9602 = -1;
Bit32s x_DWORD_D41A4_x9603 = -1;
Bit32s x_DWORD_D41A4_x9677 = -1;
Bit32s x_DWORD_D41A4_x967B = -1;
Bit32s x_DWORD_D41A4_x967F = -1;
Bit32s x_DWORD_D41A4_x4 = -1;
Bit32s x_DWORD_D41A4_x965B = -1;
Bit32s x_DWORD_D41A4_x966F = -1;
Bit32s x_DWORD_D41A4_x9683 = -1;
Bit32s x_DWORD_D41A4_x96BF = -1;
Bit32s x_DWORD_D41A4_x9691 = -1;
Bit32s x_DWORD_D41A4_x9692 = -1;*/

//Bit32s x2124_x_DWORD_D41A4_xCx_x_DWORD_D41A0_11234 = 0;

//Bit8u* x_DWORD_E9C38_smalltit; // weak

//Bit32s x_DWORD_D41A4 = -1;

//Bit8u* x_DWORD_D41A0; // weak

Bit8u* x_D41A0_BYTEARRAY_0;
//12 - my be player counter(position of structure)
//11244 - players(my by creatures, every 2124 lenght)
//196302 - terrain input
//196308 - sky and blocks


//Bit8u* x_D41A0_BYTEARRAY_4;

type_x_D41A0_BYTEARRAY_4_struct x_D41A0_BYTEARRAY_4_struct;

Bit16s x_D41A0_WORDARRAY[10000];

Bit32u x_D41A0_BYTEARRAY_4_0xE6_heapsize;
Bit8u* x_D41A0_BYTEARRAY_4_0xE2_heapbuffer;
//Bit8u* x_D41A0_BYTEARRAY_4_0xDE_heapbuffer;
//Bit32u* off_D918C[0x7c];//turn off - fix it

//xx Bit8u* dword_E9C30[1000]; // weak



Bit8u* x_DWORD_180628b_screen_buffer; //351628

Bit8u* off_D41A8;//graphics buffer// = (Bit8u*)&x_BYTE_14B4E0; // weak
Bit8u* x_BYTE_14B4E0;

posistruct* xy_DWORD_17DED4_spritestr;
posistruct* xy_DWORD_17DEC0_spritestr;
posistruct* xy_DWORD_17DEC8_spritestr;

posistruct* x_DWORD_D4188t_spritestr;

posistruct* xy_DWORD_17DEC0_spritestr_orig;

doublebyte doublebyte_conv(Bit16u a2) {
	doublebyte result;
	result.byte1 = a2 && 0xff;
	result.byte2 = a2 && 0xff00;
	return result;
};

Bit8u* pre_x_DWORD_180628b_screen_buffer;

void support_begin() {
    readbuffer = (Bit8u*)malloc(1000000);//fix it max 64000
    printbuffer = (char*)malloc(4096);
    //printbuffer[0] = '\0';
    printbuffer2 = (char*)malloc(4096);
	pre_x_DWORD_180628b_screen_buffer = (Bit8u*)malloc(1000000);
	//x_DWORD_180628b_screen_buffer = (Bit8u*)malloc(320000);
	x_DWORD_180628b_screen_buffer = &pre_x_DWORD_180628b_screen_buffer[200000];

	//x_DWORD_E9C38_smalltit= (Bit8u*)malloc(64000);
	//x_D41A0_BYTEARRAY_4_0xDE_heapbuffer= (Bit8u*)malloc(64000);

	/*for (int i = 0;i < 0x1c+0x60;i++)
		off_D918C[i] = new Bit32u;*/
	//2aa18c
	//0 2bc394
	//1
	//2

	/*xx dword_E9C30[0] = new Bit8u[4096];
	for (int i = 0;i <4096;i++)
		dword_E9C30[0][0] = 0;
	dword_E9C30[2] = new Bit8u[4096];
	dword_E9C30[4] = new Bit8u[4096];*/
	//x_D41A0_BYTEARRAY_4_struct.player_name_57 =new char[256];
	x_D41A0_BYTEARRAY_4_struct.savestring_89 = new char[256];

	x_BYTE_14B4E0 = new Bit8u[65536];
	off_D41A8 = new Bit8u[65536];
	memcpy(off_D41A8, &x_BYTE_14B4E0,4);

	xy_DWORD_17DED4_spritestr = new posistruct[1000];
	xy_DWORD_17DEC0_spritestr_orig = new posistruct[1000];
	xy_DWORD_17DEC0_spritestr=xy_DWORD_17DEC0_spritestr_orig;
	xy_DWORD_17DEC8_spritestr = new posistruct[1000];

	x_DWORD_D4188t_spritestr = new posistruct[1000];

	x_D41A0_BYTEARRAY_4_struct.harddiskchar_byte4_182 = 0x43;
	//x_D41A0_BYTEARRAY_4_struct.player_name_57 = 0;

    //printbuffer2[0] = '\0';
}
void support_end() {
	if(readbuffer)free(readbuffer);
	if(printbuffer)free(printbuffer);//char* buffer; // [esp+0h] [ebp-2h]
    if(printbuffer2)free(printbuffer2);//char v11; // [esp+40h] [ebp+3Eh]
	if(pre_x_DWORD_180628b_screen_buffer)free(pre_x_DWORD_180628b_screen_buffer);
	//free(x_DWORD_E9C38_smalltit);
	/*for (int i = 0;i < 0x1c;i++)
		free(off_D918C[i]);*/
	//free(x_D41A0_BYTEARRAY_4_0xDE_heapbuffer);
	/*xx free(dword_E9C30[0]);
	free(dword_E9C30[2]);
	free(dword_E9C30[4]);*/
	//free(x_D41A0_BYTEARRAY_4_struct.player_name_57);
	if(x_D41A0_BYTEARRAY_4_struct.savestring_89)delete(x_D41A0_BYTEARRAY_4_struct.savestring_89);

	if(x_BYTE_14B4E0)delete(x_BYTE_14B4E0);
	if(off_D41A8)delete(off_D41A8);

	if(xy_DWORD_17DED4_spritestr)delete(xy_DWORD_17DED4_spritestr);
	if(xy_DWORD_17DEC0_spritestr_orig)delete(xy_DWORD_17DEC0_spritestr_orig);//fixed
	if(xy_DWORD_17DEC8_spritestr)delete(xy_DWORD_17DEC8_spritestr);

	if(x_DWORD_D4188t_spritestr)delete(x_DWORD_D4188t_spritestr);
}

void loadfromsnapshot(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size) {
	char findnamec[500];
	FILE* fptestepc;
	sprintf(findnamec, "../remc2/memimages/engine-memory-%s", filename);
	fopen_s(&fptestepc, findnamec, "rb");	
	fseek(fptestepc, adressdos, SEEK_SET);
	fread_s(adress, size, 1, size, fptestepc);
	fclose(fptestepc);
};

void loadfromsnapshot2(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size) {
	char findnamec[500];
	FILE* fptestepc;
	Bit32u subadress;
	sprintf(findnamec, "../remc2/memimages/engine-memory-%s", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	fseek(fptestepc, adressdos, SEEK_SET);
	fread_s(&subadress, 4, 1, 4, fptestepc);
	fseek(fptestepc, subadress, SEEK_SET);
	fread_s(adress, size, 1, size, fptestepc);

	fclose(fptestepc);
};


Bit32u compare_with_snapshot(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size, Bit8u* origbyte, Bit8u* copybyte) {

	char findnamec[500];
	Bit8u* buffer = (Bit8u*)malloc(size);
	FILE* fptestepc;
	sprintf(findnamec, "c:/prenos/remc2/remc2/memimages/engine-memory-%s", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	if (fptestepc == NULL)
	{
		mydelay(100);
		fopen_s(&fptestepc, findnamec, "rb");
	}
	fseek(fptestepc, adressdos, SEEK_SET);
	
	fread_s(buffer, size, 1, size, fptestepc);
	int i;
	for (i = 0;i < size;i++)
	{
		if (buffer[i] != adress[i])
		{
			*origbyte = buffer[i];
			*copybyte = adress[i];
			break;
		}
	}

	free(buffer);
	fclose(fptestepc);
	return(i);
};

int test_D41A0_id_pointer(Bit32u adress) {
	if ((adress >= 0x314d) && (adress < 0x3151))return 2;//clock
	if ((adress >= 0x246)&& (adress < 0x12b2))return 1;
	/*if ((adress >= 0x6f32) && (adress < 0x6f37))return 1;
	if ((adress >= 0x6fd6) && (adress < 0x6fdb))return 1;
	if ((adress >= 0x707e) && (adress < 0x707f))return 1;
	if ((adress >= 0x7082) && (adress < 0x7083))return 1;
	if ((adress >= 0x7126) && (adress < 0x712b))return 1;
	if ((adress >= 0x71ce) && (adress < 0x71cf))return 1;
	if ((adress >= 0x71d2) && (adress < 0x71d3))return 1;
	if ((adress >= 0x7276) && (adress < 0x7277))return 1;
	if ((adress >= 0x727a) && (adress < 0x727b))return 1;
	if ((adress >= 0x731e) && (adress < 0x731f))return 1;
	if ((adress >= 0x7322) && (adress < 0x7323))return 1;
	if ((adress >= 0x73c6) && (adress < 0x73c7))return 1;
	if ((adress >= 0x73ca) && (adress < 0x73cb))return 1;
	if ((adress >= 0x746e) && (adress < 0x746f))return 1;
	if ((adress >= 0x7472) && (adress < 0x7473))return 1;
	if ((adress >= 0x7516) && (adress < 0x7517))return 1;
	if ((adress >= 0x751a) && (adress < 0x751b))return 1;
	if ((adress >= 0x75be) && (adress < 0x75bf))return 1;
	if ((adress >= 0x75c2) && (adress < 0x75c3))return 1;
	if ((adress >= 0x7666) && (adress < 0x7667))return 1;
	if ((adress >= 0x766a) && (adress < 0x766b))return 1;
	if ((adress >= 0x770e) && (adress < 0x770f))return 1;
	if ((adress >= 0x7712) && (adress < 0x7713))return 1;
	if ((adress >= 0x77b6) && (adress < 0x77b7))return 1;
	if ((adress >= 0x77ba) && (adress < 0x77bb))return 1;
	if ((adress >= 0x785e) && (adress < 0x7863))return 1;
	if ((adress >= 0x7906) && (adress < 0x790b))return 1;
	if ((adress >= 0x79ae) && (adress < 0x79b3))return 1;
	if ((adress >= 0x7a56) && (adress < 0x7a5b))return 1;
	if ((adress >= 0x7afe) && (adress < 0x7b03))return 1;*/
	for (int i = 0; i < 0x3e8; i++) {
		if ((adress >= 0x6f2e +i*168) && (adress < 0x6f37 + i * 168))return 1;
	}
	if ((adress >= 0x36df6) && (adress < 0x36df7))return 1;
	return 0;
}

int test_E2A74_id_pointer(Bit32u adress) {
	if ((adress >= 0x246) && (adress < 0x247))return 1;
	if ((adress >= 0x282) && (adress < 0x283))return 1;
	if ((adress >= 0x2a0) && (adress < 0x2a1))return 1;

	return 0;
}

int test_222BD3_id_pointer(Bit32u adress) {
	if ((adress >= 0x8) && (adress < 0x9))return 1;
	if ((adress >= 0x50) && (adress < 0x51))return 1;
	if ((adress >= 0x68) && (adress < 0x69))return 1;


	return 0;
}

Bit32u compare_with_snapshot_D41A0(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u size, Bit8u* origbyte, Bit8u* copybyte) {

	char findnamec[500];
	Bit8u* buffer = (Bit8u*)malloc(size);
	FILE* fptestepc;
	sprintf(findnamec, "c:/prenos/remc2/remc2/memimages/engine-memory-%s", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	if (fptestepc == NULL)
	{
		mydelay(100);
		fopen_s(&fptestepc, findnamec, "rb");
	}
	fseek(fptestepc, adressdos, SEEK_SET);

	fread_s(buffer, size, 1, size, fptestepc);
	int i;
	bool testa, testb;
	for (i = 0; i < size; i++)
	{
		int testx = test_D41A0_id_pointer(i);
		if (testx==1)
		{
			if (*(Bit32u*)&buffer[i])testa = true;
			else testa = false;
			if (*(Bit32u*)&adress[i])testb = true;
			else testb = false;
			if (testa != testb)
			{
				*origbyte = buffer[i];
				*copybyte = adress[i];
				break;
			}
			i += 3;
		}
		else if (testx == 0) {
			if (buffer[i] != adress[i])
			{
				*origbyte = buffer[i];
				*copybyte = adress[i];
				break;
			}
		}
	}

	free(buffer);
	fclose(fptestepc);
	return(i);
};

Bit32u compare_with_sequence_D41A0(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte) {

	char findnamec[500];
	Bit8u* buffer = (Bit8u*)malloc(size);
	FILE* fptestepc;
	sprintf(findnamec, "c:/prenos/remc2/remc2/memimages/sequence-%s.bin", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	if (fptestepc == NULL)
	{
		mydelay(100);
		fopen_s(&fptestepc, findnamec, "rb");
	}
	fseek(fptestepc, count*size, SEEK_SET);

	fread_s(buffer, size, 1, size, fptestepc);
	int i;
	bool testa, testb;
	for (i = 0; i < size; i++)
	{
		int testx = test_D41A0_id_pointer(i);
		if (testx == 1)
		{
			if (*(Bit32u*)&buffer[i])testa = true;
			else testa = false;
			if (*(Bit32u*)&adress[i])testb = true;
			else testb = false;
			if (testa != testb)
			{
				*origbyte = buffer[i];
				*copybyte = adress[i];
				break;
			}
			i += 3;
		}
		else if (testx == 0) {
			if (buffer[i] != adress[i])
			{
				*origbyte = buffer[i];
				*copybyte = adress[i];
				break;
			}
		}
	}

	free(buffer);
	fclose(fptestepc);
	return(i);
};

int test_F2C20ar_id_pointer(Bit32u adress) {
	if ((adress >= 0x8) && (adress < 0x9))return 1;
	return 0;
}

Bit32u compare_with_sequence_x_DWORD_F2C20ar(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, int* posdiff) {

	char findnamec[500];
	Bit8u* buffer = (Bit8u*)malloc(size);
	FILE* fptestepc;
	sprintf(findnamec, "c:/prenos/remc2/remc2/memimages/sequence-%s.bin", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	if (fptestepc == NULL)
	{
		mydelay(100);
		fopen_s(&fptestepc, findnamec, "rb");
	}
	fseek(fptestepc, count*size, SEEK_SET);

	fread_s(buffer, size, 1, size, fptestepc);
	int i;
	bool testa, testb;
	int diffindex = 0;
	for (i = 0; i < size; i++)
	{
		int testx = test_F2C20ar_id_pointer(i);
		if (testx == 1)
		{
			if (*(Bit32u*)&buffer[i])testa = true;
			else testa = false;
			if (*(Bit32u*)&adress[i])testb = true;
			else testb = false;
			if (testa != testb)
			{
				posdiff[diffindex] = i;
				origbyte[diffindex] = buffer[i];
				copybyte[diffindex++] = adress[i];
				if (diffindex >= 100)break;
			}
			i += 3;
		}
		else if (testx == 0) {
			if (buffer[i] != adress[i])
			{
				posdiff[diffindex] = i;
				origbyte[diffindex] = buffer[i];
				copybyte[diffindex++] = adress[i];
				if (diffindex >= 100)break;
			}
		}
	}

	free(buffer);
	fclose(fptestepc);
	return(diffindex);
};

Bit32u compare_with_sequence_array_E2A74(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, int* posdiff) {

	char findnamec[500];
	Bit8u* buffer = (Bit8u*)malloc(size);
	FILE* fptestepc;
	sprintf(findnamec, "c:/prenos/remc2/remc2/memimages/sequence-%s.bin", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	if (fptestepc == NULL)
	{
		mydelay(100);
		fopen_s(&fptestepc, findnamec, "rb");
	}
	fseek(fptestepc, count*size, SEEK_SET);

	fread_s(buffer, size, 1, size, fptestepc);
	int i;
	bool testa, testb;
	int diffindex=0;
	for (i = 0; i < size; i++)
	{
		int testx = test_E2A74_id_pointer(i);
		if (testx == 1)
		{
			if (*(Bit32u*)&buffer[i])testa = true;
			else testa = false;
			if (*(Bit32u*)&adress[i])testb = true;
			else testb = false;
			if (testa != testb)
			{
				posdiff[diffindex] = i;
				origbyte[diffindex] = buffer[i];
				copybyte[diffindex++] = adress[i];
				if(diffindex>=100)break;
			}
			i += 3;
		}
		else if (testx == 0) {
			if (buffer[i] != adress[i])
			{
				posdiff[diffindex] = i;
				origbyte[diffindex] = buffer[i];
				copybyte[diffindex++] = adress[i];
				if (diffindex >= 100)break;
			}
		}
	}

	free(buffer);
	fclose(fptestepc);
	return(i);
};

Bit32u compare_with_sequence_array_222BD3(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte, int* posdiff) {

	char findnamec[500];
	Bit8u* buffer = (Bit8u*)malloc(size);
	FILE* fptestepc;
	sprintf(findnamec, "c:/prenos/remc2/remc2/memimages/sequence-%s.bin", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	if (fptestepc == NULL)
	{
		mydelay(100);
		fopen_s(&fptestepc, findnamec, "rb");
	}
	fseek(fptestepc, count*size, SEEK_SET);

	fread_s(buffer, size, 1, size, fptestepc);
	int i;
	bool testa, testb;
	int diffindex = 0;
	for (i = 0; i < size; i++)
	{
		int testx = test_222BD3_id_pointer(i);
		if (testx == 1)
		{
			if (*(Bit32u*)&buffer[i])testa = true;
			else testa = false;
			if (*(Bit32u*)&adress[i])testb = true;
			else testb = false;
			if (testa != testb)
			{
				posdiff[diffindex] = i;
				origbyte[diffindex] = buffer[i];
				copybyte[diffindex++] = adress[i];
				if (diffindex >= 100)break;
			}
			i += 3;
		}
		else if (testx == 0) {
			if (buffer[i] != adress[i])
			{
				posdiff[diffindex] = i;
				origbyte[diffindex] = buffer[i];
				copybyte[diffindex++] = adress[i];
				if (diffindex >= 100)break;
			}
		}
	}

	free(buffer);
	fclose(fptestepc);
	return(i);
};

Bit32u compare_with_sequence(char* filename, Bit8u* adress, Bit32u adressdos, Bit32u count, Bit32u size, Bit8u* origbyte, Bit8u* copybyte) {

	char findnamec[500];
	Bit8u* buffer = (Bit8u*)malloc(100000);
	FILE* fptestepc;
	sprintf(findnamec, "c:/prenos/remc2/remc2/memimages/sequence-%s.bin", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	if (fptestepc == NULL)
	{
		mydelay(100);
		fopen_s(&fptestepc, findnamec, "rb");
	}
	fseek(fptestepc, count*size, SEEK_SET);
	
	int i;
	/*for (i = 0; i < count; i++)
	{
		fread_s(buffer,size,1,size, fptestepc);
	}*/

	fread_s(buffer, size, 1, size, fptestepc);
	//for (i = size-1; i >0; i--)
	for (i = 0; i < size; i++)
	{
		if (buffer[i] != adress[i])
		{
			*origbyte = buffer[i];
			*copybyte = adress[i];
			break;
		}
	}	

	free(buffer);
	fclose(fptestepc);
	return(i);
};

void mine_texts(char* filename, Bit32u adressdos, Bit32u count, char* outfilename) {

	char findnamec[500];
	FILE* fptestepc;
	FILE* fileout;
	char actchar;
	char outtext[2048];
	char outtext2[2048];
	sprintf(findnamec, "c:/prenos/remc2/remc2/memimages/engine-memory-%s", filename);
	fopen_s(&fptestepc, findnamec, "rb");
	fopen_s(&fileout, outfilename, "wb");
	if (fptestepc == NULL)
	{
		mydelay(100);
		fopen_s(&fptestepc, findnamec, "rb");
	}
	fseek(fptestepc, adressdos, SEEK_SET);
	long adressadd;
	long adressaddall=0;
	fread_s(&actchar, 1, 1, 1, fptestepc);
	for (int i = 0; i < count; i++)
	{
		adressadd = 0;
		while ((adressaddall % 4)||(actchar==0))
		{
			fread_s(&actchar, 1, 1, 1, fptestepc);
			adressaddall++;
		}
		while(actchar!=0){
			outtext[adressadd] = actchar;
			fread_s(&actchar, 1, 1, 1, fptestepc);
			adressadd++;
			adressaddall++;
		}
		
		outtext[adressadd]=0;
		sprintf(outtext2,"char* off_%05X[1]={%s};\n", 0xDB06C- 0x1131 + adressaddall,outtext);
		if(strlen(outtext2)>1)
			fwrite(outtext2, strlen(outtext2),1, fileout);
	}

	fclose(fptestepc);
	fclose(fileout);
};

void writehex(Bit8u* buffer, Bit32u count) {
	for (int i=0;i < count;i++)
	{
		if (i % 32 == 0)printf("\n");
		printf("%02X", buffer[i]);		
	}
	printf("\n");
};

type_D41A0_BYTESTR_0 D41A0_BYTESTR_0;
/*
void x_D41A0_BYTEARRAY_0_to_x_D41A0_BYTESTR_0()
{
	D41A0_BYTESTR_0.word_0xc = *(Bit16s*)&x_D41A0_BYTEARRAY_0[0xc];
	D41A0_BYTESTR_0.dword_0x235 = *(Bit32s*)&x_D41A0_BYTEARRAY_0[0x235];

	D41A0_BYTESTR_0.byte_0x218D = x_D41A0_BYTEARRAY_0[0x218D];
	D41A0_BYTESTR_0.byte_0x2190 = x_D41A0_BYTEARRAY_0[0x2190];

	D41A0_BYTESTR_0.str_2FECE.byte_0x2FED4 = x_D41A0_BYTEARRAY_0[0x2FED4];

	D41A0_BYTESTR_0.byte_0x365FC = x_D41A0_BYTEARRAY_0[0x365FC];
	D41A0_BYTESTR_0.byte_0x365FD = x_D41A0_BYTEARRAY_0[0x365FD];
	D41A0_BYTESTR_0.word_0x365E0 = *(Bit16s*)&x_D41A0_BYTEARRAY_0[0x365E0];

	D41A0_BYTESTR_0.byte_0x36E00 = x_D41A0_BYTEARRAY_0[0x36e00];
	
	memcpy(D41A0_BYTESTR_0.array_0x365F4, &x_D41A0_BYTEARRAY_0[0x365F4],0x58);
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count
	{
		memcpy(&D41A0_BYTESTR_0.array_0x2BDE[i], &x_D41A0_BYTEARRAY_0[0x2BDE + 0x84c * i], 0x84c);
	}
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count
	{
		memcpy(&D41A0_BYTESTR_0.str_2FECE.array_0x3030E[i], &x_D41A0_BYTEARRAY_0[0x3030E + 20 * i], 0x20);
	}
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count
	{
		memcpy(&D41A0_BYTESTR_0.str_2FECE.array_0x3647A[i], &x_D41A0_BYTEARRAY_0[0x3647A + 8 * i], 0x8);
	}
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count
	{
		memcpy(&D41A0_BYTESTR_0.array_0x365F4[i], &x_D41A0_BYTEARRAY_0[0x365F4+8*i], 0x8);
	}
}

void x_D41A0_BYTESTR_0_to_x_D41A0_BYTEARRAY_0()
{
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0xc]= D41A0_BYTESTR_0.word_0xc;
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x235] = D41A0_BYTESTR_0.dword_0x235;

	x_D41A0_BYTEARRAY_0[0x218D] = D41A0_BYTESTR_0.byte_0x218D;
	x_D41A0_BYTEARRAY_0[0x2190] = D41A0_BYTESTR_0.byte_0x2190;

	x_D41A0_BYTEARRAY_0[0x2FED4] = D41A0_BYTESTR_0.str_2FECE.byte_0x2FED4;
	
	x_D41A0_BYTEARRAY_0[0x365FC]= D41A0_BYTESTR_0.byte_0x365FC;
	x_D41A0_BYTEARRAY_0[0x365FD]= D41A0_BYTESTR_0.byte_0x365FD;
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0x365E0]= D41A0_BYTESTR_0.word_0x365E0;

	x_D41A0_BYTEARRAY_0[0x36e00]= D41A0_BYTESTR_0.byte_0x36E00;
	
	memcpy(&x_D41A0_BYTEARRAY_0[0x365F4], D41A0_BYTESTR_0.array_0x365F4, 0x58);
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count
	{
		memcpy(&x_D41A0_BYTEARRAY_0[0x2BDE+ 0x84c * i], &D41A0_BYTESTR_0.array_0x2BDE[i], 0x84c);
	}
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count
	{
		memcpy(&x_D41A0_BYTEARRAY_0[0x3030E + 20 * i], &D41A0_BYTESTR_0.str_2FECE.array_0x3030E[i], 0x20);
	}
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count
	{
		memcpy(&x_D41A0_BYTEARRAY_0[0x3647A + 8 * i], &D41A0_BYTESTR_0.str_2FECE.array_0x3647A[i], 0x8);
	}
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count			
	{
		memcpy(&x_D41A0_BYTEARRAY_0[0x365F4 + 8 * i], &D41A0_BYTESTR_0.array_0x365F4[i], 0x8);
	}
}*/

void testdword(Bit32s* val1, Bit32s* val2) {
	if (*val1 != *val2)
	{
		printf("x_D41A0_BYTEARRAY_0_error");
		exit(0);
	}
}

void testbyte(Bit8u* val1, Bit8u* val2) {
	if (*val1 != *val2)
	{
		printf("x_D41A0_BYTEARRAY_0_error");
		exit(0);
	}
}

Bit32s get_x_D41A0_BYTEARRAY_0_4() {
	testdword((Bit32s*)(x_D41A0_BYTEARRAY_0 + 4), &D41A0_BYTESTR_0.dword_0x4);
	return *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 4);
};
void set_x_D41A0_BYTEARRAY_0_4(Bit32s value) {
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 4) = value;
	D41A0_BYTESTR_0.dword_0x4 = value;
};

Bit32s get_x_D41A0_BYTEARRAY_0_8() {
	testdword((Bit32s*)(x_D41A0_BYTEARRAY_0 + 8), &D41A0_BYTESTR_0.dword_0x8);
	return *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8);
};
void set_x_D41A0_BYTEARRAY_0_8(Bit32s value) {
	*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) = value;
	D41A0_BYTESTR_0.dword_0x8 = value;
};

Bit8u get_x_D41A0_BYTEARRAY_0_0xe() {
	testbyte(&x_D41A0_BYTEARRAY_0[0xe], &D41A0_BYTESTR_0.byte_0xe);
	return D41A0_BYTESTR_0.byte_0xe;
};
void set_x_D41A0_BYTEARRAY_0_0xe(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0xe] = value;
	D41A0_BYTESTR_0.byte_0xe = value;
};

Bit8u get_x_D41A0_BYTEARRAY_0_0x22() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x22], &D41A0_BYTESTR_0.byte_0x22);
	return D41A0_BYTESTR_0.byte_0x22;
};
void set_x_D41A0_BYTEARRAY_0_0x22(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x22] = value;
	D41A0_BYTESTR_0.byte_0x22 = value;
};

Bit8u get_x_D41A0_BYTEARRAY_0_0x218a() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x218A], &D41A0_BYTESTR_0.byte_0x218A);
	return D41A0_BYTESTR_0.byte_0x218A;
};
void set_x_D41A0_BYTEARRAY_0_0x218a(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x218A] = value;
	D41A0_BYTESTR_0.byte_0x218A = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x218b() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x218B], &D41A0_BYTESTR_0.byte_0x218B);
	return D41A0_BYTESTR_0.byte_0x218B;
};
void set_x_D41A0_BYTEARRAY_0_0x218b(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x218b] = value;
	D41A0_BYTESTR_0.byte_0x218B = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x218c() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x218C], &D41A0_BYTESTR_0.byte_0x218C);
	return D41A0_BYTESTR_0.byte_0x218C;
};
void set_x_D41A0_BYTEARRAY_0_0x218c(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x218C] = value;
	D41A0_BYTESTR_0.byte_0x218C = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x218d() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x218d], &D41A0_BYTESTR_0.byte_0x218D);
	return D41A0_BYTESTR_0.byte_0x218D;
};
void set_x_D41A0_BYTEARRAY_0_0x218d(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x218d] = value;
	D41A0_BYTESTR_0.byte_0x218D = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x218e() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x218e], &D41A0_BYTESTR_0.byte_0x218E);
	return D41A0_BYTESTR_0.byte_0x218E;
};
void set_x_D41A0_BYTEARRAY_0_0x218e(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x218e] = value;
	D41A0_BYTESTR_0.byte_0x218E = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x218f() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x218f], &D41A0_BYTESTR_0.byte_0x218F);
	return D41A0_BYTESTR_0.byte_0x218F;
};
void set_x_D41A0_BYTEARRAY_0_0x218f(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x218f] = value;
	D41A0_BYTESTR_0.byte_0x218F = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x2190() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x2190], &D41A0_BYTESTR_0.byte_0x2190);
	return D41A0_BYTESTR_0.byte_0x2190;
};
void set_x_D41A0_BYTEARRAY_0_0x2190(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x2190] = value;
	D41A0_BYTESTR_0.byte_0x2190 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x2196() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x2196], &D41A0_BYTESTR_0.byte_0x2196);
	return D41A0_BYTESTR_0.byte_0x2196;
};
void set_x_D41A0_BYTEARRAY_0_0x2196(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x2196] = value;
	D41A0_BYTESTR_0.byte_0x2196 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x2197() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x2197], &D41A0_BYTESTR_0.byte_0x2197);
	return D41A0_BYTESTR_0.byte_0x2197;
};
void set_x_D41A0_BYTEARRAY_0_0x2197(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x2197] = value;
	D41A0_BYTESTR_0.byte_0x2197 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x2198() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x2198], &D41A0_BYTESTR_0.byte_0x2198);
	return D41A0_BYTESTR_0.byte_0x2198;
};
void set_x_D41A0_BYTEARRAY_0_0x2198(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x2198] = value;
	D41A0_BYTESTR_0.byte_0x2198 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x2199() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x2199], &D41A0_BYTESTR_0.byte_0x2199);
	return D41A0_BYTESTR_0.byte_0x2199;
};
void set_x_D41A0_BYTEARRAY_0_0x2199(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x2199] = value;
	D41A0_BYTESTR_0.byte_0x2199 = value;
};


Bit8u get_x_D41A0_BYTEARRAY_0_0x21aa() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21AA], &D41A0_BYTESTR_0.byte_0x21AA);
	return D41A0_BYTESTR_0.byte_0x21AA;
};
void set_x_D41A0_BYTEARRAY_0_0x21aa(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21AA] = value;
	D41A0_BYTESTR_0.byte_0x21AA = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21ab() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21AB], &D41A0_BYTESTR_0.byte_0x21AB);
	return D41A0_BYTESTR_0.byte_0x21AB;
};
void set_x_D41A0_BYTEARRAY_0_0x21ab(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21AB] = value;
	D41A0_BYTESTR_0.byte_0x21AB = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21ac() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21AC], &D41A0_BYTESTR_0.byte_0x21AC);
	return D41A0_BYTESTR_0.byte_0x21AC;
};
void set_x_D41A0_BYTEARRAY_0_0x21ac(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21AC] = value;
	D41A0_BYTESTR_0.byte_0x21AC = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21ad() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21AD], &D41A0_BYTESTR_0.byte_0x21AD);
	return D41A0_BYTESTR_0.byte_0x21AD;
};
void set_x_D41A0_BYTEARRAY_0_0x21ad(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21AD] = value;
	D41A0_BYTESTR_0.byte_0x21AD = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21ae() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21AE], &D41A0_BYTESTR_0.byte_0x21AE);
	return D41A0_BYTESTR_0.byte_0x21AE;
};
void set_x_D41A0_BYTEARRAY_0_0xx21ae(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21AE] = value;
	D41A0_BYTESTR_0.byte_0x21AE = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21af() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21AF], &D41A0_BYTESTR_0.byte_0x21AF);
	return D41A0_BYTESTR_0.byte_0x21AF;
};
void set_x_D41A0_BYTEARRAY_0_0x21af(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21AF] = value;
	D41A0_BYTESTR_0.byte_0x21AF = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b0() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B0], &D41A0_BYTESTR_0.byte_0x21B0);
	return D41A0_BYTESTR_0.byte_0x21B0;
};
void set_x_D41A0_BYTEARRAY_0_0x21b0(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B0] = value;
	D41A0_BYTESTR_0.byte_0x21B0 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b1() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B1], &D41A0_BYTESTR_0.byte_0x21B1);
	return D41A0_BYTESTR_0.byte_0x21B1;
};
void set_x_D41A0_BYTEARRAY_0_0x21b1(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B1] = value;
	D41A0_BYTESTR_0.byte_0x21B1 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b2() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B2], &D41A0_BYTESTR_0.byte_0x21B2);
	return D41A0_BYTESTR_0.byte_0x21B2;
};
void set_x_D41A0_BYTEARRAY_0_0x21b2(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B2] = value;
	D41A0_BYTESTR_0.byte_0x21B2 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b3() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B3], &D41A0_BYTESTR_0.byte_0x21B3);
	return D41A0_BYTESTR_0.byte_0x21B3;
};
void set_x_D41A0_BYTEARRAY_0_0x21b3(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B3] = value;
	D41A0_BYTESTR_0.byte_0x21B3 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b4() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B4], &D41A0_BYTESTR_0.byte_0x21B4);
	return D41A0_BYTESTR_0.byte_0x21B4;
};
void set_x_D41A0_BYTEARRAY_0_0x21b4(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B4] = value;
	D41A0_BYTESTR_0.byte_0x21B4 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b5() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B5], &D41A0_BYTESTR_0.byte_0x21B5);
	return D41A0_BYTESTR_0.byte_0x21B5;
};
void set_x_D41A0_BYTEARRAY_0_0x21b5(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B5] = value;
	D41A0_BYTESTR_0.byte_0x21B5 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b6() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B6], &D41A0_BYTESTR_0.byte_0x21B6);
	return D41A0_BYTESTR_0.byte_0x21B6;
};
void set_x_D41A0_BYTEARRAY_0_0x21b6(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B6] = value;
	D41A0_BYTESTR_0.byte_0x21B6 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b7() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B7], &D41A0_BYTESTR_0.byte_0x21B7);
	return D41A0_BYTESTR_0.byte_0x21B7;
};
void set_x_D41A0_BYTEARRAY_0_0x21b7(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B7] = value;
	D41A0_BYTESTR_0.byte_0x21B7 = value;
};
Bit8u get_x_D41A0_BYTEARRAY_0_0x21b8() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x21B8], &D41A0_BYTESTR_0.byte_0x21B8);
	return D41A0_BYTESTR_0.byte_0x21B8;
};
void set_x_D41A0_BYTEARRAY_0_0x21b8(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x21B8] = value;
	D41A0_BYTESTR_0.byte_0x21B8 = value;
};

Bit8u get_x_D41A0_BYTEARRAY_0_0x2BE0(int number) {
	testbyte(&x_D41A0_BYTEARRAY_0[0x84c * number + 0x2BE0], &D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x002_2BE0_11232);
	return D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x002_2BE0_11232;
};
void set_x_D41A0_BYTEARRAY_0_0x2BE0(int number, Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x84c * number + 0x2BE0] = value;
	D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x002_2BE0_11232 = value;
};

Bit8u get_x_D41A0_BYTEARRAY_0_0x36e0b() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x36E0B], &D41A0_BYTESTR_0.byte_0x36E0B);
	return D41A0_BYTESTR_0.byte_0x36E0B;
};
void set_x_D41A0_BYTEARRAY_0_0x36e0b(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x36E0B] = value;
	D41A0_BYTESTR_0.byte_0x36E0B = value;
};
void and_x_D41A0_BYTEARRAY_0_0x36e0b(Bit8u value) {
	set_x_D41A0_BYTEARRAY_0_0x36e0b(get_x_D41A0_BYTEARRAY_0_0x36e0b()&value);
};


void clean_x_D41A0_BYTEARRAY_0() {
	D41A0_BYTESTR_0.dword_0x8 = 0;
};

void errorsize(int type,int size)
{
	printf("Test x_D41A0_BYTEARRAY_0 %d %X ERROR\n",type,size);
	exit(0);
}

void test_x_D41A0_BYTEARRAY_0() {
	//test1
	if (D41A0_BYTESTR_0.dword_0x8 != *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8))errorsize(0,0);
	//test2
	if (sizeof(D41A0_BYTESTR_0) != 0x36e18)
	{
		;// errorsize(1, sizeof(D41A0_BYTESTR_0));
	}
	//test3
	for(int i=0;i< 0x36e17;i++)
		if(((Bit8u*)&D41A0_BYTESTR_0)[i]!= x_D41A0_BYTEARRAY_0[i])
			errorsize(2,i);
};