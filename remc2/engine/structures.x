int /*__cdecl*/ sub_main(int argc, char **argv, char **envp)//236F70
	void sub_5B8D0_initialize()//23c8d0
	void sub_46830_main_loop(signed __int16 *a1, signed int a2, unsigned __int16 a3)//227830
		signed int /*__fastcall*/ sub_76930(int a2, signed __int16 *a3)//257930
			int sub_76A40()//257A40
				char /*__cdecl*/ sub_779E0(int a1)//2589E0


x_DWORD_17DED4 -[34eed4]->48c73d mozna je to path struct
5e3a45000000
realzny puvod 48c80f
nastavuje se zde:0x25b110

struktura:
[0-32]-adresa
[1-16]-lenght


eax=1a6f44
eax=[eax] ...0

6f005d3f3a13
eax=[ebp-1c]...453a5e
edx=[ebp+14]..48c73d
add [edx],eax     [edx]=000 + 453a5e
eax=[ebp+14]..1a6f44
[ebp+14]+=6

5d3f3a13423a
eax=[ebp-1c]...453a5e
edx=[ebp+14]..48c743
add [edx],eax     [edx]=002 + 453a60
eax=[ebp+14]..1a6f44
[ebp+14]+=6

5d3f3a1342
eax=[ebp-1c]...453a5e
edx=[ebp+14]..48c749
add [edx],eax     [edx]=c22200000 + 455d45
eax=[ebp+14]..1a6f44
[ebp+14]+=6

zameny
x_DWORD_17DED4 za xy_DWORD_17DED4_spritestr
x_DWORD_17DEC0 za xy_DWORD_17DEC0_spritestr
x_DWORD_17DEC8 za xy_DWORD_17DEC8_spritestr

x_WORD_E1F84 - upravit na strukturu
unk_E24BCx - upravit na strukturu

x_DWORD_17DE48c - nahrazeno heap?

x_DWORD_E9B20 =

x_DWORD_E9C2C == BLOCK32DAT_BEGIN_BUFFER tj. pstr[psxadatablock32dat].var28_begin_buffer

x_DWORD_F6ED0);//2c7ed0 == TMAPS00TAB_BEGIN_BUFFER

x_D41A0_BYTEARRAY_0
*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + v9 + 0x365F5) |= 2u;
*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + v10 + 0x365F5) |= 0x20u;
*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + v11 + 0x365F5) |= 0x40u;

*(Bit16u*)(8 * v3+ &x_D41A0_BYTEARRAY_0[0x3647C]))
*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result + 0x365F6) = *(x_BYTE *)(8 * (signed __int16)result + a1 + 0x65AD)
*(_WORD *)(&x_D41A0_BYTEARRAY_0[0x30313	+ 20 * *(_WORD *)(&x_D41A0_BYTEARRAY_0[0x3647c + 8 * v6])])

if (x_D41A0_BYTEARRAY_0[8 * *(unsigned __int8 *)(a1 + 72) + 0x365F6])

i - (&x_D41A0_BYTEARRAY_0[28302])) / 168;

struct_28222{//lenght 10
byte_28222
byte_28223
byte_28224_unk
byte_28225
byte_28226
byte_28227_unk
word_28228
word_28230
}

struct_11230{//lenght 2124
byte_11232 - setting1
12149-rot1?
byte_11286 - setting2 - resolution?
12244-pos1?
12568-rot2?
12573-rot3?
12593-pos2/rot4?
byte_12286

}

struct_222539{//lenght 10
byte_222541
}

struct_222620{//lenght 11
byte_222620
}

x_D41A0_BYTEARRAY_0
{
struct_11232[8]
struct_28222[8]

struct_222539[8]
struct_222620[8]
}

test:
void sub_12100(Bit8u* a1, Bit8u* a2, char a3)//1f3100		
void sub_11EE0(Bit8u* a1)//1f2ee0