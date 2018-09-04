find 0x351660;

x_WORD_180662(351662)// neni treba

void sub_46B40()

signed int sub_5BF50()//23cf50

2a9a70
//30ac2b00

eax:1c - 1a0038 -0
edx:24 - 00001000
add =1a1038

typedef struct pathstruct {
	char path[0x1c];
	Bit32s* var28_begin_buffer;//1C // asi buffer
	Bit32s* var32_end_buffer;//20 // asi buffer
	Bit32u var36_size_buffer;//24 //asi file size
	Bit32u var40_alloc_type;//28
} Pathstruct;

sub_9A32D - malloc open unpack

unk_D4350 - 2A5350

  sub_53E60_readfile_and_decompress("data/pald-0.dat", (Bit8u*)x_DWORD_EA3D8_pallette);
  sub_53E60_readfile_and_decompress("data/clrd-0.dat", (Bit8u*)x_BYTE_E8900);

  ebx 0
  esi 5332c1
  edi 2b9ee0
  ebp 355250

i++
[2BB3E0+i*4]=(i*168)+[2a51a0]+0x6E8E

ebx 2b2d38-0
esi 3532c9-c:\netherw.exe
ebp 35522c-3532c9-c:\netherw.exe

mov ebp,esp - 355228-355240-355248-355254-355280-355294-0
sup esp,38 -3551f0- -1
mov esi,[ebp+10] - 140
mov ebx,[ebp-14] - c8
push 1c
push 0
lea eax,[ebp-38] - 3551f0 - -1
push eax
call
add esp,c - 3551f0
push 1c
push 0
lea eax,[ebp-1c] - 35520c - /sound - 2f736f75
push eax
call

v8 - 355204

off_D918C 2aa18c - find 2bc394

*search
-2a9a50

pointers_buffer - off_D918C

 sub_101C0();//23CA36 - 1f11c0 //sem se to vubec nedostane
  //zkontrolovat inicializaci

  Problem1:-ok

    v3 = (x_BYTE*)xasearchd.var28_begin_buffer;//002bac30 //je to v 23c8d0,23cf50,265250,27B32d, nekde u 27b453
  result = 0;

  Problem2:

  void sub_8F935(__int16 a1, signed int a2, int a3, char *a4, unsigned __int8 a5, char a6)

  pointerstab_buffer - subproblem 

  najdi prirazeni 1a6f44 - je to v 24fb90
  sub_6EB90((unsigned int **)&off_D91EC);
  ma strukturu
  pointer
  size
  pointer
  size
  78651a00 0000 7a651a00 181a 0a661a00

        result = sub_98709(*i[0], *i[1], *i[2]);//279709 //1a6f44,1a6f86,1a6578
	//result = sub_98709(**i, *i[1], *i[2]);
	//1a6f44 000000000000020000000c0d92000000//pointers.tab
	//1a6f86 00000000000000000000000000000000
	//1a6578 0b0005a1a1a1a1a10005a1b413aaa100//pointers.dat

	1a6578:  sub_5BF50_load_psxdata() - tady se to nacita
	-call265250

	dword eb394:
	int __usercall sub_7ADE0@<eax>(int a1@<eax>, char a2)

	struktura 2aa18c:
	94c32b00 88c32b00 90c32b00 20ab2b00
	
	446f1a00 00000000
	866f1a00 00000000 78651a00 446f1a00
	78651a00 446f1a00 00000000 004850

	neco se nastavuje uz zde:
	sub_5BCC0_set_any_variables1();//23C9F2 - 23CCC0
	a to hlavni zde:
	if ( !(unsigned __int16)sub_5BF50_load_psxdata() )//23C9F7 - 23CF50 //neco se soubory asi jejich naceteni, nebo jen soubor palety

	-u pointer.tab

	*search-2a9a54

	data->pointers.dat 2bc390
	data->pointers->2bc394, 2bc388

	1)
	zkontrolovat prirazovani x_WORD_E29D8//2b39d8
	je to v sub_5BF50_load_psxdata()
	OK - inicializace palety zapisovala kam nema
	2)
	kontrola x_DWORD_180628b
		sub_728A9((void *)x_DWORD_180628b, 0x1E0u, v1);//[351628d]3aa0a4,1E0,0

3)chyab nacitani
	sub_7AA70((char*)"DATA/SCREENS/HSCREEN0.DAT", (Bit8u*)x_DWORD_17DEC0, (int)&unk_13B194, 548);

	najit puvod - 34ee48 - x_DWORD_17DE48c
	predany jako v2 v
	tj. 
	x_D41A0_BYTEARRAY_4[0xE2] -38d032

	if ( !(unsigned __int16)sub_5BF50_load_psxdata() )//23C9F7 - 23CF50 //neco se soubory asi jejich naceteni, nebo jen soubor palety
	
	je to tu:
	x_D41A0_BYTEARRAY_4[0xE2] = (Bit32s)sub_83CD0_malloc2(x_D41A0_BYTEARRAY_4[0xE6]);

	4)
	zasek v:
	v4 = sub_7F7D0((unsigned int *)&v35, (x_DWORD*)&v33, v1, (int)v3);//2607d0

	5)
	zasek v:
	sub_728A9((void *)x_DWORD_180628b, 0x1E0u, v1);//[351628d]3aa0a4,1E0,0

	6)nedefinovana x_DWORD_D41BC - 2a51bc
	na radku:
	sub_5B870((x_BYTE *)x_DWORD_D41BC, (int)x_DWORD_E9C4C, 471);//fix it

	  sprintf_s(printbuffer,512, "%c%s/%s", x_D41A0_BYTEARRAY_4[0xB6], ":/NETHERW/LANGUAGE", a4);
	  kde je a4

	  v4 = sub_7F7D0((unsigned int *)&v35, (x_DWORD*)&v33, v1, (int)v3);//2607d0
	  tj a3

	        v3 = &v20;
    else
      v3 = &v22;

	  	//35513c 355134 451414 3550d2
		7)
		nekde to prepisuje buffer x_DWORD_180628b

		8)
		doresit find first

		9)unknown_libname_4 - opravit

		10)
		uvizlo to v teto smycce:
		while ( !x_BYTE_1806E4 && !x_WORD_180746 && !x_WORD_180744 && (j___clock(v5, v3, v1) - v4) / 0x64u <= 0x14 )

		11)
		zde se vzalo silene cislo
		x_D41A0_BYTEARRAY_4[0x4]

		sprintf_s(printbuffer,512,"%c%s/L%d.txt",x_D41A0_BYTEARRAY_4[0xB6],":/NETHERW/LANGUAGE",x_D41A0_BYTEARRAY_4[0x4]);
      while ( v2 < 2 )

char aDataMsprd00Dat[18] = "data/msprd0-0.dat"; // weak
char aDataMsprd00Tab_0[18] = "data/msprd0-0.tab"; // weak
char aDataHsprd00Dat[18] = "data/hsprd0-0.dat"; // weak
char aDataHsprd00Tab_0[18] = "data/hsprd0-0.tab"; // weak

sub_8F8B0(0, 0, a1);//2708B0 super inicializace //a2 ma byt 86 nebo a1 nema nikam ukazovat

sub_8CD27(&x_D41A0_BYTEARRAY_4_0xDE_heapbuffer);//26dd27 - najit kde se nastavuje
bytearray de - [1a6578] - 0b0005a1a1a1a1


//fix it! sub_2BB40(263, 134, (Bit8u**)(v36 + 6));//20cb40
najit, kde se bere 45116 - [4527bf]->[451516]->58e2e2e2
-nacita se zde:
v4 = sub_7F7D0(&v35, &v33, v1, langfilename);//2607d0
---sub_988A7_read(langfile, x_DWORD_D41BC_langbuffer, langfilelenght - 4785);


int /*__cdecl*/ sub_7E840(signed __int16 *a1, __int16 a2, __int16 a3)//25f840
a1 - 3550e0 ->c6012a012c018d012a01004101ec00
-kde je difonovana?

sub_7FCB0 - 260cb0
56210 237210

repair this function:
void sub_8F935(Bit16u a1, signed int a2, int a3, Bit8u* a4, unsigned __int8 a5, char a6)//270935

oprava
int /*__cdecl*/ sub_81360(int a1, int a2, int a3, int a4, __int16 a5)//262360
oprava4
__int16 /*__cdecl*/ /*__spoils<>*/ sub_72633(Bit16u a1, Bit16u a2)//253633

int /*__cdecl*/ sub_81360(Bit32s a1, Bit32s a2, Bit32s a3, Bit32s a4, __int16 a5)//262360

ladeni:
void sub_8F935(Bit16u a1, signed int a2, int a3, Bit8u* a4, unsigned __int8 a5, char a6)//270935

divne -  x_WORD_E36D4 = x_WORD_E36D4;//355230

problem:
int /*__cdecl*/ sub_2BC80(__int16 a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5)

dalsi problem:

int sub_7FCB0(int a1, x_BYTE *a2, int a3, int a4, int a5, Bit8u a6, unsigned __int8 a7, Bit32u a8)//260cb0

test

void /*__cdecl*/ sub_90164(Bit16u x1, Bit16u y1, Bit16u x2, Bit16u y2, Bit8u a5)

0:
0,0,0
83-f9:
124,120,120
224:
252,0,0
228:
0,0,220

1a7358

void /*__cdecl*/ sub_7A110(char a1, char a2)//25b110

25ba70

sub_9A0FC
sub_41A90_VGA_pallette_install

vylepsit sub_90B27_VGA_command

fix this:
v8[i] = x_BYTE_181544[i] + ((unk_181B42 >> 16) * (buffer[i] - x_BYTE_181544[i]) / shadow_levels);
//352b42 352544

find
void /*__cdecl*/ sub_41A90_VGA_pallette_install(Bit8u* buffer)//222a90
and
void /*__cdecl*/ sub_A0D2C_VGA_get_pallette(Bit8u* buffer)

sub_90B27_VGA_pal_fadein_fadeout((Bit8u*)x_DWORD_17DE38, 0x20u, 0);//271b27 - tady se nacita paleta
000000 1a1a15 13120f

mozna
x_DWORD_17DE38
354f6c
x_BYTE_181544_oldpalbuffer - 352544
i - 352b44
newpalbuffer - 354f44

procedure 2589E0
34ee38->1a7358

sub_7A110(x_WORD_180660_VGA_type_resolution, 14);//25b110 - tady se paleta nastavi

x_DWORD_17DEE0 = 34eee0



hledani grafiky
x_DWORD_180628b - 351628->3aa0a4
00 00 00 00
...
00 86 00 20 86 20 20 20 20 20 09
vlozeni zde:
sub_9A144_memcopy_640line((void*)x_DWORD_E9C38_smalltit, (void*)x_DWORD_180628b, 0x1E0u);//27b144
kopie z
x_DWORD_E9C38_smalltit - 2bac38->406514

x_DWORD_E9C38_smalltit nastaveno zde:
sub_7A110(x_WORD_180660_VGA_type_resolution, 14);//25b110
kde se to nahrava do x_D41A0_BYTEARRAY_4_0xE2_heapbuffer - 91c59
x_D41A0_BYTEARRAY_4_0xE2_heapbuffer - nenacita se derenc
stale se lisi na 3aa0a4+0xf8a8, tj 3B994C(asi vlajka)
42 87 87 87 87
nahraje se opet zde:
sub_9A144_memcopy_640line((void*)x_DWORD_E9C38_smalltit, (void*)x_DWORD_180628b, 0x1E0u);//27b144
v39 = sub_7E0E0();//25f0e0 - ale vlajka prida zde
406514+0xf8a8=415DBC

repair this function:
signed int sub_7E0E0()//25f0e0

repair this
void sub_8F935(doublebyte a1, Bit16u tilex, int a3, Bit8u* a4, unsigned __int8 a5, char a6)//270935

texture - 47be3b
sub_7AA70((char*)"DATA/SCREENS/HSCREEN0.DAT", &x_D41A0_BYTEARRAY_4_0xE2_heapbuffer[0x4D54A], 168849, 102213);
sub_7AA70((char*)"DATA/SCREENS/HSCREEN0.DAT", x_DWORD_17DED4, 271062, 411);

zmensen rozsah pro sdl zobrazeno na 1px

NEW VALUE0168:0047BE3B - 0000,BA7B3C04
NEW VALUE0168:0047BE3B - BA7B3C04,BA7B3C19
NEW VALUE0168:0047BE3B - BA7B3C19,BA191919
NEW VALUE0168:0047BE3B - BA191919,191919
[34ee48]->453a5e

edx,[ebp+1c]
texture - 47ae39
48c80f -<ebp+10
esi=[esi], tj 47ae48

odladit
signed int sub_7E0E0()//25f0e0

x_DWORD_180628b_screen_buffer
texture
a dalsi
v
void sub_8F935(doublebyte a1, Bit16u tiley, int tilex, Bit8u* texture, unsigned __int8 a5, char a6)//270935
diagnostika dalsich typu spritu

najit puvod a1 v 270935
je to promenna 1a6f48
v void sub_8CD27(Pathstruct a1)//26dd27
0 0 1a6f44
spust sub_8F8B0(0, 0, a1.var28_begin_buffer[0]);//2708B0 super inicializace //a2 ma byt 86 nebo a1 nema nikam ukazovat
  

hleda se puvod:
x_WORD_E1F84 - 2b2f84 - 000000000000000000002301cd003c
-to je ok, je to definovano na zacatku
x_DWORD_17DED4 - 34eed4? - 3dc74800d7c9480000000000ffffffff4001c8
v 257a40
v 2589e0
mov [34eee6]-<si di
dal testuj toto:
sub_2BB40(x_WORD_E1F84[i + 5], x_WORD_E1F84[i + 6], &x_DWORD_17DED4[6 * v3]);
291 205 &130
48c80f cd 123
48c73d +d2= 48c80f?
48ae4700263341b6470026333abe4700

hlavni procedura-
char /*__cdecl*/ sub_779E0(int a1)//2589E0
34eed4? - void /*__cdecl*/ sub_7A110(char a1, char a2)//25b110
[34eed4]<-[[34ee48]+4d54a+38cdf] - toto zkontrolovat
x_DWORD_17DE38 = *xadatapald0dat2.var28_begin_buffer;

toto se nakrmi zde:
19b
422d6
push 2a2a18
call 25ba70

48c73d - zkontrolovat xadatapald0dat2.var28_begin_buffer:
000000000000020000005d5fc2220000

xadatapald0dat2.var28_begin_buffer
x_DWORD_EA3D8 - 2bb3d8->58731a
prirazeno v  23vf50
sub_5BF50_load_psxdata()

48c80f
void /*__cdecl*/ sub_7A110(char a1, char a2)//25b110
priradi se to v 19b 422d6 34eed4
ale pak se to jeste prepocte
ea730200 0026

jeste je treba rozlustit toto:

        if ( x_WORD_180660_VGA_type_resolution & 1 )
          sub_98709_create_index_dattab_power((Bit8u*)(int)x_DWORD_17DED4, (Bit8u*)x_DWORD_17DED8, &x_D41A0_BYTEARRAY_4_0xE2_heapbuffer[0x4D54A], (new posistruct));
        else
          sub_9874D_create_index_dattab((Bit8u*)(int)x_DWORD_17DED4, (Bit8u*)x_DWORD_17DED8, &x_D41A0_BYTEARRAY_4_0xE2_heapbuffer[0x4D54A], (new posistruct));
        
		//34eed4 34eed8 37ee48

najit problem zde:

//4527b9=[ebp+66]+6 4527bf 161545005835->58e2e2e2e2e2
sub_2BB40(263, 134, xadatalang.var28_begin_buffer[0]);//20cb40
4527bf se nakrmi v:

push eax - 3550d2 -> d2.txt
push ebx - 451514 -> 00000000000000000
push [ebp+5a] - 355134 - > 00100000000000000
push [ebp+62] - 35513c - > 00000000a4a03a00
v4 = sub_7F7D0(&v35, &v33, v1, langfilename);//2607d0
-neco v 2798a7
zbytek v 34c870

toto v char /*__cdecl*/ sub_779E0(int a1)//2589E0 by mozna melo byt Bit8u*
v1 = x_DWORD_E9C38_smalltit[307200];

int /*__cdecl*/ sub_81360(Bit32s a1, Bit32s a2, Bit32s a3, Bit32s a4, __int16 a5)//262360

dalsi problem zde:
sub_7C120(v5, v6, &x_DWORD_17DED4[6 * a5]);

v9 = &unk_E24BCx[18 * v31];//2b34bc

//loadfromsnapshot2((char*)"0160-00262360", (Bit8u*)x_DWORD_17DED4, 0x34eed4, 0x300);//3e
  

x_DWORD_17DED4 - 34eed4? - 3dc74800d7c9480000000000ffffffff4001c8
v 257a40
v 2589e0
v 25b110

x_DWORD_17DED4_spritestr - 

x_DWORD_17DED4_spritestr[0x44].sizex - tohle uz je spatne

48c73d
48c9d7

fix this procedure:
int sub_7FCB0(int a1, Bit8u* a2, Bit32s a3, Bit32s a4, int a5, Bit8u a6, unsigned __int8 a7, Bit32u a8)//260cb0
v ní opravit:
sub_7FAE0(v87, v99, a4, v98, v86);

problemek:
v12 = sub_5BE80_index_pallette(x_DWORD_17DE38, 0, 0, v86);
