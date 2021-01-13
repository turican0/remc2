#include "engine_support_converts.h"



void convert_array_to_struct_D41A0_0(Bit8u* input, type_D41A0_BYTESTR_0* output){
}

void convert_struct_to_array_0x1AC_428(type_str_0x1AC_428* input, Bit8u* output) {
	//Bit16s word_0;
	memcpy(output + 0x0, &input->word_0, 2);
	//Bit16s word_2BDE_12658[8];
	for (int i = 0; i < 8; i++)
		memcpy(output + 0x2+2*i, &input->word_2BDE_12658[i], 2);
}


void convert_struct_to_array_164_pos(axis_2d* input, Bit8u* output) {
	//Bit16s x;
	memcpy(output + 0x0, &input->x, 2);
	//Bit16s y;
	memcpy(output + 0x2, &input->y, 2);
}

void convert_struct_to_array_164(type_str_164* input, Bit8u* output) {
	//Bit32s dword_0x0_0;
	memcpy(output + 0x0, &input->dword_0x0_0, 4);
	//Bit16s word_0x4_4;
	memcpy(output + 0x4, &input->word_0x4_4, 2);
	//Bit16s word_0x6_6;
	memcpy(output + 0x6, &input->word_0x6_6, 2);
	//Bit8u stuba[4];
	memset(output + 0xa, 0, 4);
	//Bit16s word_0xc_12;//final speed
	memcpy(output + 0xc, &input->word_0xc_12, 2);
	//Bit16s word_0xe_14;
	memcpy(output + 0xe, &input->word_0xe_14, 2);
	//Bit16s word_0x10_16;
	memcpy(output + 0x10, &input->word_0x10_16, 2);
	//Bit8u stubb[2];
	memset(output + 0x12, 0, 2);
	//type_str_164_pos position_20;//position
	convert_struct_to_array_164_pos(&input->position_backup_20, output + 0x14);
	//Bit16s word_0x18_24_next_entity;
	memcpy(output + 0x24, &input->word_0x18_24_next_entity, 2);
	//Bit16s word_0x1A_26;
	memcpy(output + 0x26, &input->word_0x1A_26, 2);
	//Bit8u stubc[2];
	memset(output + 0x1c, 0, 2);
	//Bit16s word_0x1E_30;
	memcpy(output + 0x1e, &input->word_0x1E_30, 2);
	//Bit16s word_0x20_32;
	memcpy(output + 0x20, &input->word_0x20_32, 2);
	//Bit16s word_0x22_34;
	memcpy(output + 0x22, &input->word_0x22_34, 2);
	//Bit16s word_0x24_36;
	memcpy(output + 0x24, &input->word_0x24_36, 2);
	//Bit16s word_0x26_38[8];
	for(int i=0;i<8;i++)
		memcpy(output + 0x36+2*i, &input->word_0x26_38[i], 2);
	//Bit16s word_0x36_54;
	memcpy(output + 0x36, &input->word_0x36_54, 2);
	//Bit16s word_0x38_56;
	memcpy(output + 0x36, &input->word_0x38_56, 2);
	//Bit16s word_0x3A_58;
	memcpy(output + 0x3a, &input->word_0x3A_58, 2);
	//Bit16u array_0x3C_60[16];//size??
	for (int i = 0; i < 16; i++)
		memcpy(output + 0x3c + 2 * i, &input->array_0x3C_60[i], 2);
	//Bit16u array_0x5C_92[16];
	for (int i = 0; i < 16; i++)
		memcpy(output + 0x5c + 2 * i, &input->array_0x5C_92[i], 2);
	//Bit8u stubd[174];
	memset(output + 0x7c, 0, 174);
	//Bit32s dword_0x12A_298;
	memcpy(output + 0x12a, &input->dword_0x12A_298, 4);
	//Bit32s dword_0x12E_302;
	memcpy(output + 0x12e, &input->dword_0x12E_302, 4);
	//Bit8u stube[10];
	memset(output + 0x132, 0, 10);
	//Bit32s dword_0x13C_316;
	memcpy(output + 0x13c, &input->dword_0x13C_316, 4);
	//Bit8u stubf[2];
	memset(output + 0x140, 0, 2);
	//Bit32s dword_0x142_322;
	memcpy(output + 0x142, &input->dword_0x142_322, 4);
	//Bit16s word_0x146_326;
	memcpy(output + 0x146, &input->word_0x146_326, 2);
	//Bit16s word_0x148_328;
	memcpy(output + 0x148, &input->word_0x148_328, 2);
	//Bit16s word_0x14A_330;
	memcpy(output + 0x14a, &input->word_0x14A_330, 2);
	//Bit8u byte_0x14C_332;
	memcpy(output + 0x14c, &input->byte_0x14C_332, 1);
	//Bit8s byte_0x14D_333;
	memcpy(output + 0x14d, &input->byte_0x14D_333, 1);
	//Bit8s byte_0x14E_334;
	memcpy(output + 0x14e, &input->byte_0x14E_334, 1);
	//Bit8s byte_0x14F_335;
	memcpy(output + 0x14f, &input->byte_0x14F_335, 1);
	//Bit32s byte_0x150_336;
	memcpy(output + 0x150, &input->byte_0x150_336, 1);
	//Bit8s byte_0x154_340;
	memcpy(output + 0x154, &input->byte_0x154_340, 1);
	//Bit16s word_0x155_341;
	memcpy(output + 0x155, &input->word_0x155_341, 2);
	//Bit16s word_0x157_343;
	memcpy(output + 0xc, &input->word_0x157_343, 2);
	//Bit16s word_0x159_345;
	memcpy(output + 0x159, &input->word_0x159_345, 2);
	//Bit8s array_0x15B_347[8];
	for (int i = 0; i < 8; i++)
		memcpy(output + 0x15b + i, &input->array_0x15B_347[i], 1);
	//Bit16s word_0x163_355;
	memcpy(output + 0x163, &input->word_0x163_355, 2);
	//Bit32s dword_0x165_357;
	memcpy(output + 0x165, &input->dword_0x165_357, 4);
	//Bit32s dword_0x169_361;
	memcpy(output + 0x169, &input->dword_0x169_361, 4);
	//Bit32s dword_0x16D_365;
	memcpy(output + 0x16d, &input->dword_0x16D_365, 4);
	//Bit8u stubg[4];
	memset(output + 0x171, 0, 4);
	//Bit32s dword_0x175_373;
	memcpy(output + 0x175, &input->dword_0x175_373, 4);
	//Bit32s dword_0x179_377;
	memcpy(output + 0x179, &input->dword_0x179_377, 4);
	//Bit32s dword_0x17D_381;
	memcpy(output + 0x17d, &input->dword_0x17D_381, 4);
	//Bit32s dword_0x181_385;
	memcpy(output + 0x181, &input->dword_0x181_385, 4);
	//Bit32s dword_0x185_389;
	memcpy(output + 0x185, &input->dword_0x185_389, 4);
	//Bit32s dword_0x189_393;
	memcpy(output + 0x189, &input->dword_0x189_393, 4);
	//Bit32s dword_0x18D_397;
	memcpy(output + 0x18d, &input->dword_0x18D_397, 4);
	//Bit8u stubi[4];
	memset(output + 0x191, 0, 4);
	//Bit8s byte_0x195_405;
	memcpy(output + 0x195, &input->byte_0x195_405, 1);
	//Bit8s byte_0x196_406;
	memcpy(output + 0x196, &input->byte_0x196_406, 1);
	//Bit8s byte_0x197_407;
	memcpy(output + 0x197, &input->byte_0x197_407, 1);
	//Bit8u stubj[2];
	memset(output + 0x198, 0, 2);
	//Bit32s dword_0x19A_410;
	memcpy(output + 0x19a, &input->dword_0x19A_410, 4);
	//Bit32s dword_0x19E_414;
	memcpy(output + 0x19e, &input->dword_0x19E_414, 4);
	//Bit16s word_0x1A2_418;
	memcpy(output + 0x1a2, &input->word_0x1A2_418, 2);
	//Bit16s word_0x1A4_420;
	memcpy(output + 0x1a4, &input->word_0x1A4_420, 2);
	//Bit16s word_0x1A6_422;//211 x add
	memcpy(output + 0x1a6, &input->word_0x1A6_422, 2);
	//Bit16s word_0x1A8_424;//212 y add
	memcpy(output + 0x1a8, &input->word_0x1A8_424, 2);
	//Bit16s word_0x1AA_426;//213 z add
	memcpy(output + 0x1aa, &input->word_0x1AA_426, 2);
	//type_str_0x1AC_428 str_0x1AC_428;
	convert_struct_to_array_0x1AC_428(&input->str_0x1AC_428, output + 0x1ac);
	//Bit8s byte_0x1BE_446;
	memcpy(output + 0x1be, &input->byte_0x1BE_446, 1);
	//Bit8s byte_0x1BF_447;
	memcpy(output + 0x1bf, &input->byte_0x1BF_447, 1);
	//Bit8s byte_0x1C0_448;
	memcpy(output + 0x448, &input->byte_0x1C0_448, 1);
	//Bit8s byte_0x1C1_449;//switch - event? type?
	memcpy(output + 0x449, &input->byte_0x1C1_449, 1);
	//Bit16s word_0x1C2_450;
	memcpy(output + 0x1c2, &input->word_0x1C2_450, 2);
	//Bit8u stubl[56];
	memset(output + 0x1c4, 0, 56);
	//Bit16s array_0x1FC_508[32];//0x1ca
	for (int i = 0; i < 32; i++)
		memcpy(output + 0x1fc + i*2, &input->array_0x1FC_508[i], 2);
	//Bit8u stubm[6];
	memset(output + 0x23c, 0, 6);
	//Bit16s word_0x242_578;
	memcpy(output + 0x242, &input->word_0x242_578, 2);
	//Bit16s word_0x244_580;
	memcpy(output + 0x244, &input->word_0x244_580, 2);
	//Bit16s word_0x246_582;
	memcpy(output + 0x246, &input->word_0x246_582, 2);
	//Bit16s word_0x248_584;
	memcpy(output + 0x248, &input->word_0x248_584, 2);
	//Bit16s word_0x24A_586;
	memcpy(output + 0x24a, &input->word_0x24A_586, 2);
	//Bit16s word_0x24C_588;
	memcpy(output + 0x24c, &input->word_0x24C_588, 2);
	//Bit8s array_0x24E_590[10];//size??
	for (int i = 0; i < 10; i++)
		memcpy(output + 0x24e + i, &input->array_0x24E_590[i], 1);
	//Bit8u stubn[9];
	memset(output + 0x608, 0, 9);
	//Bit8s byte_0x261_609;
	memcpy(output + 0x609, &input->byte_0x261_609, 1);
	//Bit8s byte_0x262_610;
	memcpy(output + 0x610, &input->byte_0x262_610, 1);
	//type_str_611 str_611;
}

void convert_struct_to_array_axis_2d(axis_2du* input, Bit8u* output) {
	//Bit16u x;
	memcpy(output + 0x0, &input->x, 2);
	//Bit16u y;
	memcpy(output + 0x2, &input->y, 2);
}

void convert_struct_to_array_axis_3d(axis_3d* input, Bit8u* output) {
	//Bit16u x;
	memcpy(output + 0x0, &input->x, 2);
	//Bit16u y;
	memcpy(output + 0x2, &input->y, 2);
	//Bit16u z;
	memcpy(output + 0x4, &input->z, 2);
}
void convert_struct_to_array_axis_4d(axis_4d* input, Bit8u* output) {
	//Bit16s aa;
	memcpy(output + 0x0, &input->aa, 2);
	//Bit16s bb;
	memcpy(output + 0x2, &input->xshift, 2);
	//Bit16s cc;
	memcpy(output + 0x4, &input->yshift, 2);
	//Bit16s dd;
	memcpy(output + 0x6, &input->dd, 2);
}

void convert_struct_to_array_0x1d1_2BDE_11695(type_struct_0x1d1_2BDE_11695* input, Bit8u* output) {
	//axis_3d axis_2BDE_11695;//11709 0 2 4
	convert_struct_to_array_axis_3d(&input->axis_2BDE_11695, output + 0x0);
	//axis_4d rotation__2BDE_11701;//_2BDE_11715 6 8 10 12 //aa
	convert_struct_to_array_axis_4d(&input->rotation__2BDE_11701, output + 0x6);
}

void convert_struct_to_array_0x2BDE(type_str_0x2BDE* input, Bit8u* output) {
	//dw_w_b dw_w_b_0_2BDE_11230;
	memcpy(output + 0x0, &input->dw_w_b_0_2BDE_11230, 4);
	//Bit8u byte_0x004_2BE0_11234;//2	//11234 - byte //4
	memcpy(output + 0x4, &input->byte_0x004_2BE0_11234, 1);
	//Bit8u byte_0x005_2BE0_11235;//5
	memcpy(output + 0x5, &input->byte_0x005_2BE0_11235, 1);
	//Bit8u byte_0x006_2BE4_11236;//6	//11236 - byte //6
	memcpy(output + 0x6, &input->byte_0x006_2BE4_11236, 1);
	//Bit16s word_0x007_2BE4_11237;//index of player
	memcpy(output + 0x7, &input->word_0x007_2BE4_11237, 2);
	//Bit8s byte_0x009_2BE4_11239;
	memcpy(output + 0x9, &input->byte_0x009_2BE4_11239, 1);
	//Bit16u word_0x00a_2BE4_11240;//10 //11240 - word - index z EA3E4 //0xa
	memcpy(output + 0xa, &input->word_0x00a_2BE4_11240, 2);
	//Bit8u byte_0x00c_2BE0_11242_xx;
	memcpy(output + 0xc, &input->byte_0x00c_2BE0_11242_xx, 1);
	//Bit8u byte_0x00d_2BE0_11243_xx;
	memcpy(output + 0xd, &input->byte_0x00d_2BE0_11243_xx, 1);
	//Bit16s word_0x00e_2BDE_11244; //14
	memcpy(output + 0xe, &input->word_0x00e_2BDE_11244, 2);
	//Bit16s word_0x010_2BDE_11246; //16	
	memcpy(output + 0x10, &input->word_0x010_2BDE_11246, 2);
	//Bit32s dword_0x012_2BE0_11248;//18 ??
	memcpy(output + 0x12, &input->dword_0x012_2BE0_11248, 4);
	//Bit8u byte_0x016_2BE0_11252_xx;//22
	memcpy(output + 0x16, &input->byte_0x016_2BE0_11252_xx, 1);
	//Bit8u byte_0x017_2BE0_11253_xx;//23	
	memcpy(output + 0x17, &input->byte_0x017_2BE0_11253_xx, 1);
	//Bit32s dword_0x018_2BDE_11254; //24 //pointer
	memcpy(output + 0x18, &input->dword_0x018_2BDE_11254, 4);
	//char array_0x01c_2BFA_11258[49];//28//11258 - byte* jmeno2?
	memcpy(output + 0x1c, input->array_0x01c_2BFA_11258, 49);
	//Bit16u word_0x04d_2C2B_11307;//77 //11307 - word
	memcpy(output + 0x4d, &input->word_0x04d_2C2B_11307, 2);
	//Bit16u word_0x04f_2C2D_11309;//79 //11309 - word
	memcpy(output + 0x4f, &input->word_0x04f_2C2D_11309, 2);
	//char names_81[8][48];//81 ?8
	for(int i=0;i<8;i++)
		memcpy(output + 0x51, input->names_81[i], 48);
	//type_struct_0x1d1_2BDE_11695 struct_0x1d1_2BDE_11695[0x21];//465
	for (int i = 0; i < 0x21; i++)
		convert_struct_to_array_0x1d1_2BDE_11695(&input->struct_0x1d1_2BDE_11695[i], output + 0x1d1+i*14);
	//char array_0x39f_2BFA_12157[0x40];//927//12157 - byte(11230+927) 100% name
	memcpy(output + 0x39f, input->array_0x39f_2BFA_12157, 0x40);
	//Bit8u byte_0x3DF_2BE4_12221;//991//12221 - byte
	memcpy(output + 0x3df, &input->byte_0x3DF_2BE4_12221, 1);
	//Bit8u byte_0x3E0_2BE4_12222;//992//12222 - byte
	memcpy(output + 0x3e0, &input->byte_0x3E0_2BE4_12222, 1);
	//Bit8u byte_0x3E1_2BE4_12223;//993//12223 - byte - ok
	memcpy(output + 0x3e1, &input->byte_0x3E1_2BE4_12223, 1);
	//Bit8u byte_0x3E2_2BE4_12224;//994//12224 - byte
	memcpy(output + 0x3e2, &input->byte_0x3E2_2BE4_12224, 1);
	//Bit8u byte_0x3E3_2BE4_12225;//995
	memcpy(output + 0x3e3, &input->byte_0x3E3_2BE4_12225, 1);
	//Bit8u byte_0x3E4_2BE4_12226;//996
	memcpy(output + 0x3e4, &input->byte_0x3E4_2BE4_12226, 1);
	//Bit8u stub3[1];
	memset(output + 0x3e5, 0, 1);
	//type_str_164 dword_0x3E6_2BE4_12228;//998//pointer or other	
	convert_struct_to_array_164(&input->dword_0x3E6_2BE4_12228, output + 0x3e6);
	//Bit8s byte_0x846_2BDE;//2118
	memcpy(output + 0x846, &input->byte_0x846_2BDE, 1);
	//Bit8s byte_0x847_2BDE;//2119
	memcpy(output + 0x847, &input->byte_0x847_2BDE, 2);
	//Bit16s word_0x848_2BDE;//2120
	memcpy(output + 0x848, &input->word_0x848_2BDE, 2);
	//Bit16s word_0x84A_2BDE;//2122
	memcpy(output + 0x84a, &input->word_0x84A_2BDE, 2);
}

void convert_struct_to_array_0x5E_94(type_str_0x5E_94* input, Bit8u* output) {
	//Bit32s dword_0x5E_94;//94 //36 lenght
	memcpy(output + 0x0, &input->dword_0x5E_94, 4);
	//Bit16s word_0x62_98;//98 //4 // index subentity
	memcpy(output + 0x4, &input->word_0x62_98, 2);
	//Bit32s dword_0x64_100;//100 //6
	memcpy(output + 0x6, &input->dword_0x64_100, 4);
	//Bit16s word_0x68_104;//104 //10
	memcpy(output + 0xa, &input->word_0x68_104, 2);
	//Bit8u stub5b[6];
	memset(output + 0x10, 0, 6);
	//Bit32s dword_0x70_112;//112
	memcpy(output + 0x14, &input->dword_0x70_112, 4);
	//Bit16s word_0x74_116;//116
	memcpy(output + 0x16, &input->word_0x74_116, 2);
	//Bit16s word_0x76_118;//118
	memcpy(output + 0x18, &input->word_0x76_118, 2);
	//Bit16s word_0x78_120;//120
	memcpy(output + 0x1a, &input->word_0x78_120, 2);
	//Bit16s word_0x7A_122;//122
	memcpy(output + 0x1c, &input->word_0x7A_122, 2);
	//Bit16s word_0x7C_124;//124
	memcpy(output + 0x1e, &input->word_0x7C_124, 2);
	//Bit8u stub5d[2];
	memset(output + 0x20, 0, 2);
	//Bit16u word_0x80_128;//128
	memcpy(output + 0x22, &input->word_0x80_128, 2);
}

void convert_struct_to_array_0x6E8E(type_str_0x6E8E* input, Bit8u* output) {
	//struct _str_0x6E8E* next_0;
	memcpy(output + 0, &input->next_0, 4);
	//Bit32s dword_0x4;//4 //?bitmap//LIVE
	memcpy(output + 4, &input->dword_0x4, 4);
	//Bit32s dword_0x8;//8 // this is Bit32s ?bitmap//MAX LIVE
	memcpy(output + 8, &input->dword_0x8, 4);
	//dw_w_b struct_byte_0xc_12_15;//12
	memcpy(output + 12, &input->struct_byte_0xc_12_15.dword, 4);
	//Bit32s dword_0x10_16;//16 // index of array
	memcpy(output + 16, &input->dword_0x10_16, 4);
	//Bit16u word_0x14_20;//20 //random seed adress
	memcpy(output + 20, &input->word_0x14_20, 2);
	//Bit16u word_0x16_22;//22
	memcpy(output + 22, &input->word_0x16_22, 2);
	//Bit16u word_0x18_24_next_entity;//24
	memcpy(output + 24, &input->word_0x18_24_next_entity, 2);
	//Bit16u word_0x1A_26;//26 // index - owner //ID last index
	memcpy(output + 26, &input->word_0x1A_26, 2);
	//Bit16s word_0x1C_28;//28//rotate1
	memcpy(output + 28, &input->word_0x1C_28, 2);
	//Bit16s word_0x1E_30;//30//rotate2
	memcpy(output + 30, &input->word_0x1E_30, 2);
	//Bit16s word_0x20_32;//32//rotate3
	memcpy(output + 32, &input->word_0x20_32, 2);
	//Bit16s word_0x22_34;//34//rotate4
	memcpy(output + 34, &input->word_0x22_34, 2);
	//Bit16s word_0x24_36;//36 // index subentity
	memcpy(output + 36, &input->word_0x24_36, 2);
	//Bit16s word_0x26_38;//38 // index subentity
	memcpy(output + 38, &input->word_0x26_38, 2);
	//Bit16u word_0x28_40;//40//WHO OWNS ME
	memcpy(output + 40, &input->word_0x28_40, 2);
	//Bit16u word_0x2A_42;//42 //maybe Bit16s
	memcpy(output + 42, &input->word_0x2A_42, 2);
	//Bit16s word_0x2C_44;//44 //add to z
	memcpy(output + 44, &input->word_0x2C_44, 2);
	//Bit16s word_0x2E_46;//46
	memcpy(output + 46, &input->word_0x2E_46, 2);
	//Bit16u word_0x30_48;//48
	memcpy(output + 48, &input->word_0x30_48, 2);
	//Bit16u word_0x32_50;//50
	memcpy(output + 50, &input->word_0x32_50, 2);
	//Bit16u word_0x34_52;//52 // index subentity
	memcpy(output + 52, &input->word_0x34_52, 2);
	//Bit16u word_0x36_54;//54
	memcpy(output + 54, &input->word_0x36_54, 2);
	//Bit8s byte_0x38_56;//56
	memcpy(output + 56, &input->byte_0x38_56, 1);
	//Bit8s byte_0x39_57;//57
	memcpy(output + 57, &input->byte_0x39_57, 1);
	//Bit8s byte_0x3A_58;//58
	memcpy(output + 58, &input->byte_0x3A_58, 1);
	//Bit8s byte_0x3B_59;//59
	memcpy(output + 59, &input->byte_0x3B_59, 1);
	//Bit8s byte_0x3C_60;//60
	memcpy(output + 60, &input->byte_0x3C_60, 1);
	//Bit8s byte_0x3D_61;//61
	memcpy(output + 61, &input->byte_0x3D_61, 1);
	//Bit8s byte_0x3E_62;//62 //index
	memcpy(output + 62, &input->byte_0x3E_62, 1);
	//Bit8s byte_0x3F_63;//63 //type//MODEL
	memcpy(output + 63, &input->byte_0x3F_63, 1);
	//Bit8s byte_0x40_64;//64 //subtype//CLASS
	memcpy(output + 64, &input->byte_0x40_64, 1);
	//Bit8s byte_0x41_65;//65 //type subentity
	memcpy(output + 65, &input->byte_0x41_65, 1);
	//Bit8s byte_0x42_66;//66 //subtype subentity
	memcpy(output + 66, &input->byte_0x42_66, 1);
	//Bit8s byte_0x43_67;//67
	memcpy(output + 67, &input->byte_0x43_67, 1);
	//Bit8s byte_0x44_68;//68	
	memcpy(output + 68, &input->byte_0x44_68, 1);
	//Bit8u byte_0x45_69;//69 // index of structure with lenght 14//STATE
	memcpy(output + 69, &input->byte_0x45_69, 1);
	//Bit8u byte_0x46_70;//70 // index of bitmap
	memcpy(output + 70, &input->byte_0x46_70, 1);
	//Bit8s byte_0x47_71_xx;//71
	memcpy(output + 71, &input->byte_0x47_71_xx, 1);
	//Bit8s byte_0x48_72;//72
	memcpy(output + 72, &input->byte_0x48_72, 1);
	//Bit8s byte_0x49_73;//70
	memcpy(output + 73, &input->byte_0x49_73, 1);
	//Bit16s word_0x4A_74;
	memcpy(output + 74, &input->word_0x4A_74, 2);
	//axis_3d array_0x4C_76;//position//ACTUAL X Y Z
	//axis_4d array_0x54_82;
	//Bit16s word_0x5A_90;
	memcpy(output + 90, &input->word_0x5A_90, 2);
	//Bit8s byte_0x5C_92;
	memcpy(output + 92, &input->byte_0x5C_92, 1);
	//Bit8s byte_0x5D_93;
	memcpy(output + 93, &input->byte_0x5D_93, 1);
	//type_str_0x5E_94 str_0x5E_94;
	convert_struct_to_array_0x5E_94(&input->str_0x5E_94, output + 94);
	//Bit16s word_0x82_130;//130 //add to z//ACTUAL SPEED
	memcpy(output + 130, &input->word_0x82_130, 2);
	//Bit16s word_0x84_132;//132//MIN SPEED
	memcpy(output + 132, &input->word_0x84_132, 2);
	//Bit16s word_0x86_134;//134//MAX SPEED
	memcpy(output + 134, &input->word_0x86_134, 2);
	//Bit32s dword_0x88_136;//136
	memcpy(output + 136, &input->dword_0x88_136, 4);
	//Bit32s dword_0x8C_140;//140 //wiz name?//ACTUAL MANA
	memcpy(output + 140, &input->dword_0x8C_140, 4);
	//Bit32s dword_0x90_144;//144 0-1000//MAX MANA
	memcpy(output + 144, &input->dword_0x90_144, 4);
	//Bit16u word_0x94_148;//148
	memcpy(output + 0x94, &input->word_0x94_148, 2);
	//Bit16s word_0x96_150;//150 // index subentity
	memcpy(output + 0x96, &input->word_0x96_150, 2);
	//Bit16u word_0x98_152;//152
	memcpy(output + 0x98, &input->word_0x98_152, 2);
	//axis_3d word_0x9A_154x;//154
	convert_struct_to_array_axis_3d(&input->word_0x9A_154x, output + 0x9a);
	//type_str_160* dword_0xA0_160x;//160 //special settings
	memcpy(output + 160, &input->dword_0xA0_160x, 4);
	//type_str_164* dword_0xA4_164x;//100 // adress of xx
	memcpy(output + 164, &input->dword_0xA4_164x, 4);
}

void convert_struct_to_array_0x6E3E(type_str_0x6E3E* input, Bit8u* output) {//10 lenght
	//Bit8s str_0x6E3E_byte0;//0x6E3E-28222//D41A0_BYTESTR_0.array_0x6E3E[xx].str_0x6E3E_byte0
	memcpy(output + 0, &input->str_0x6E3E_byte0, 1);
	//Bit8s str_0x6E3E_byte1;//0x6E3F-28223
	memcpy(output + 1, &input->str_0x6E3E_byte1, 1);
	//Bit8s str_0x6E3E_byte2;//0x6E40-28224
	memcpy(output + 2, &input->str_0x6E3E_byte2, 1);
	//Bit8s str_0x6E3E_byte3;//0x6E41-28225
	memcpy(output + 3, &input->str_0x6E3E_byte3, 1);
	//Bit8s str_0x6E3E_byte4;//0x6E42-28226
	memcpy(output + 4, &input->str_0x6E3E_byte4, 1);
	//Bit8s str_0x6E3E_byte5;//0x6E43-28227
	memcpy(output + 5, &input->str_0x6E3E_byte5, 1);
	//Bit16u str_0x6E3E_word6;//0x6E44-28228
	memcpy(output + 6, &input->str_0x6E3E_word6, 2);
	//Bit16u str_0x6E3E_word8;//0x6E46-28230
	memcpy(output + 8, &input->str_0x6E3E_word8, 2);
}

void convert_struct_to_array_0x30311(type_str_0x30311* input, Bit8u* output) {//lenght 20
	//Bit16u str_0x30311_type;//type_str_0x30311//type_str_0x30325//1091
	memcpy(output + 0, &input->str_0x30311_type, 2);
	//Bit16s str_0x30311_subtype;//type_str_0x30313//type_str_0x30327//1091
	memcpy(output + 2, &input->str_0x30311_subtype, 2);
	//axis_3d axis3d_4;//type_str_0x30315//type_str_0x30329
	//convert_struct_to_array_axis_3d(&input->axis3d_4, output + 0x4);
	convert_struct_to_array_axis_2d(&input->axis2d_4, output + 0x4);
	memcpy(output + 8, &input->DisId, 2);
	//Bit16u word_10;//type_str_0x3031b//type_str_0x3032f
	memcpy(output + 10, &input->word_10, 2);
	//Bit16u word_12;//type_str_0x3031d//type_str_0x30331
	memcpy(output + 12, &input->word_12, 2);
	//Bit16u word_14;//type_str_0x3031f//type_str_0x30333//1105
	memcpy(output + 14, &input->word_14, 2);
	//Bit16u word_16;//type_str_0x30321//type_str_0x30335
	memcpy(output + 16, &input->word_16, 2);
	//Bit16u word_18;//type_str_0x30323//type_str_0x30337
	memcpy(output + 18, &input->word_18, 2);
}

void convert_struct_to_array_0x360D2(type_str_0x360D2* input, Bit8u* output) {//lenght 110 //word_0x360D2
	//Bit8u stuba[3];//0
	memset(output + 0, 0, 2);
	//Bit16s word_0x360D5;//3
	memcpy(output + 3, &input->word_0x360D5, 2);
	//Bit8u stubb[2];//5
	memset(output + 5, 0, 2);
	//Bit16s word_0x360D9;//7
	memcpy(output + 7, &input->word_0x360D9, 2);
	//Bit8u stubc[2];//9
	memset(output + 9, 0, 2);
	//Bit16s word_0x360DD;//11
	memcpy(output + 11, &input->word_0x360DD, 2);
	//Bit8u stubd[2];//13
	memset(output + 13, 0, 2);
	//Bit8u byte_0x360E1x[26];//15
	memcpy(output + 15, &input->word_0x360DD, 26);
	//Bit8u byte_0x360FBx[26];//41
	memcpy(output + 41, &input->word_0x360DD, 26);
	//Bit8u byte_0x36115x[26];//67
	memcpy(output + 67, &input->word_0x360DD, 26);
	//Bit16s word_0x3612F;//93
	memcpy(output + 93, &input->word_0x3612F, 2);
	//Bit8u stubf[15];//95
	memset(output + 95, 0, 15);
}

void convert_struct_to_array_0x36442(type_str_0x36442* input, Bit8u* output) {
	//Bit8s byte_0;
	memcpy(output + 0, &input->byte_0, 1);
	//Bit16s word_1;//25973//word_0
	memcpy(output + 1, &input->word_1, 2);
	//Bit16s word_3;//25975//word_2
	memcpy(output + 3, &input->_axis_2d.x, 2);
	//Bit16s word_5; //25977//word_4
	memcpy(output + 5, &input->_axis_2d.y, 2);
}


void convert_struct_to_array_0x3647Ac(type_str_0x3647Ac* input, Bit8u* output) {//lenght 8
	//Bit8s str_0x3647A_byte_0;//array_0x3647a
	memcpy(output + 0, &input->str_0x3647A_byte_0, 1);
	//Bit8s str_0x3647A_byte_1;//array_0x3647b
	memcpy(output + 1, &input->str_0x3647A_byte_1, 1);
	//uaxis_2d str_0x3647A_2;//array_0x3647c//str_0x3647A_2.word
	memcpy(output + 2, &input->str_0x3647A_2, 2);
	//un1 str_0x3647C_4;//array_0x3647e		
	memcpy(output + 4, &input->str_0x3647C_4, 4);
}

void convert_struct_to_array_2FECE(type_str_2FECE* input, Bit8u* output) {//lenght 6604
	//type_str_0x6E8E* dword_2FECE;//D41A0_BYTESTR_0.str_2FECE.dword_2FECE
	memcpy(output + 0, &input->word_2FECE, 2);
	memcpy(output + 2, &input->word_2FED0, 2);
	//Bit8u byte_0x2FED2;//x_D41A0_BYTEARRAY_0[196306] // type of level graphics//4
	memcpy(output + 4, &input->byte_0x2FED2, 1);//0xa8
	//Bit8u stuba[1];//5
	memset(output + 5, 0, 1);
	//Bit8u byte_0x2FED4;//x_D41A0_BYTEARRAY_0[196308]//6
	memcpy(output + 6, &input->MapType, 1);
	//Bit16s word_0x2FED5;//7
	memcpy(output + 7, &input->word_0x2FED5, 2);
	//Bit16s word_0x2FED7;//9
	memcpy(output + 9, &input->word_0x2FED7, 2);
	//Bit8s array_0x2FED9[8];//17
	memcpy(output + 17, input->array_0x2FED9, 8);
	//Bit8u stubb[4];//21
	memset(output + 0x21, 0, 4);
	//Bit16u word_0x2FEE5;//2FEE5//23
	memcpy(output + 23, &input->word_0x2FEE5, 2);
	//Bit8u stubb2[2];//25
	memset(output + 0x20, 0, 2);
	//Bit16u word_0x2FEE9;//2FEE9//27 //first seed position
	memcpy(output + 5, &input->word_0x2FEE9, 2);
	//Bit8u stubc[2];//29
	memset(output + 0x20, 0, 2);
	//Bit16u word_0x2FEED;//2FEED//31 //first seed height
	memcpy(output + 31, &input->word_0x2FEED, 2);
	//Bit8u stubc2[2];//33
	memset(output + 0x20, 0, 2);
	//Bit16u word_0x2FEF1;//2FEF1//35
	memcpy(output + 35, &input->word_0x2FEF1, 2);
	//Bit8u stubc3[2];//37
	memset(output + 0x20, 0, 2);
	//Bit32u word_0x2FEF5;//2FEF5//39
	memcpy(output + 37, &input->word_0x2FEF5, 4);
	//Bit16u word_0x2FEF9;//2FEF9//43
	memcpy(output + 43, &input->word_0x2FEF9, 2);
	//Bit8u stube[2];//45
	memset(output + 45, 0, 2);
	//Bit16u word_0x2FEFD;//2FEFD//47
	memcpy(output + 47, &input->word_0x2FEFD, 2);
	//Bit8u stubf[2];//49
	memset(output + 49, 0, 2);
	//Bit16u word_0x2FF01;//2FF01//51
	memcpy(output + 51, &input->word_0x2FF01, 2);
	//Bit8u stubg[2];//53
	memset(output + 0x20, 0, 2);
	//Bit16u word_0x2FF05;//2FF05//55
	memcpy(output + 55, &input->word_0x2FF05, 2);
	//Bit8u stubh[2];//57
	memset(output + 57, 0, 2);
	//Bit16u word_0x2FF09;//2FF09//59
	memcpy(output + 59, &input->word_0x2FF09, 2);
	//Bit8u stubi[2];//61
	memset(output + 61, 0, 2);
	//Bit16u word_0x2FF0D;//2FF0D//63
	memcpy(output + 63, &input->word_0x2FF0D, 2);
	//Bit8u stubj[2];//65
	memset(output + 65, 0, 2);
	//Bit16u word_0x2FF11;//2FF11//67
	memcpy(output + 5, &input->word_0x2FF11, 2);
	//Bit8u stubk[1022];//69
	memset(output + 69, 0, 1022);
	//type_str_0x30311 array_0x30311[0x4b0];//end(next entity) - 0x360d1//1091
	for (int i = 0; i < 0x4b0; i++)
		convert_struct_to_array_0x30311(&input->array_0x30311[i], output + 1091+i*20);
	//Bit8u next_0x360D1;//6203
	memcpy(output + 6203, &input->next_0x360D1, 1);
	//type_str_0x360D2 next_0x360D2[8];//lenght 110//25092
	for (int i = 0; i < 0x8; i++)
		convert_struct_to_array_0x360D2(&input->next_0x360D2[i], output + 1091 + i * 110);
	//type_str_0x36442 str_0x36442[8];//25972
	for (int i = 0; i < 0x8; i++)
		convert_struct_to_array_0x36442(&input->str_0x36442[i], output + 25972 + i * 7);
	//type_str_0x3647Ac array_0x3647A[0xb];//8x11//26028
	for (int i = 0; i < 0xb; i++)
		convert_struct_to_array_0x3647Ac(&input->array_0x3647A[i], output + 26028 + i * 8);
} //compress level 

void convert_struct_to_array_0x364D2(type_str_0x364D2* input, Bit8u* output) {//lenght 108
	//Bit32u dword_0x364D2;//lenght 108
	memcpy(output + 0, &input->dword_0x364D2, 4);
	//Bit32u dword_0x364D6[0x1a];// lenght 0x68=104 end 0x3653e
	for (int i = 0; i < 0xb; i++)
		memcpy(output + 0+i*4, &input->dword_0x364D6[i], 4);
}

void convert_struct_to_array_3654C(type_str_3654C* input, Bit8u* output){ //size 10 count 8
	//Bit8s str_3654C_byte0;//222540
	memcpy(output + 0, &input->str_3654C_byte0, 1);
	//Bit8s str_3654D_byte1;
	memcpy(output + 1, &input->str_3654D_byte1, 1);
	//Bit16s str_3654E_word2;//objective parametres
	memcpy(output + 2, &input->str_3654E_axis.x, 2);
	//Bit16s str_36550_word4;
	memcpy(output + 4, &input->str_3654E_axis.y, 2);
	//Bit32s str_36552_dword6; //pointer sometimes
	memcpy(output + 6, &input->str_36552_un.dword, 4);
}

void convert_struct_to_array_0x3664C(type_str_0x3664C* input, Bit8u* output) {//lenght 39
	//Bit8s byte_0;
	memcpy(output + 0, &input->byte_0, 1);
	//Bit8s byte_1;
	memcpy(output + 1, &input->byte_1, 1);
	//Bit8s byte_2;
	memcpy(output + 2, &input->byte_2, 1);
	//Bit8s byte_3;
	memcpy(output + 3, &input->byte_3, 1);
	//axis_3d axis3d_4;
	convert_struct_to_array_axis_3d(&input->axis3d_4, output + 0x4);
	//type_str_0x6E8E* dword_A;
	memcpy(output + 0xa, &input->dword_A, 4);
	//Bit8u array_E[25];
	memcpy(output + 0xe, input->array_E, 25);
}

void convert_struct_to_array_D41A0_0(type_D41A0_BYTESTR_0* input,Bit8u* output) {	
	//Bit8u stub0[4];
	memset(output + 0, 0, 4);
	//Bit32s dword_0x4;
	memcpy(output + 4, &input->dword_0x4, 4);
	//Bit32s dword_0x8;
	memcpy(output + 8, &input->dword_0x8, 4);
	//Bit16s word_0xc;//player_index?
	memcpy(output + 0xc, &input->word_0xc, 2);
	//Bit16s word_0xe;
	memcpy(output + 0xe, &input->word_0xe, 2);
	//Bit8u array_0x10[0x1d]; //0x10, next 0x2d(45)
	memcpy(output + 0x10, input->array_0x10, 0x1d);
	//Bit32s dword_0x2d;//45
	memcpy(output + 0x2d, &input->dword_0x2d, 4);
	//Bit16u word_0x31;//49
	memcpy(output + 0x31, &input->word_0x31, 2);
	//Bit16u word_0x33;//51
	memcpy(output + 0x33, &input->word_0x33, 2);
	//Bit32s dword_0x35;//53//entity counter(max 1000 entity changes per step)
	memcpy(output + 0x35, &input->dword_0x35, 4);
	//Bit8u array_0x39[508];//57
	memcpy(output + 0x39, input->array_0x39, 508);
	//Bit32s dword_0x235;//act music
	memcpy(output + 0x235, &input->dword_0x235, 4);
	//Bit8u byte_0x239;//569
	memcpy(output + 0x239, &input->byte_0x239, 1);
	//Bit32s dword_0x23a;
	memcpy(output + 0x23a, &input->dword_0x23a, 4);
	//Bit32s dword_0x23e;
	memcpy(output + 0x23e, &input->dword_0x23e, 4);
	//Bit32s dword_0x242;
	memcpy(output + 0x242, &input->dword_0x242, 4);
	//type_str_0x6E8E* pointers_0x246[0x3e8];//pointers
	for (int i = 0; i < 0x3e8;i++)
		memcpy(output + 0x246+4*i, &input->pointers_0x246[i], 4);		
	//Bit32s dword_0x11e6;//second entity counter
	memcpy(output + 0x11e6, &input->dword_0x11e6, 4);
	//type_str_0x6E8E* dword_0x11EA[0x3e8];//??4586
	for (int i = 0; i < 0x3e8; i++)
		memcpy(output + 0x11ea + 4 * i, &input->dword_0x11EA[i], 4);
	//type_str_0x218A str_0x218A;
	memcpy(output + 0x218A, &input->m_GameSettings.m_Graphics, 4);
	//type_str_0x218E str_0x218E;
	memcpy(output + 0x218E, &input->m_GameSettings.m_Display, 4);
	//type_str_0x2192 str_0x2192;
	memcpy(output + 0x2192, &input->m_GameSettings.str_0x2192, 4);
	//type_str_0x2196 str_0x2196;
	memcpy(output + 0x2196, &input->m_GameSettings.str_0x2196, 4);
	//Bit32u dword_0x219A;//8602
	memcpy(output + 0x219A, &input->dword_0x219A, 4);
	//Bit32u dword_0x219E;//8606
	memcpy(output + 0x219E, &input->dword_0x219E, 4);
	//Bit32u dword_0x21A2;//8610
	memcpy(output + 0x21A2, &input->dword_0x21A2, 4);
	//Bit32u dword_0x21A6;//8614
	memcpy(output + 0x21A6, &input->dword_0x21A6, 4);
	//type_str_0x21AA str_0x21AA;
	memcpy(output + 0x21AA, &input->str_0x21AA, 4);
	//type_str_0x21AE str_0x21AE;
	memcpy(output + 0x21ae, &input->str_0x21AE, 4);
	//type_str_0x21B2 str_0x21B2;
	memcpy(output + 0x21B2, &input->str_0x21B2, 4);
	//type_str_0x21B6 str_0x21B6;
	memcpy(output + 0x21B6, &input->str_0x21B6, 4);
	//Bit8u stub3b[0x1a8];
	memset(output + 0x21ba, 0, 0x1A8);
	//axis_3d array_0x2362[8];//0x30
	for (int i = 0; i < 0x8; i++)
		convert_struct_to_array_axis_3d(&input->array_0x2362[i], output + 0x30);
	//Bit8u stub3c[0x14d];
	memset(output + 0x2392, 0, 0x14d);
	//Bit8u stub3d[0x6ff];
	memset(output + 0x24df, 0, 0x6ff);
	//type_str_0x2BDE array_0x2BDE[0x8]; //0x84c*0x8 ??
	for (int i = 0; i < 0x8; i++)
		convert_struct_to_array_0x2BDE(&input->array_0x2BDE[i], output + 0x2bde + i * 0x84C);
	//type_str_0x6E3E array_0x6E3E[8];//28222	lenght 0xa size 0x8// game events
	for (int i = 0; i < 0x8; i++)
		convert_struct_to_array_0x6E3E(&input->array_0x6E3E[i], output + 0x6e3e + i * 10);
	//type_str_0x6E8E struct_0x6E8E[0x3e8];//28302 a8*3e8 
	for (int i = 0; i < 0x8; i++)
		convert_struct_to_array_0x6E8E(&input->struct_0x6E8E[i], output + 0x6e8e + i * 0xa8);
	//type_str_2FECE str_2FECE;// a1 = &x_D41A0_BYTEARRAY_0[0x2FECE/*196302*/];//fix - size 0x6604u//compress level 
	convert_struct_to_array_2FECE(&input->str_2FECE, output + 0x2FECE);
	//type_str_0x364D2 str_0x364D2;//lenght 108
	convert_struct_to_array_0x364D2(&input->str_0x364D2, output + 0x364D2);
	//Bit16s word_0x3653E;//
	memcpy(output + 0x3653E, &input->word_0x3653E, 2);
	//Bit16s word_0x36540;//
	memcpy(output + 0x36540, &input->word_0x36540, 2);
	//Bit16s word_0x36542;//
	memcpy(output + 0x36542, &input->word_0x36542, 2);
	//Bit16s word_0x36544;//
	memcpy(output + 0x36544, &input->word_0x36544, 2);
	//Bit16s word_0x36546;//
	memcpy(output + 0x36546, &input->word_0x36546, 2);
	//Bit16s word_0x36548;//
	memcpy(output + 0x36548, &input->word_0x36548, 2);
	//Bit16s word_0x3654A;//
	memcpy(output + 0x3654A, &input->word_0x3654A, 2);
	//type_str_3654C struct_0x3654C[0x8];//size 10 count 8
	for (int i = 0; i < 0x8; i++)
		convert_struct_to_array_3654C(&input->struct_0x3654C[i], output + 0x3654C +i*10);
	//type_str_3659C struct_0x3659C[0x8];//size 11 count 8 C-0 D-1 E-2 F-3
	for (int i = 0; i < 0x8; i++)
		memcpy(output + 0x3659C+i*11, &input->struct_0x3659C[i].array_0x3659C_byte, 11);
	//type_str_0x3647Ac array_0x365F4[0xb];//8x11	//set 0x58 // end 0x3664c
	for (int i = 0; i < 0x8; i++)
		convert_struct_to_array_0x3647Ac(&input->array_0x365F4[i], output + 0x365F4 + i * 8);
	//type_str_0x3664C str_0x3664C[0x32];// lenght 0x79e//39x50 (end-0x36dea)
	for (int i = 0; i < 0x32; i++)
		convert_struct_to_array_0x3664C(&input->str_0x3664C[i], output + 0x3664C + i * 39);
	//Bit8s byte_0x36DEA_fly_asistant;
	memcpy(output + 0x36DEA, &input->byte_0x36DEA_fly_asistant, 1);
	//Bit8s byte_0x36DEB_xx;
	memcpy(output + 0x36DEB, &input->byte_0x36DEB_xx, 1);
	//Bit16s word_0x36DEC_mousex;
	memcpy(output + 0x36DEC, &input->word_0x36DEC_mousex, 2);
	//Bit16s word_0x36DEE_mousey;
	memcpy(output + 0x36DEE, &input->word_0x36DEE_mousey, 2);
	//Bit16s word_0x36DF0_mousexx;
	memcpy(output + 0x36DF0, &input->word_0x36DF0_mousexx, 2);
	//Bit32s dword_0x36DF2;//index
	memcpy(output + 0x36DF2, &input->dword_0x36DF2, 4);
	//Bit32u dword_0x36DF6;//pointer
	memcpy(output + 0x36DF6, &input->dword_0x36DF6, 4);
	//Bit16s word_0x36DFA;
	memcpy(output + 0x36DFA, &input->word_0x36DFA, 2);
	//Bit16s word_0x36DFC;
	memcpy(output + 0x36DFC, &input->word_0x36DFC, 2);
	//Bit16s word_0x36DFE;
	memcpy(output + 0x36DFE, &input->word_0x36DFE, 2);
	//Bit8u byte_0x36E00;
	memcpy(output + 0x36E00, &input->byte_0x36E00, 1);
	//Bit8u byte_0x36E01;//count objectives
	memcpy(output + 0x36E01, &input->byte_0x36E01, 1);
	//Bit8s byte_0x36E02;//temp objective
	memcpy(output + 0x36E02, &input->byte_0x36E02, 1);
	//Bit8s byte_0x36E03;
	memcpy(output + 0x36E03, &input->byte_0x36E03, 1);
	//Bit8s byte_0x36E04;
	memcpy(output + 0x36E04, &input->byte_0x36E04, 1);
	//Bit8u stub3k[0x6];
	memset(output + 0x36E05, 0, 6);
	//Bit8s byte_0x36E0B;
	memcpy(output + 0x36E0B, &input->byte_0x36E0B, 1);
	//Bit8u stubend[0xa];
};

void test_converts_D41A0_0(type_D41A0_BYTESTR_0* str) {
	int size = 224790;
	Bit8u* arr = (Bit8u*)malloc(size);

	convert_struct_to_array_D41A0_0(str, arr);
	for (int i = 0; i < size; i++)
	{
		if (arr[i] != *((Bit8u*)& str + i))
		{
			allert_error();
		}
	}

	for (int i = 0; i < size; i++)
	{
		*((Bit8u*)& str + i) = 0x81;
	}

	convert_array_to_struct_D41A0_0(arr, str);
	for (int i = 0; i < size; i++)
	{
		if (arr[i] != *((Bit8u*)& str + i))
		{
			allert_error();
		}
	}

	free(arr);
};
