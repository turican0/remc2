#include "engine_support_converts.h"

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
	//Bit32s dword_0x11e6;//second entity counter
	memcpy(output + 0x11e6, &input->dword_0x11e6, 4);
	//type_str_0x6E8E* dword_0x11EA[0x3e8];//??4586
	//type_str_0x218A str_0x218A;
	//type_str_0x218E str_0x218E;
	//type_str_0x2192 str_0x2192;
	//type_str_0x2196 str_0x2196;
	//Bit32u dword_0x219A;//8602
	memcpy(output + 0x219A, &input->dword_0x219A, 4);
	//Bit32u dword_0x219E;//8606
	memcpy(output + 0x219E, &input->dword_0x219E, 4);
	//Bit32u dword_0x21A2;//8610
	memcpy(output + 0x21A2, &input->dword_0x21A2, 4);
	//Bit32u dword_0x21A6;//8614
	memcpy(output + 0x21A6, &input->dword_0x21A6, 4);
	//type_str_0x21AA str_0x21AA;
	//type_str_0x21AE str_0x21AE;
	//type_str_0x21B2 str_0x21B2;
	//type_str_0x21B6 str_0x21B6;
	//Bit8u stub3b[0x1a8];
	//axis_3d array_0x2362[8];//0x30
	//Bit8u stub3c[0x14d];
	//Bit8u stub3d[0x6ff];
	//type_str_0x2BDE array_0x2BDE[0x8]; //0x84c*0x8 ??
	//type_str_0x6E3E array_0x6E3E[8];//28222	lenght 0xa size 0x8// game events
	//type_str_0x6E8E struct_0x6E8E[0x3e8];//28302 a8*3e8 
	//type_str_2FECE str_2FECE;// a1 = &x_D41A0_BYTEARRAY_0[0x2FECE/*196302*/];//fix - size 0x6604u//compress level 
	//type_str_0x364D2 str_0x364D2;//lenght 108
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
	//type_str_3659C struct_0x3659C[0x8];//size 11 count 8 C-0 D-1 E-2 F-3
	//type_str_0x3647Ac array_0x365F4[0xb];//8x11	//set 0x58 // end 0x3664c
	//type_str_0x3664C str_0x3664C[0x32];// lenght 0x79e//39x50 (end-0x36dea)
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