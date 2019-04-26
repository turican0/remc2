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

//Bit8u* x_D41A0_BYTEARRAY_0;
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

void allert_error() {
	int a = 10;
	int b = 0;
	int c = a / b;//this is generate error
}

void support_begin() {
    readbuffer = (Bit8u*)malloc(1000000);//fix it max 64000
    printbuffer = (char*)malloc(4096);
    //printbuffer[0] = '\0';
    printbuffer2 = (char*)malloc(4096);
	pre_x_DWORD_180628b_screen_buffer = (Bit8u*)malloc(2000000);
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

//type_D41A0_BYTESTR_0 D41A0_BYTESTR_0;
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

inline void setRGBA(png_byte *ptr, Bit8u* val)
{	
		ptr[0] = val[0];
		ptr[1] = val[1];
		ptr[2] = val[2];
		ptr[3] = val[3];	
}

int writeImage(char* filename, int width, int height, Bit8u *buffer, char* title)
{
	int code = 0;
	FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep row = NULL;

	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = (png_charp)"Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep)malloc(4 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			setRGBA(&(row[x * 4]), buffer+(y*width + x)*4);
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	return code;
}


void write_posistruct_to_png(Bit8u* buffer,int width, int height, char* filename) {
	//int width = actposistruct->width;
	//int height = actposistruct->height;
	//png_bytep *row_pointers=(png_bytep*)malloc(sizeof(row_pointers)*height);
	png_bytep row = NULL;
	//Bit8u buffer[10000];

	//Bit8u* buffer_ptr = buffer;

	//Bit8u* texture= actposistruct->data;

	//Bit8u* v21_buffer_temp_index1 = buffer;
	/*
	//v21 = (char *)(dword_18062C * a2 + a3 + v6);
	Bit8s v22_loc = 0;
	Bit8s v23_loc = -1;
	Bit8s* v25_loc = 0;
	Bit8s v26_loc = 0;
	Bit8s* v27_loc = 0;
	Bit8s* v24_loc = (Bit8s*)v21_buffer_temp_index1;

	int x_DWORD_18062C_resolution_x = 11;
	int a1byte2 = 8;

	do
	{
		while (1)
		{
			while (1)
			{
				v23_loc = texture[0];
				texture++;
				if ((v23_loc & 0x80u) == 0)
					break;
				v25_loc = (Bit8s*)&v21_buffer_temp_index1[-v23_loc];
				v26_loc = texture[0];
				v27_loc = (Bit8s*)(texture + 1);
				v22_loc = v26_loc;
				memcpy(v25_loc, v27_loc, v22_loc);
				texture = (Bit8u*)&v27_loc[v22_loc];
				v21_buffer_temp_index1 = (Bit8u*)&v25_loc[v22_loc];
				v22_loc = 0;
			}
			if (!v23_loc)
				break;
			v22_loc = v23_loc;
			memcpy(v21_buffer_temp_index1, texture, v22_loc);
			texture += v22_loc;
			v21_buffer_temp_index1 += v22_loc;
			v22_loc = 0;
		}
		v24_loc += x_DWORD_18062C_resolution_x;
		v21_buffer_temp_index1 = (Bit8u*)v24_loc;
		a1byte2--;
	} while (a1byte2);
	*/
	Bit8u buffer2[10000*4];
	for (int i = 0; i < 10000; i++)
	{
		buffer2[i * 4 + 0] = buffer[i];
		buffer2[i * 4 + 1] = buffer[i];
		buffer2[i * 4 + 2] = buffer[i];
		if (buffer[i] != 0xff)buffer2[i * 4 + 3]=255;
	}
	writeImage(filename, width, height, buffer2, (char*)"test");
	/*
	int y;

	FILE *fp = fopen(filename, "wb");
	if (!fp) abort();

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();

	png_infop info = png_create_info_struct(png);
	if (!info) abort();

	if (setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
		png,
		info,
		width, height,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	png_write_info(png, info);

	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	//png_set_filler(png, 0, PNG_FILLER_AFTER);


	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep)malloc(3 * width * sizeof(png_byte));

	// Write image data
	int xx, yy;
	for (yy = 0; yy < height; yy++) {
		for (xx = 0; xx < width; xx++) {
			setRGB(&(row[xx * 3]), buffer[yy*width + xx]);
		}
		png_write_row(png, row);
	}

	// End write
	png_write_end(png, NULL);

	if (fp != NULL) fclose(fp);
	if (info != NULL) png_free_data(png, info, PNG_FREE_ALL, -1);
	if (png != NULL) png_destroy_write_struct(&png, (png_infopp)NULL);
	if (row != NULL) free(row);*/
}


void testdword(Bit32s* val1, Bit32s* val2) {
	if (*val1 != *val2)
	{
		printf("x_D41A0_BYTEARRAY_0_error");
		//allert_error();
		//exit(0);
	}
}

void testcbyte(int count,Bit8u* val1, Bit8u* val2) {
	for(int i=0;i<count;count++)
	if (val1[i] != val2[i])
	{
		printf("x_D41A0_BYTEARRAY_0_error");
		//allert_error();
		//exit(0);
	}
}

void testword(Bit16s* val1, Bit16s* val2) {
	if (*val1 != *val2)
	{
		printf("x_D41A0_BYTEARRAY_0_error");
		//allert_error();
		//exit(0);
	}
}

void testbyte(Bit8u* val1, Bit8u* val2) {
	if (*val1 != *val2)
	{
		printf("x_D41A0_BYTEARRAY_0_error");
		//allert_error();
		//exit(0);
	}
}

/*Bit32s get_x_D41A0_BYTEARRAY_0_4() {
	testdword((Bit32s*)(x_D41A0_BYTEARRAY_0 + 4), &D41A0_BYTESTR_0.dword_0x4);
	return *(Bit32s*)(x_D41A0_BYTEARRAY_0 + 4);
};
void set_x_D41A0_BYTEARRAY_0_4(Bit32s value) {
	*(Bit32s*)(x_D41A0_BYTEARRAY_0 + 4) = value;
	D41A0_BYTESTR_0.dword_0x4 = value;
};

Bit32s get_x_D41A0_BYTEARRAY_0_8() {
	testdword((Bit32s*)(x_D41A0_BYTEARRAY_0 + 8), &D41A0_BYTESTR_0.dword_0x8);
	return *(Bit32s*)(x_D41A0_BYTEARRAY_0 + 8);
};
void set_x_D41A0_BYTEARRAY_0_8(Bit32s value) {
	D41A0_BYTESTR_0.dword_0x8 = value;
	*(Bit32s*)(x_D41A0_BYTEARRAY_0 + 8) = value;
};

Bit16s get_x_D41A0_BYTEARRAY_0_0xe() {
	testword((Bit16s*)&x_D41A0_BYTEARRAY_0[0xe], &D41A0_BYTESTR_0.word_0xe);
	return D41A0_BYTESTR_0.word_0xe;
};
void set_x_D41A0_BYTEARRAY_0_0xe(Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0xe] = value;
	D41A0_BYTESTR_0.word_0xe = value;
};

Bit8s get_x_D41A0_BYTEARRAY_0_0x10(int number) {
	testbyte(&x_D41A0_BYTEARRAY_0[number + 0x10], &D41A0_BYTESTR_0.array_0x10[number]);
	return D41A0_BYTESTR_0.array_0x10[number];
};
void set_x_D41A0_BYTEARRAY_0_0x10(int number, Bit8s value) {
	x_D41A0_BYTEARRAY_0[number + 0x10] = value;
	D41A0_BYTESTR_0.array_0x10[number] = value;
};

void plus_x_D41A0_BYTEARRAY_0_0x10(int number) {
	x_D41A0_BYTEARRAY_0[number + 0x10]++;
	D41A0_BYTESTR_0.array_0x10[number] = x_D41A0_BYTEARRAY_0[number + 0x10];
};

Bit8u get_x_D41A0_BYTEARRAY_0_0x22() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x22], &D41A0_BYTESTR_0.byte_0x22);
	return D41A0_BYTESTR_0.byte_0x22;
};
void set_x_D41A0_BYTEARRAY_0_0x22(Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x22] = value;
	D41A0_BYTESTR_0.byte_0x22 = value;
};

Bit32s get_x_D41A0_BYTEARRAY_0_0x35() {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x35], &D41A0_BYTESTR_0.dword_0x35);
	return D41A0_BYTESTR_0.dword_0x35;
};
void set_x_D41A0_BYTEARRAY_0_0x35(Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x35] = value;
	D41A0_BYTESTR_0.dword_0x35 = value;
};

void plus_x_D41A0_BYTEARRAY_0_0x35() {
	(*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x35])++;
	D41A0_BYTESTR_0.dword_0x35 = *(Bit32s*)&x_D41A0_BYTEARRAY_0[0x35];
};

void minus_x_D41A0_BYTEARRAY_0_0x35() {
	(*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x35])--;
	D41A0_BYTESTR_0.dword_0x35 = *(Bit32s*)&x_D41A0_BYTEARRAY_0[0x35];
};

Bit32s get_x_D41A0_BYTEARRAY_0_235() {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x235], &D41A0_BYTESTR_0.dword_0x235);
	return D41A0_BYTESTR_0.dword_0x235;
};
void set_x_D41A0_BYTEARRAY_0_235(Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x235] = value;
	D41A0_BYTESTR_0.dword_0x235 = value;
};

Bit32s get_x_D41A0_BYTEARRAY_0_23a() {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x23a], &D41A0_BYTESTR_0.dword_0x23a);
	return D41A0_BYTESTR_0.dword_0x23a;
};
void set_x_D41A0_BYTEARRAY_0_23a(Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x23a] = value;
	D41A0_BYTESTR_0.dword_0x23a = value;
};

Bit32s get_x_D41A0_BYTEARRAY_0_23e() {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x23e], &D41A0_BYTESTR_0.dword_0x23e);
	return D41A0_BYTESTR_0.dword_0x23e;
};
void set_x_D41A0_BYTEARRAY_0_23e(Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x23e] = value;
	D41A0_BYTESTR_0.dword_0x23e = value;
};

Bit32s get_x_D41A0_BYTEARRAY_0_242() {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x242], &D41A0_BYTESTR_0.dword_0x242);
	return D41A0_BYTESTR_0.dword_0x242;
};
void set_x_D41A0_BYTEARRAY_0_242(Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x242] = value;
	D41A0_BYTESTR_0.dword_0x242 = value;
};

type_str_0x6E8E* get_x_D41A0_BYTEARRAY_0_0x246(int number) {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[4*number + 0x246], (Bit32s*)&D41A0_BYTESTR_0.pointers_0x246[number]);
	return D41A0_BYTESTR_0.pointers_0x246[number];
};
void set_x_D41A0_BYTEARRAY_0_0x246(int number, type_str_0x6E8E* value) {
	*(Bit8u**)&x_D41A0_BYTEARRAY_0[4*number + 0x246] = (Bit8u*)value;
	D41A0_BYTESTR_0.pointers_0x246[number] = value;
};

Bit32s get_x_D41A0_BYTEARRAY_0_0x11e6() {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x11e6], (Bit32s*)&D41A0_BYTESTR_0.dword_0x11e6);
	return D41A0_BYTESTR_0.dword_0x11e6;
};
void set_x_D41A0_BYTEARRAY_0_0x11e6(Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x11e6] = value;
	D41A0_BYTESTR_0.dword_0x11e6= value;
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

axis_3d get_x_D41A0_BYTEARRAY_0_0x2362(int number) {
	testcbyte(6, (Bit8u*)&x_D41A0_BYTEARRAY_0[6 * number + 0x2362], (Bit8u*)&D41A0_BYTESTR_0.array_0x2362[number]);
	//testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[6*number+0x2362], &D41A0_BYTESTR_0.array_0x2362[number]);
	return D41A0_BYTESTR_0.array_0x2362[number];
};
void set_x_D41A0_BYTEARRAY_0_0x2362(int number, axis_3d value) {
	memcpy(&x_D41A0_BYTEARRAY_0[6 * number + 0x2362],&value,6);
	//*(Bit32s*)&x_D41A0_BYTEARRAY_0[6 * number + 0x2362] = value;
	D41A0_BYTESTR_0.array_0x2362[number] = value;
};
*/
/*Bit16s get_x_D41A0_BYTEARRAY_0_0x2366(int number) {
	testword((Bit16s*)&x_D41A0_BYTEARRAY_0[6 * number + 0x2362+4], &D41A0_BYTESTR_0.array_0x2362[number].w);
	return D41A0_BYTESTR_0.array_0x2362[number].w;
};
void set_x_D41A0_BYTEARRAY_0_0x2366(int number, Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[6 * number + 0x2362+4] = value;
	D41A0_BYTESTR_0.array_0x2362[number].w = value;
};*/

void clean_x_D41A0_BYTEARRAY_0_0x2362() {
	//memset((void*)(&x_D41A0_BYTEARRAY_0[0x2362]), 0, 48);
	for (int i=0; i < 8; i++)
	{
		D41A0_BYTESTR_0.array_0x2362[i].x = 0;
		D41A0_BYTESTR_0.array_0x2362[i].y = 0;
		D41A0_BYTESTR_0.array_0x2362[i].z = 0;
	}

};

void clean_x_D41A0_BYTEARRAY_0_0x2BDE(int number) {
	//memset((void*)(&x_D41A0_BYTEARRAY_0[0x2BDE+2124*number]), 0, 2124);
	memset((void*)(&D41A0_BYTESTR_0.array_0x2BDE[number]), 0, 2124);
	/*for (int i = 0; i < 8; i++)
	{
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x000_2BDE_11230=0;//0	//11230 - byte? - ne zacatek struktury
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x001_2BDF_11231_xx = 0;
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x002_2BE0_11232 = 0;//2	//11232 - byte //2
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x003_2BE0_11233_xx = 0;
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x004_2BE0_11234 = 0;//2	//11234 - byte //4
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x005_2BE0_11235_xx = 0;
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x006_2BE4_11236 = 0;//6	//11236 - byte //6
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x007_2BE4_11237_xx = 0;
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x008_2BE4_11238_xx = 0;
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x009_2BE4_11239_xx = 0;
		D41A0_BYTESTR_0.array_0x2BDE[i].word_0x00a_2BE4_11240 = 0;//10 //11240 - word - index z EA3E4 //0xa
		for (int j = 0; j < 16; j++)
			D41A0_BYTESTR_0.array_0x2BDE[i].stub[j]=0;
		for (int j = 0; j < 49; j++)
			D41A0_BYTESTR_0.array_0x2BDE[i].array_0x01c_2BFA_11258[j]=0;//28//11258 - byte* jmeno2?
		D41A0_BYTESTR_0.array_0x2BDE[i].word_0x04d_2C2B_11307 = 0;//77 //11307 - word
		D41A0_BYTESTR_0.array_0x2BDE[i].word_0x04d_2C2D_11309 = 0;//79 //11309 - word
		for (int j = 0; j < 846; j++)
			D41A0_BYTESTR_0.array_0x2BDE[i].stub2[j];
		for (int j = 0; j < 64; j++)
			D41A0_BYTESTR_0.array_0x2BDE[i].array_0x39f_2BFA_12157[j];//927//12157 - byte(11230+927) 100% jmeno
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x3DF_2BE4_12221 = 0;//991//12221 - byte
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x3E0_2BE4_12222_xx = 0;
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x3E1_2BE4_12223 = 0;//993//12223 - byte
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x3E2_2BE4_12224 = 0;//994//12224 - byte
		for (int j = 0; j < 61; j++)
			D41A0_BYTESTR_0.array_0x2BDE[i].stub3[j];
		D41A0_BYTESTR_0.array_0x2BDE[i].byte_0x420_2BE4_12286 = 0;//1056//12286 - byte
		for (int j = 0; j < 942; j++)
			D41A0_BYTESTR_0.array_0x2BDE[i].stub4[j];	
		for (int j = 0; j < 24; j++)
			D41A0_BYTESTR_0.array_0x2BDE[i].struct_0x649_2BDE_12839.array_0x7CF_2BDE_13229[j];
		D41A0_BYTESTR_0.array_0x2BDE[i].struct_0x649_2BDE_12839.word_0x7E7_2BDE_13253=0;
		for (int j = 0; j < 99; j++)
			D41A0_BYTESTR_0.array_0x2BDE[i].stub5[j];
	}*/
};
/*
void copyto_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int number, Bit8u* value) {
	memcpy(&x_D41A0_BYTEARRAY_0[0x2BDE + 2124*number+1999], value, 24);
	memcpy(D41A0_BYTESTR_0.array_0x2BDE[number].struct_0x649_2BDE_12839.array_0x7CF_2BDE_13229, value, 24);
};

void copyto_x_D41A0_BYTEARRAY_0_0x2BDE_0x7E7(int number, Bit8u* value) {
	memcpy(&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 2023], value, 24);
	//xx D41A0_BYTESTR_0.array_0x2BDE[number].struct_0x649_2BDE_12839.word_0x7E7_2BDE_13253 = value;
	memcpy(&D41A0_BYTESTR_0.array_0x2BDE[number].struct_0x649_2BDE_12839.array_0x7CF_2BDE_13229[0x18], value, 24);
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7(int number, Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 0x7] = value;
	D41A0_BYTESTR_0.array_0x2BDE[number].word_0x007_2BE4_11237 = value;
};

Bit8s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x9(int number) {
	testbyte(&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 0x9], (Bit8u*)&D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x009_2BE4_11239);
	return D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x009_2BE4_11239;
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x9(int number, Bit8s value) {
	*(Bit8s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 0x9] = value;
	D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x009_2BE4_11239 = value;
};
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0xe(int number, Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 0xe] = value;
	D41A0_BYTESTR_0.array_0x2BDE[number].word_0x00e_2BDE_11244 = value;
};
Bit16s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x10(int number) {
	testword((Bit16s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 0x10], &D41A0_BYTESTR_0.array_0x2BDE[number].word_0x010_2BDE_11246);
	return D41A0_BYTESTR_0.array_0x2BDE[number].word_0x010_2BDE_11246;
};
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x10(int number, Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 0x10] = value;
	D41A0_BYTESTR_0.array_0x2BDE[number].word_0x010_2BDE_11246 = value;
};


void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x18(int number, Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 0x18]=value;
	D41A0_BYTESTR_0.array_0x2BDE[number].dword_0x018_2BDE_11254 = value;
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x3e1(int number, Bit8s value) {
	*(Bit8s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * number + 0x3e1] = value;
	D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x3E1_2BE4_12223 = value;
};


Bit32s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d1_2BDE_11707(int index, int subindex) {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x84c * index +0x1d1+ 0x2BDE + 0xe * subindex + 0], &D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d1_2BDE_11695);
	return D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d1_2BDE_11695;
};//465
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d1_2BDE_11707(int index, int subindex, Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * index + 0x1d1 + 0xe * subindex + 0] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d1_2BDE_11695 = value;
};//465

Bit32s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d5_2BDE_11707(int index, int subindex) {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x1d1 + 0x2BDE + 0xe * subindex + 4], &D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d5_2BDE_11699);
	return D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d5_2BDE_11699;
};//469
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d5_2BDE_11707(int index, int subindex, Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * index + 0x1d1 + 0xe * subindex + 4] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d5_2BDE_11699 = value;
};//469

Bit32s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d9_2BDE_11707(int index, int subindex) {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x1d1 + 0x2BDE + 0xe * subindex + 8], &D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d9_2BDE_11703);
	return D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d9_2BDE_11703;
};//473
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d9_2BDE_11707(int index, int subindex, Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * index + 0x1d1 + 0xe * subindex + 8] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].dword_0x1d9_2BDE_11703 = value;
};//473
Bit16s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1dd_2BDE_11707(int index, int subindex) {
	testword((Bit16s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x1d1 + 0x2BDE + 0xe*subindex+12], &D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].word_0x1dd_2BDE_11707);
	return D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].word_0x1dd_2BDE_11707;
};//477
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1dd_2BDE_11707(int index, int subindex, Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0x2BDE + 2124 * index + 0x1d1 + 0xe* subindex+12] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x1d1_2BDE_11695[subindex].word_0x1dd_2BDE_11707 = value;
};//477

Bit8s* get_x_D41A0_BYTEARRAY_0_0x2BDE_0x39f_2BFA_12157(int index) {
	return D41A0_BYTESTR_0.array_0x2BDE[index].array_0x39f_2BFA_12157;
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x5a6(int number, Bit8s value) {
	x_D41A0_BYTEARRAY_0[0x84c * number + 0x2BDE+0x5a6] = value;
	D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x5a6_2BE4_12676 = value;
};

Bit8u* get_x_D41A0_BYTEARRAY_0_0x2BDE_0x649(int number) {
	return (Bit8u*)&D41A0_BYTESTR_0.array_0x2BDE[number].struct_0x649_2BDE_12839;
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x649(int index, int subindex, Bit32s value) {
	*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE+0x649 + 4 * subindex] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.dword_0x649_2BDE[subindex] = value;
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x717(int index, int subindex, Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x717+2* subindex] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.word_0x717_2BDE[subindex] = value;
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x781(int index, int subindex, Bit8s value) {
	x_D41A0_BYTEARRAY_0[0x2BDE + 0x84c * index + 0x781 + subindex] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.byte_0x781_2BDE[subindex] = value;
};

Bit8s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x7B5(int index, int subindex) {
	testbyte(&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x7b5 + subindex], (Bit8u*)&D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.byte_0x7B5_2BDE[subindex]);
	return D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.byte_0x7B5_2BDE[subindex];
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7B5(int index, int subindex, Bit8s value) {
	x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x7b5 + subindex] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.byte_0x7B5_2BDE[subindex] = value;
};

Bit8s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int index, int subindex) {
	testbyte(&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x7cf + subindex], (Bit8u*)&D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.array_0x7CF_2BDE_13229[subindex]);
	return D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.array_0x7CF_2BDE_13229[subindex];
};
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(int index, int subindex, Bit8s value) {
	x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE+0x7cf + subindex] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.array_0x7CF_2BDE_13229[subindex] = value;
};

void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x7e8(int index, int subindex, Bit8s value) {
	x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x7e8 + subindex] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.array_0x7CF_2BDE_13229[0x18+subindex+1] = value;
};
Bit8s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x803(int index, int subindex) {
	testbyte(&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x803 + subindex], (Bit8u*)&D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.byte_0x803_2BDE[subindex]);
	return D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.byte_0x803_2BDE[subindex];
};
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x803(int index, int subindex, Bit8s value) {
	x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x803 + subindex] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.byte_0x803_2BDE[subindex] = value;
};
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x81c(int index, int subindex, Bit8s value) {
	x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x81c + subindex] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].struct_0x649_2BDE_12839.byte_0x81C_2BDE[subindex] = value;
};

Bit16s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x837(int index) {
	testword((Bit16s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE+0x837], (Bit16s*)&D41A0_BYTESTR_0.array_0x2BDE[index].word_0x837_2BDE);
	return D41A0_BYTESTR_0.array_0x2BDE[index].word_0x837_2BDE;
};
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x837(int index, Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x837] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].word_0x837_2BDE = value;
};
Bit16s get_x_D41A0_BYTEARRAY_0_0x2BDE_0x839(int index) {
	testword((Bit16s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x839], (Bit16s*)&D41A0_BYTESTR_0.array_0x2BDE[index].word_0x839_2BDE);
	return D41A0_BYTESTR_0.array_0x2BDE[index].word_0x839_2BDE;
};
void set_x_D41A0_BYTEARRAY_0_0x2BDE_0x839(int index, Bit16s value) {
	*(Bit16s*)&x_D41A0_BYTEARRAY_0[0x84c * index + 0x2BDE + 0x839] = value;
	D41A0_BYTESTR_0.array_0x2BDE[index].word_0x839_2BDE = value;
};

Bit8u get_x_D41A0_BYTEARRAY_0_0x2BDE(int number) {
	testbyte(&x_D41A0_BYTEARRAY_0[0x84c * number + 0x2BDE], &D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x002_2BE0_11232);
	return D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x002_2BE0_11232;
};
void set_x_D41A0_BYTEARRAY_0_0x2BDE(int number, Bit8u value) {
	x_D41A0_BYTEARRAY_0[0x84c * number + 0x2BDE] = value;
	D41A0_BYTESTR_0.array_0x2BDE[number].byte_0x002_2BE0_11232 = value;
};

//array_0x6E3E
Bit8s get_x_D41A0_BYTEARRAY_0_0x6E3E(int index, int subindex) {
	testbyte(&x_D41A0_BYTEARRAY_0[0x6E3E+ index*0xa+ subindex], (Bit8u*)&D41A0_BYTESTR_0.array_0x6E3E[index]);
	return D41A0_BYTESTR_0.array_0x6E3E[index][subindex];
};
void set_x_D41A0_BYTEARRAY_0_0x6E3E(int index, int subindex, Bit8s value) {
	x_D41A0_BYTEARRAY_0[0x6E3E + index*0xa+ subindex] = value;
	D41A0_BYTESTR_0.array_0x6E3E[index][subindex] = value;
};

type_str_0x6E8E* get_x_D41A0_BYTEARRAY_0_0x6E8E_adr() {
	return D41A0_BYTESTR_0.struct_0x6E8E;
};
*/
/*
Bit8u* get_x_D41A0_BYTEARRAY_0_0x6F32() {
	testdword((Bit32s*)&x_D41A0_BYTEARRAY_0[0x6F32], (Bit32s*)&D41A0_BYTESTR_0.pointer_0x6F32);
	return D41A0_BYTESTR_0.pointer_0x6F32;
};
void set_x_D41A0_BYTEARRAY_0_0x6F32(Bit8u* value) {
	*(Bit8u**)&x_D41A0_BYTEARRAY_0[0x6F32] = value;
	D41A0_BYTESTR_0.pointer_0x6F32 = value;
};

Bit8u get_x_D41A0_BYTEARRAY_0_0x36e0b() {
	testbyte(&x_D41A0_BYTEARRAY_0[0x36E0B], &D41A0_BYTESTR_0.byte_0x36E0B);
	return x_D41A0_BYTEARRAY_0[0x36E0B];
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
*/
void errorsize(int type,int size)
{
	printf("Test x_D41A0_BYTEARRAY_0 %d %X ERROR\n",type,size);
	//exit(0);
}
/*
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
		if(((Bit8u*)&D41A0_BYTESTR_0)[i]!= x_D41A0_BYTEARRAY_0[i])//((Bit8u*)&D41A0_BYTESTR_0+i)
			errorsize(2,i);
};
*/