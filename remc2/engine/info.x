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

v12 = sub_5BE80_index_pallette(x_DWORD_17DE38, 0, 0, v86);//mozny problem

0x260ae0

v15 = x_D41A0_BYTEARRAY_4[v13 + 5]; - chyba, ma byt 07, ale je 00
x_D41A0_BYTEARRAY_4[0x1ec + 5]
48c92e
na adrese 260e55
jak jinak, nez v 25b110

zde se to naplni:
sub_7AA70((char*)"DATA/SCREENS/HSCREEN0.DAT", x_DWORD_17DED4, 271062, 411);//19b
-34eed4 tj. x_DWORD_17DED4

a je to ok:
v15 = x_DWORD_17DED4_spritestr[v13 / 6].sizey;

fix this:
void sub_7FB90(char* a1, int a2, __int16 a3, unsigned __int8 a4)//260b90
int /*__cdecl*/ sub_7C120(__int16 a1, __int16 a2, Bit8u* a3)//25d120
x_DWORD_17DEC0 - 34eec0 - neni naplnena korektne

rozdil v blitu
x02287 - prvni rozdil
zde se nastavi:
sub_2BB40(263, 134, v1_langdattab[1].pointer);//20cb40
-sub_8F8E8(a1, a2, a3);
druhy pruchod:qmemcpy(&v21_buffer_temp_index1[outindex], &texture[inindex], count);
chyba je v nastaveni x_DWORD_18062C_resolution_x(35162c) - coz ma byt 0x280 - 640

problém zde:

while ( v141 );
      }
      else if ( x_WORD_E36D4 & 0x40 )
      {
        v56 = (char *)(x_DWORD_18062C_resolution_x * tiley + tilex + pixel_buffer_index);

sub_72C40(v5, v6, x_DWORD_17DEC0_spritestr[v4[0]].pointer, a4);

test cycle:
char /*__cdecl*/ sub_779E0(int a1)//2589E0
hledam zmeny 351628->3aa0a4 - 3842212

bez vlajky
engine-memory-0160-00258C99
call 25f0e0 jinak tlacitko, bez vlajky
engine-memory-0160-00258CA1
call 20cb40 - prida vlajku
engine-memory-0160-00258CBA
call 279786 - pridano textove okno
engine-memory-0160-00258D6A
zadna zmena
engine-memory-0160-00258DDF

x_DWORD_17DEE4 -mouse info

Bit32u /*__cdecl*/ sub_7FAE0_draw_text(char* a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5)//260ae0
-problem s a5

*(x_DWORD *)&v86 = (int32)&v87[strlen(v87)]; - fix v87

chyba je uz v retezu: na adrese
0x251c3 - 152003 323x237 0x143 x ed 
386x289 x182 x x121

-dosbox 0x3d74a6


void __usercall sub_8F935(__int16 a1@<dx>, signed int a2@<ecx>, int a3@<ebx>, char *a4@<esi>, unsigned __int8 a5, char a6)
{
  int v6; // edi
  _BYTE *v7; // edi
  int v8; // ecx
  signed int v9; // eax
  _BYTE *v10; // ebx
  char v11; // al
  char v12; // al
  char v13; // al
  _BYTE *v14; // edi
  int v15; // ecx
  signed int v16; // eax
  _BYTE *v17; // ebx
  char v18; // al
  char v19; // al
  char v20; // al
  char *v21; // edi
  unsigned int v22; // ecx
  signed int v23; // eax
  char *v24; // ebx
  char *v25; // edi
  char v26; // al
  char *v27; // esi
  char *v28; // edi
  unsigned int v29; // ecx
  signed int v30; // eax
  char *v31; // ebx
  char *v32; // edi
  char v33; // al
  char *v34; // esi
  int v35; // ebp
  _BYTE *v36; // edi
  int v37; // ecx
  _BYTE *v38; // ebx
  unsigned __int8 v39; // al
  int v40; // eax
  _BYTE *v41; // edi
  int v42; // ecx
  unsigned __int8 i; // dl
  char v44; // al
  char v45; // al
  unsigned __int8 v46; // of
  char v47; // dl
  unsigned __int8 v48; // al
  char *v49; // edi
  char v50; // al
  char v51; // dl
  char v52; // al
  char v53; // dl
  unsigned int v54; // ebx
  const void *v55; // esi
  char *v56; // edi
  unsigned int v57; // ecx
  signed int v58; // eax
  char *v59; // ebx
  char *v60; // edi
  char *v61; // edi
  unsigned int v62; // ecx
  signed int v63; // eax
  char *v64; // ebx
  char *v65; // edi
  char v66; // al
  char *v67; // esi
  int v68; // edi
  int v69; // ebx
  _BYTE *v70; // edi
  _BYTE *v71; // edx
  int v72; // ecx
  char v73; // al
  char v74; // al
  _BOOL1 v75; // zf
  _BOOL1 v76; // sf
  char v77; // al
  char v78; // al
  int v79; // ebx
  _BYTE *v80; // edi
  int v81; // ecx
  signed int v82; // eax
  _BYTE *v83; // ebx
  char v84; // al
  int v85; // ebx
  _BYTE *v86; // edi
  int v87; // ecx
  signed int v88; // eax
  _BYTE *v89; // ebx
  char v90; // al
  _BYTE *v91; // edi
  int v92; // ecx
  int v93; // ebx
  signed int v94; // eax
  _BYTE *v95; // ebx
  char v96; // al
  unsigned int v97; // ebx
  _BYTE *v98; // edi
  _BYTE *v99; // edx
  int v100; // ecx
  char v101; // al
  char v102; // al
  char v103; // al
  char v104; // al
  _BYTE *v105; // edi
  int v106; // ecx
  int v107; // ebx
  _BYTE *v108; // ebx
  signed int v109; // eax
  char v110; // al
  _BYTE *v111; // edi
  int v112; // ecx
  int v113; // ebx
  _BYTE *v114; // ebx
  signed int v115; // eax
  char v116; // al
  unsigned int v117; // ebx
  _BYTE *v118; // edi
  _BYTE *v119; // edx
  int v120; // ecx
  char v121; // al
  char v122; // al
  char v123; // al
  char v124; // al
  int v125; // eax
  int v126; // edi
  int v127; // eax
  int v128; // eax
  int v129; // eax
  int v130; // eax
  char *v131; // [esp-4h] [ebp-Ch]
  unsigned __int8 v132; // [esp+2h] [ebp-6h]
  unsigned __int8 v133; // [esp+2h] [ebp-6h]
  unsigned __int8 v134; // [esp+2h] [ebp-6h]
  unsigned __int8 v135; // [esp+2h] [ebp-6h]
  unsigned __int8 v136; // [esp+2h] [ebp-6h]
  unsigned __int8 v137; // [esp+2h] [ebp-6h]
  unsigned __int8 v138; // [esp+2h] [ebp-6h]
  unsigned __int8 v139; // [esp+2h] [ebp-6h]
  char v140; // [esp+3h] [ebp-5h]
  char v141; // [esp+3h] [ebp-5h]
  _BYTE *v142; // [esp+4h] [ebp-4h]
  _BYTE *v143; // [esp+4h] [ebp-4h]

  if ( !HIBYTE(a1) )
    return;
  v6 = dword_180628 + dword_18063C + dword_18062C * dword_180650;
  if ( word_180660 & 1 )
  {
    LOBYTE(a1) = (unsigned __int8)a1 >> 1;
    HIBYTE(a1) >>= 1;
    a3 >>= 1;
    a2 >>= 1;
  }
  if ( a2 < 0 )
  {
    if ( word_E36D4 & 2 )
    {
      v128 = a2 + HIBYTE(a1);
      v46 = __OFADD__(1, v128);
      v129 = v128 + 1;
      if ( (unsigned __int8)((v129 < 0) ^ v46) | (v129 == 0) )
        return;
      a2 = -1;
      HIBYTE(a1) = v129;
    }
    else
    {
      v130 = a2 + HIBYTE(a1);
      if ( (unsigned __int8)((a2 + HIBYTE(a1) < 0) ^ __OFADD__(a2, HIBYTE(a1))) | (a2 + HIBYTE(a1) == 0) )
        return;
      HIBYTE(a1) += a2;
      BYTE1(v130) = 0;
      do
      {
        while ( 1 )
        {
          LOBYTE(v130) = *a4++;
          if ( !(_BYTE)v130 )
            break;
          if ( (v130 & 0x80u) == 0 )
            a4 += v130;
        }
        ++a2;
      }
      while ( a2 );
    }
  }
  else if ( a2 + HIBYTE(a1) >= dword_180644 )
  {
    if ( word_E36D4 & 2 )
    {
      if ( a2 + 1 >= dword_180644 )
        return;
      v125 = HIBYTE(a1) + a2 + 1 - dword_180644;
      HIBYTE(a1) = dword_180644 - (a2 + 1);
      v126 = v125;
      v127 = 0;
      do
      {
        while ( 1 )
        {
          LOBYTE(v127) = *a4++;
          if ( !(_BYTE)v127 )
            break;
          if ( (v127 & 0x80u) == 0 )
            a4 += v127;
        }
        --v126;
      }
      while ( v126 );
      v6 = dword_180628;
    }
    else
    {
      if ( dword_180644 <= a2 )
        return;
      HIBYTE(a1) = dword_180644 - a2;
    }
  }
  if ( a3 >= 0 )
  {
    if ( a3 + (unsigned __int8)a1 >= dword_180648 )
    {
      if ( word_E36D4 )
      {
        if ( word_E36D4 & 1 )
        {
          if ( word_E36D4 & 2 )
          {
            if ( dword_180634 - a3 >= 0 )
            {
              v79 = (unsigned __int8)a1 + a3;
              v80 = (_BYTE *)(dword_18062C * (HIBYTE(a1) + a2) + v79 + v6);
              v81 = 0;
              LOBYTE(v79) = dword_180634 - v79 - 2;
              LOBYTE(a1) = v79;
              v133 = v79;
              v82 = -1;
              v83 = v80;
              do
              {
                while ( 1 )
                {
                  while ( 1 )
                  {
                    LOBYTE(v82) = *a4++;
                    if ( (v82 & 0x80u) == 0 )
                      break;
                    v80 += v82;
                    LOBYTE(a1) = a1 - v82;
                  }
                  if ( !(_BYTE)v82 )
                    break;
                  LOBYTE(v81) = v82;
                  do
                  {
                    v84 = *a4++;
                    LOBYTE(a1) = a1 + 1;
                    if ( (a1 & 0x80u) == 0 )
                      *v80 = v84;
                    --v80;
                    --v81;
                  }
                  while ( v81 );
                }
                v83 -= dword_18062C;
                v80 = v83;
                a1 = __PAIR__(HIBYTE(a1), v133) - 256;
              }
              while ( HIBYTE(a1) );
            }
          }
          else if ( dword_180634 - a3 >= 0 )
          {
            v85 = (unsigned __int8)a1 + a3;
            v86 = (_BYTE *)(dword_18062C * a2 + v85 + v6);
            v87 = 0;
            LOBYTE(v85) = dword_180634 - v85 - 2;
            LOBYTE(a1) = v85;
            v134 = v85;
            v88 = -1;
            v89 = v86;
            do
            {
              while ( 1 )
              {
                while ( 1 )
                {
                  LOBYTE(v88) = *a4++;
                  if ( (v88 & 0x80u) == 0 )
                    break;
                  v86 += v88;
                  LOBYTE(a1) = a1 - v88;
                }
                if ( !(_BYTE)v88 )
                  break;
                LOBYTE(v87) = v88;
                do
                {
                  v90 = *a4++;
                  LOBYTE(a1) = a1 + 1;
                  if ( (a1 & 0x80u) == 0 )
                    *v86 = v90;
                  --v86;
                  --v87;
                }
                while ( v87 );
              }
              v89 += dword_18062C;
              v86 = v89;
              a1 = __PAIR__(HIBYTE(a1), v134) - 256;
            }
            while ( HIBYTE(a1) );
          }
        }
        else if ( word_E36D4 & 2 )
        {
          v91 = (_BYTE *)(dword_18062C * (HIBYTE(a1) + a2) + a3 + v6);
          v92 = 0;
          v93 = dword_180634 - a3;
          if ( v93 >= 0 )
          {
            LOBYTE(a1) = v93;
            v135 = v93;
            v94 = -1;
            v95 = v91;
            do
            {
              while ( 1 )
              {
                while ( 1 )
                {
                  LOBYTE(v94) = *a4++;
                  if ( (v94 & 0x80u) == 0 )
                    break;
                  v91 -= v94;
                  LOBYTE(a1) = v94 + a1;
                }
                if ( !(_BYTE)v94 )
                  break;
                LOBYTE(v92) = v94;
                do
                {
                  v96 = *a4++;
                  LOBYTE(a1) = a1 - 1;
                  if ( (a1 & 0x80u) == 0 )
                    *v91++ = v96;
                  --v92;
                }
                while ( v92 );
              }
              v95 -= dword_18062C;
              v91 = v95;
              a1 = __PAIR__(HIBYTE(a1), v135) - 256;
            }
            while ( HIBYTE(a1) );
          }
        }
      }
      else
      {
        v68 = a3 + v6;
        v69 = dword_180634 - a3;
        if ( v69 >= 0 )
        {
          v70 = (_BYTE *)(dword_18062C * a2 + v68);
          BYTE1(v69) = HIBYTE(a1);
          v132 = v69;
          v71 = v70;
          v72 = 0;
          do
          {
            while ( 1 )
            {
              while ( 1 )
              {
                v73 = *a4++;
                if ( v73 >= 0 )
                  break;
                v46 = __OFADD__(v73, (_BYTE)v69);
                v75 = v73 + (_BYTE)v69 == 0;
                v76 = (char)(v73 + v69) < 0;
                LOBYTE(v69) = v73 + v69;
                if ( !((unsigned __int8)(v76 ^ v46) | (unsigned __int8)v75) )
                {
                  v70 -= v73;
                  v77 = *a4++;
                  LOBYTE(v72) = v77;
                  do
                  {
                    v78 = *a4++;
                    LOBYTE(v69) = v69 - 1;
                    if ( (v69 & 0x80u) == 0 )
                      *v70++ = v78;
                    --v72;
                  }
                  while ( v72 );
                }
              }
              if ( !v73 )
                break;
              LOBYTE(v72) = v73;
              do
              {
                v74 = *a4++;
                LOBYTE(v69) = v69 - 1;
                if ( (v69 & 0x80u) == 0 )
                  *v70++ = v74;
                --v72;
              }
              while ( v72 );
            }
            v71 += dword_18062C;
            v70 = v71;
            LOWORD(v69) = __PAIR__(BYTE1(v69), v132) - 256;
          }
          while ( BYTE1(v69) );
        }
      }
    }
    else if ( word_E36D4 & 1 )
    {
      if ( word_E36D4 & 2 )
      {
        v7 = (_BYTE *)(dword_18062C * (HIBYTE(a1) + a2) + (unsigned __int8)a1 + a3 + v6);
        v8 = 0;
        v9 = -1;
        v10 = v7;
        do
        {
          while ( 1 )
          {
            while ( 1 )
            {
              LOBYTE(v9) = *a4++;
              if ( (v9 & 0x80u) == 0 )
                break;
              v7 += v9;
              v12 = *a4++;
              LOBYTE(v8) = v12;
              do
              {
                v13 = *a4++;
                *v7-- = v13;
                --v8;
              }
              while ( v8 );
            }
            if ( !(_BYTE)v9 )
              break;
            LOBYTE(v8) = v9;
            do
            {
              v11 = *a4++;
              *v7-- = v11;
              --v8;
            }
            while ( v8 );
          }
          v10 -= dword_18062C;
          v7 = v10;
          --HIBYTE(a1);
        }
        while ( HIBYTE(a1) );
      }
      else
      {
        v14 = (_BYTE *)(dword_18062C * a2 + (unsigned __int8)a1 + a3 + v6);
        v15 = 0;
        v16 = -1;
        v17 = v14;
        do
        {
          while ( 1 )
          {
            while ( 1 )
            {
              LOBYTE(v16) = *a4++;
              if ( (v16 & 0x80u) == 0 )
                break;
              v14 += v16;
              v19 = *a4++;
              LOBYTE(v15) = v19;
              do
              {
                v20 = *a4++;
                *v14-- = v20;
                --v15;
              }
              while ( v15 );
            }
            if ( !(_BYTE)v16 )
              break;
            LOBYTE(v15) = v16;
            do
            {
              v18 = *a4++;
              *v14-- = v18;
              --v15;
            }
            while ( v15 );
          }
          v17 += dword_18062C;
          v14 = v17;
          --HIBYTE(a1);
        }
        while ( HIBYTE(a1) );
      }
    }
    else if ( word_E36D4 )
    {
      if ( word_E36D4 & 2 )
      {
        v28 = (char *)(dword_18062C * (HIBYTE(a1) + a2) + a3 + v6);
        v29 = 0;
        v30 = -1;
        v31 = v28;
        do
        {
          while ( 1 )
          {
            while ( 1 )
            {
              LOBYTE(v30) = *a4++;
              if ( (v30 & 0x80u) == 0 )
                break;
              v32 = &v28[-v30];
              v33 = *a4;
              v34 = a4 + 1;
              LOBYTE(v29) = v33;
              qmemcpy(v32, v34, v29);
              a4 = &v34[v29];
              v28 = &v32[v29];
              v29 = 0;
            }
            if ( !(_BYTE)v30 )
              break;
            LOBYTE(v29) = v30;
            qmemcpy(v28, a4, v29);
            a4 += v29;
            v28 += v29;
            v29 = 0;
          }
          v31 -= dword_18062C;
          v28 = v31;
          --HIBYTE(a1);
        }
        while ( HIBYTE(a1) );
      }
      else if ( word_E36D4 & 4 )
      {
        v35 = dword_E3890;
        v36 = (_BYTE *)(dword_18062C * a2 + a3 + v6);
        v37 = 0;
        v38 = v36;
        do
        {
          while ( 1 )
          {
            while ( 1 )
            {
              v39 = *a4++;
              if ( (v39 & 0x80u) == 0 )
                break;
              v36 -= (char)v39;
            }
            if ( !v39 )
              break;
            LOBYTE(v37) = v39;
            v40 = v39;
            do
            {
              LOBYTE(v40) = *a4++;
              BYTE1(v40) = *v36;
              *v36++ = *(_BYTE *)(v40 + v35);
              --v37;
            }
            while ( v37 );
          }
          v38 += dword_18062C;
          v36 = v38;
          --HIBYTE(a1);
        }
        while ( HIBYTE(a1) );
      }
      else if ( word_E36D4 & 8 )
      {
        v41 = (_BYTE *)(dword_18062C * a2 + a3 + v6);
        v42 = 0;
        v142 = v41;
        v140 = HIBYTE(a1);
        for ( i = a5; ; i = a5 )
        {
          while ( 1 )
          {
            while ( 1 )
            {
              v44 = *a4++;
              if ( v44 >= 0 )
                break;
              v42 = (unsigned __int8)-v44;
              do
              {
                v46 = __OFSUB__(i--, 1);
                if ( ((i & 0x80u) != 0) ^ v46 )
                {
                  i = a5;
                  ++v41;
                }
                --v42;
              }
              while ( v42 );
            }
            if ( !v44 )
              break;
            LOBYTE(v42) = v44;
            do
            {
              v45 = *a4++;
              v46 = __OFSUB__(i--, 1);
              if ( ((i & 0x80u) != 0) ^ v46 )
              {
                i = a5;
                *v41++ = v45;
              }
              --v42;
            }
            while ( v42 );
          }
          if ( !--v140 )
            break;
          v47 = a6;
          while ( --v47 >= 0 )
          {
            while ( 1 )
            {
              do
                v48 = *a4++;
              while ( (v48 & 0x80u) != 0 );
              if ( !v48 )
                break;
              a4 += v48;
            }
          }
          v142 += dword_18062C;
          v41 = v142;
        }
      }
      else if ( word_E36D4 & 0x20 )
      {
        v49 = (char *)(dword_18062C * a2 + a3 + v6);
        v143 = v49;
        v141 = HIBYTE(a1);
        do
        {
          while ( 1 )
          {
            while ( 1 )
            {
              v50 = *a4++;
              if ( v50 >= 0 )
                break;
              v49 += (unsigned __int16)(a5 * (unsigned __int8)-v50);
            }
            if ( !v50 )
              break;
            v51 = v50;
            do
            {
              v52 = *a4++;
              memset(v49, v52, a5);
              v49 += a5;
              v46 = __OFSUB__(v51--, 1);
            }
            while ( !((unsigned __int8)((v51 < 0) ^ v46) | (v51 == 0)) );
          }
          v53 = a6 - 1;
          if ( a6 > 1 )
          {
            v131 = a4;
            v54 = v49 - v143;
            do
            {
              v55 = v143;
              v143 += dword_18062C;
              qmemcpy(v143, v55, v54);
              v46 = __OFSUB__(v53--, 1);
            }
            while ( !((unsigned __int8)((v53 < 0) ^ v46) | (v53 == 0)) );
            a4 = v131;
          }
          v143 += dword_18062C;
          v49 = v143;
          --v141;
        }
        while ( v141 );
      }
      else if ( word_E36D4 & 0x40 )
      {
        v56 = (char *)(dword_18062C * a2 + a3 + v6);
        v57 = 0;
        v58 = -1;
        v59 = v56;
        do
        {
          while ( 1 )
          {
            while ( 1 )
            {
              LOBYTE(v58) = *a4++;
              if ( (v58 & 0x80u) == 0 )
                break;
              v60 = &v56[-v58];
              LOBYTE(v57) = *a4;
              a4 += v57 + 1;
              memset(v60, a5, v57);
              v56 = &v60[v57];
              v57 = 0;
            }
            if ( !(_BYTE)v58 )
              break;
            LOBYTE(v57) = v58;
            a4 += v57;
            memset(v56, a5, v57);
            v56 += v57;
            v57 = 0;
          }
          v59 += dword_18062C;
          v56 = v59;
          --HIBYTE(a1);
        }
        while ( HIBYTE(a1) );
      }
      else
      {
        v61 = (char *)(dword_18062C * a2 + a3 + v6);
        v62 = 0;
        v63 = -1;
        v64 = v61;
        do
        {
          while ( 1 )
          {
            while ( 1 )
            {
              LOBYTE(v63) = *a4++;
              if ( (v63 & 0x80u) == 0 )
                break;
              v65 = &v61[-v63];
              v66 = *a4;
              v67 = a4 + 1;
              LOBYTE(v62) = v66;
              qmemcpy(v65, v67, v62);
              a4 = &v67[v62];
              v61 = &v65[v62];
              v62 = 0;
            }
            if ( !(_BYTE)v63 )
              break;
            LOBYTE(v62) = v63;
            qmemcpy(v61, a4, v62);
            a4 += v62;
            v61 += v62;
            v62 = 0;
          }
          v64 += dword_18062C;
          v61 = v64;
          --HIBYTE(a1);
        }
        while ( HIBYTE(a1) );
      }
    }
    else
    {
      v21 = (char *)(dword_18062C * a2 + a3 + v6);
      v22 = 0;
      v23 = -1;
      v24 = v21;
      do
      {
        while ( 1 )
        {
          while ( 1 )
          {
            LOBYTE(v23) = *a4++;
            if ( (v23 & 0x80u) == 0 )
              break;
            v25 = &v21[-v23];
            v26 = *a4;
            v27 = a4 + 1;
            LOBYTE(v22) = v26;
            qmemcpy(v25, v27, v22);
            a4 = &v27[v22];
            v21 = &v25[v22];
            v22 = 0;
          }
          if ( !(_BYTE)v23 )
            break;
          LOBYTE(v22) = v23;
          qmemcpy(v21, a4, v22);
          a4 += v22;
          v21 += v22;
          v22 = 0;
        }
        v24 += dword_18062C;
        v21 = v24;
        --HIBYTE(a1);
      }
      while ( HIBYTE(a1) );
    }
    return;
  }
  if ( !word_E36D4 )
  {
    v97 = -a3;
    if ( (unsigned __int8)a1 <= v97 )
      return;
    v98 = (_BYTE *)(dword_18062C * a2 + v6);
    BYTE1(v97) = HIBYTE(a1);
    v99 = v98;
    v136 = v97;
    v100 = 0;
    while ( 1 )
    {
      while ( 1 )
      {
        while ( 1 )
        {
          v101 = *a4++;
          if ( v101 < 0 )
            break;
          if ( v101 )
          {
            LOBYTE(v100) = v101;
            do
            {
              v102 = *a4++;
              LOBYTE(v97) = v97 - 1;
              if ( (v97 & 0x80u) != 0 )
                *v98++ = v102;
              --v100;
            }
            while ( v100 );
          }
          else
          {
            v99 += dword_18062C;
            v98 = v99;
            LOWORD(v97) = __PAIR__(BYTE1(v97), v136) - 256;
            if ( !BYTE1(v97) )
              return;
          }
        }
        if ( (char)v97 > 0 )
          break;
LABEL_179:
        v98 -= v101;
        v103 = *a4++;
        LOBYTE(v100) = v103;
        do
        {
          v104 = *a4++;
          LOBYTE(v97) = v97 - 1;
          if ( (v97 & 0x80u) != 0 )
            *v98++ = v104;
          --v100;
        }
        while ( v100 );
      }
      LOBYTE(v97) = v101 + v97;
      if ( (v97 & 0x80u) != 0 )
      {
        v101 = v97;
        goto LABEL_179;
      }
    }
  }
  if ( !(word_E36D4 & 1) )
  {
    if ( !(word_E36D4 & 2) )
      return;
    v117 = -a3;
    if ( (unsigned __int8)a1 <= v117 )
      return;
    v118 = (_BYTE *)(dword_18062C * (HIBYTE(a1) + a2) + v6);
    BYTE1(v117) = HIBYTE(a1);
    v119 = v118;
    v139 = v117;
    v120 = 0;
    while ( 1 )
    {
      while ( 1 )
      {
        while ( 1 )
        {
          v121 = *a4++;
          if ( v121 < 0 )
            break;
          if ( v121 )
          {
            LOBYTE(v120) = v121;
            do
            {
              v122 = *a4++;
              LOBYTE(v117) = v117 - 1;
              if ( (v117 & 0x80u) != 0 )
                *v118++ = v122;
              --v120;
            }
            while ( v120 );
          }
          else
          {
            v119 -= dword_18062C;
            v118 = v119;
            LOWORD(v117) = __PAIR__(BYTE1(v117), v139) - 256;
            if ( !BYTE1(v117) )
              return;
          }
        }
        if ( (char)v117 > 0 )
          break;
LABEL_225:
        v118 -= v121;
        v123 = *a4++;
        LOBYTE(v120) = v123;
        do
        {
          v124 = *a4++;
          LOBYTE(v117) = v117 - 1;
          if ( (v117 & 0x80u) != 0 )
            *v118++ = v124;
          --v120;
        }
        while ( v120 );
      }
      LOBYTE(v117) = v121 + v117;
      if ( (v117 & 0x80u) != 0 )
      {
        v121 = v117;
        goto LABEL_225;
      }
    }
  }
  if ( word_E36D4 & 2 )
  {
    v105 = (_BYTE *)(dword_18062C * (HIBYTE(a1) + a2) + (unsigned __int8)a1 + a3 + v6);
    v106 = 0;
    v107 = (unsigned __int8)a1 + a3 + 1;
    if ( v107 >= 0 )
    {
      LOBYTE(a1) = v107;
      v137 = v107;
      v108 = v105;
      v109 = -1;
      do
      {
        while ( 1 )
        {
          while ( 1 )
          {
            LOBYTE(v109) = *a4++;
            if ( (v109 & 0x80u) == 0 )
              break;
            v105 += v109;
            LOBYTE(a1) = v109 + a1;
          }
          if ( !(_BYTE)v109 )
            break;
          LOBYTE(v106) = v109;
          do
          {
            v110 = *a4++;
            LOBYTE(a1) = a1 - 1;
            if ( (a1 & 0x80u) == 0 )
              *v105 = v110;
            --v105;
            --v106;
          }
          while ( v106 );
        }
        v108 -= dword_18062C;
        v105 = v108;
        a1 = __PAIR__(HIBYTE(a1), v137) - 256;
      }
      while ( HIBYTE(a1) );
    }
  }
  else
  {
    v111 = (_BYTE *)(dword_18062C * a2 + (unsigned __int8)a1 + a3 + v6);
    v112 = 0;
    v113 = (unsigned __int8)a1 + a3 + 1;
    if ( v113 >= 0 )
    {
      LOBYTE(a1) = v113;
      v138 = v113;
      v114 = v111;
      v115 = -1;
      do
      {
        while ( 1 )
        {
          while ( 1 )
          {
            LOBYTE(v115) = *a4++;
            if ( (v115 & 0x80u) == 0 )
              break;
            v111 += v115;
            LOBYTE(a1) = v115 + a1;
          }
          if ( !(_BYTE)v115 )
            break;
          LOBYTE(v112) = v115;
          do
          {
            v116 = *a4++;
            LOBYTE(a1) = a1 - 1;
            if ( (a1 & 0x80u) == 0 )
              *v111 = v116;
            --v111;
            --v112;
          }
          while ( v112 );
        }
        v114 += dword_18062C;
        v111 = v114;
        a1 = __PAIR__(HIBYTE(a1), v138) - 256;
      }
      while ( HIBYTE(a1) );
    }
  }
}

x_DWORD_17DEE4_mouse_position
34eee4
a
34eee6

 setCursor:
    push bp 
    mov bp, sp
    pusha
    push es
    mov ax, 09h
    mov bx, WORD PTR [bp+08h]
    mov cx, WORD PTR [bp+06h]
    mov dx, WORD PTR [bp+04h]
    push ds                         ;Setting ES = DS is not necessary in COM
    pop es                          ;files unless somebody changed ES
    int 33h 
    pop es
    popa
    pop bp  
    ret 06h  
    ;
    ; CURSORS
    ;
    barCursor       dw  16 DUP(0fe7fh)
                    dw  16 DUP(0180h)
    checkerCursor   dd  8 DUP(5555aaaah)
                    dd  8 DUP(0aaaa5555h)
_CODE ENDS 

po cursoru je funkce:

sub_8CB3A 

	cseg01:00099374 __GETDS:                                ; CODE XREF: sub_6FD30+A↑p
	cseg01:00099374                                         ; sub_752C0+A↑p ...
	cseg01:00099374                 mov     ds, cs:word_9911C
	cseg01:0009937C                 retn
	cseg01:0009937C start           endp ; sp-analysis failed
	cseg01:0009937C

sub_8C839
sub_9951B

sub_8C329

zde se to nastavuje:
sub_8CEDF_install_mouse()

https://www.equestionanswers.com/c/c-int33-mouse-service.php

instalace kursoru mozna zde:
sub_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);//24FBF0 - 2AA18C//?tab

nebo tady:
sub_52E90

tady se to meni
if ((x_WORD_E1F84[i+12]&0xff) && sub_7B200_in_region(&x_WORD_E1F84[i], x_DWORD_17DEE4_mouse_position, SHIWORD(x_DWORD_17DEE4_mouse_position)) )


char sub_7A060_get_mouse_position2()//25B060

x_DWORD_E3760 - fix xy -ok
x_DWORD_17DEE4_mouse_position - fix xy

x_DWORD_1806E8 - fix xy

x_DWORD_17DEE4_mouse_position = x_DWORD_E3760;
SHIWORD(x_DWORD_17DEE4_mouse_position) = SHIWORD(x_DWORD_E3760);

sjednotit
	x_DWORD_E3760
	  x_WORD_E3760 = temp_mouse_x; //nastav x
	  x_WORD_E3762 = temp_mouse_y; //nastav y

Bit16s x_WORD_E3760_mousex;
Bit16s x_WORD_E3762_mousey;


Bit8s x_DWORD_1806E8_mousex; // weak
Bit8s x_DWORD_1806E8_mousey; // weak

x_DWORD_180730 - fix xy
x_DWORD_1806F8 - fix xy

Bit16s x_DWORD_1806F8_mousex; // weak
Bit16s x_DWORD_1806F8_mousey; // weak

x_WORD_180660_VGA_type_resolution//351660 - ma by 8? asi ne, je to nejspis textovy mod

I find 2b4760 changes - x_WORD_E3760_mousex

test:
char /*__cdecl*/ sub_779E0_lang_setting_loop(int a1x)//2589E0

repair sub_7F960

repair sub_83E80

najit 256200 ! - vykresli kursor! ne musi to by predtim
najit 260960 ! - vykresli vlajku
vlajka je asi uz zde - 25f840

if ( !byte_E3766 )
    sub_8CACD(v1); - urcite prida kursor

	je nekde zde:
	void /*__cdecl*/ sub_75200_VGA_Blit640(Bit16u height)//256200

	zjistit zmeny x_WORD_18072C - 35172c

	testuj:
	void sub_8CD27(posistruct a2)//26dd27

	35172c

	az posledni volani - 

	0x35172c;

	pozor na toto:
	sub_8CD27_set_cursor(*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct/*filearray_2aa18c[0]*/);//26dd27
  x_WORD_17DEEC = 0;

  	//[34eed4]+de? =48c81b ->3abe47001315
	//25b110

	x_DWORD_17DED4_spritestr[0x25]

	x_DWORD_180700 - 351700 // tady mozna nevadi - nealokovane pole
	x_DWORD_1806F0 - 3516f0 // tady mozna take ne - nealokovane pole
	x_DWORD_180730 - 351730 - 191919 fefefefe		
	x_DWORD_18062C_resolution_x - 35162c musi byt 40

	vlajka se prekresli v 258cba - v prvnim kroku je puvodni vlajka, ale v druhem
	se vlajka nastavi

	4527bf->451516

	misto vlajky:
	0x152C0

	x_DWORD_18062C_resolution_x(351628) -3BF364
	27b144 - tady se vlajka smaze
	sub_7F960 / 260960 - prekresli texturu vlajky

	eax - 3550a6
	ebx - 451514

	x_D41A0_BYTEARRAY_4_struct.byteindex_A - 38cf5a
	23ccc0

	2a51a4

	fix cursor:
	/* sub_8CD27_set_cursor((Bit8u**)**filearray_2aa18c[0]); fix it*/

	int v8; // [esp+10h] [ebp-4h] - musi byt asi vstup

	find variable : result = 7 * x_WORD_D4004;//2a5004

	x_BYTE_E3798 && x_BYTE_E3799
	se lisi od
	2b4798 a 2b4798
	227dd0
	je to zde:
	sub_90FD0(v3, v4, a3); //fix it

	neni nastaven E9C4C - 2bac4c - 82fd8f
	sub_2EBB0_draw_text_with_border_630x340(x_DWORD_E9C4C_langindexbuffer[0]);//fix it
	257a40
	sub_76A40_lang_setting()

	x_D41A0_BYTEARRAY_4_struct.byteindex_4 - ma byt 2 38cf54
	adress - 257b53 -fixed

	eax - 10
	ebx - 10
	ecx - 50
	edx - 40
	3551e6
	3f52a8

	x_WORD_E12FE je 0
	2b22fe ma byt 08a400
	v 257160 - to je ok

	x_DWORD_17DB54
	34eb54 ma byt 8b nebo 98 nebo ba nebo 40
	call 256e70
	call 257540
		call 256d70

	x_DWORD_E12F4 = 000000
	versus
	2b22f4 = a4a03a00000000001000000

	x_DWORD_180628b_screen_buffer

	34eb50
	a
	x_DWORD_17DB50->a4523f

	v3 = (char*)"COLOUR256 ";
    sub_76260_read_intro_pallette(0x100);
	-tady je to ok
	v7 = (char*)"BRUN ";//ok
    sub_76540();
	-tady je to take ok
	sub_75CB0();//256cb0
    if ( v23 )
  	-tady je to stale ok

dalsi cyklus 8c - ok
sub_76300 - chyba

dosbox - discoverwhich 0 276 174 0
sub_7FAE0_draw_text(v87, v99, a4, v98, v86);
remc2 discoverwhich 0 276 15c 0

x_DWORD_17DEC0
versus
34eec0

x_DWORD_17DEC0[394];//x_DWORD_17DEC0_spritestr[65]

3ed8- 50 radek

text - problem palety


zkontrolovat tento kod:
  {
    sub_9A0FC_wait_to_screen_beam();//27b0fc
    if ( x_WORD_E12FC )
    {
      sub_41A90_VGA_pallette_install(unk_17D838);
      v19 = sub_5BE80_test_pallette(unk_17D838, 0x3Fu, 0x3Fu, 0x3Fu);
      sub_2EC90(v19);//20fc90

zkontrolovat paletu:
unk_17D838
versus
34e838

barva
805c30 versus
000000

radek cca 169
posouva se o 31 radku - 0x26C0

Kafkar... 0 276 154 0
Kafkar... 0 276 154 0

setbyte is color

void sub_8F935_bitmap_draw_final(Bit8u a1byte1,Bit8u a1byte2, Bit16u tiley, int tilex, Bit8u* texture, Bit8u setbyte, char a6)//270935

	memset(x_DWORD_180628b_screen_buffer, 127, 320*200);//debug
		//VGA_Blit(320, 200, x_DWORD_180628b_screen_buffer);
	sub_2EBB0_draw_text_with_border_630x340(x_DWORD_E9C4C_langindexbuffer[a1]);//fix it - edx
	sub_90478_VGA_Blit320();//debug
}

void sub_loc_1B5BC(Bit16u a1) {
	memset(x_DWORD_180628b_screen_buffer, 200, 320*200);//debug
	Set_basic_pallette();
	sub_2EBB0_draw_text_with_border_630x340(x_DWORD_E9C4C_langindexbuffer[a1]);//fix it - edx
	sub_90478_VGA_Blit320();//debug
}

void sub_76160_play_intro(__int16 a1, __int16 a2, Bit8u* a3)//257160

x_DWORD_180628b_screen_buffer

0x3aa0a4

256e70 - sub_75E70
	20fc90 - zde se doplni titulky

dosbox Kafkar	0 276 154 fe
sub_7FAE0_draw_text(v87, v99, a4, v98, v86);
remc2 Kafkar 0 276 154 0

ebp+28 3551cc

find this:
2b2bac - dosbox 508325000000

Bit8u* sub_7AB00(__int16 a1, int a2, signed __int16 *a3, unsigned __int8 a4)

int /*__fastcall*/ sub_76FA0(int a1, int a2, signed __int16 *a3x)//257fa0 - menu loop

sub_7A110_load_hscreen(x_WORD_180660_VGA_type_resolution, 4);

void sub_81360_draw_bitmap_line(Bit32s minx, Bit32s miny, Bit32s maxx, Bit32s maxy, __int16 a5)//262360

problem:
      v21 = sub_7AB00(v19, v20, (Bit16s*)a3x, 4u);
      if ( sub_7B250((int)v21, v22, a3x) )

sub_8CD27_set_cursor(*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct);//anything with vga, maybe mouse cursor//26dd27 //xadatapointersdat asi 1a6f44
sub_8CD27_set_cursor(*filearray_2aa18c[0].posistruct);
sub_8CD27_set_cursor(x_DWORD_17DED4_spritestr[234]);

	  ;// fix it! sub_8CD27_set_cursor((Bit8u**)x_DWORD_180720); - not fixed
x_BYTE_E25ED - 2b35ed

x_DWORD_E3768 2b4768 - tlacitka
x_DWORD_180710_mouse_buttons_states - 351710 - tlacitka

problem in:
sub_6FC30() - 250c30
x_DWORD_EA3D4 - 2bb3d4
..x_DWORD_E9B20[1];=2bab20 v sub_6Fc50


find setting of 
0x19f0ec,0x19f7ec
24fb90

2bab20 - find
	23cf50
		265250
			27b32d

problem in sub_2BD10:
x_DWORD_D41A0

x_DWORD_EA3D4 in
int sub_7FCB0_draw_text_with_border(int a1, Bit8u* a2, Bit32s a3, Bit32s a4, int a5, Bit8u a6, unsigned __int8 a7, Bit32u a8)//260cb0
	sub_2BC10
		sub_2BC10
			sub_6F940
sub_7B660

x_DWORD_EA3D4 - 2bb3d4
melo by to byt posistruct
zmeneno v 
0x257160
	1fc280
		20fbb0(2ebb0)

2BB578
in
sub_5C1B0_set_any_variables2 - 23d1b0
sub_5C330(); - 23d330
a pote:

Bit32s x = *x_D41A0_BYTEARRAY_0 + 0x6e8e + 168 * i;//fix

find 2a51a0

x_DWORD_EA3D4 - 2bb3d4
ecf71900 58731a00 00000000 0000000
c6ce3500 6ecf3500 16d03500 bed03500
35cec6 - 0000000

[2bb3d4]->19f7ec
2c32a100 00 00 2e32a100 04 07 36321a
1a322c->110100000000000007f0000000000000000

1a32dc
0101000ff01010000ff010100007f001
0x257160
	1fc280
		20fbb0(2ebb0)
			250c50
x_DWORD_EA3D4=x_DWORD_E9B20[0] nebo x_DWORD_E9B20[1]
Pathstruct x_DWORD_E9B20[2] = { xafonts0tab,xafonts1tab };?
sub_5B8D0_initialize_0x0023C8D0
	sub_5BF50_load_psxdata()-23CF50
		sub_84250_load_file_array-265250
			sub_9A32D_malloc_open_unpack-27b32d
*pstr[3].var28_begin_buffer,100

tempposistruct.pointer= &x_DWORD_EA3D4[v8];//fixed

19f990->59361a00 0407 70361a00 04 07 85361a00
-prvni 24fb90
19f990->59361a00 080e 70361a00 08 0e 85361a00
nekde se to musi upravit a tam z toho bude posistruct
sub_7AC00_load_and_set_graphics_and_pallette - 25bc00
	24fbf0

sub_6EBF0(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);
-find filearray_2aa18c changes !

x_DWORD_E9B20 je filearray_2aa18c[filearrayindex_ZERO1] - ale je nuytne prijit na to kde to vznika

2aa18c-> 
	2bc394->1a6f44 000000 0000
	2bc388->1a6f86 000000 0000
	2bc390->1a6578 1a6f44 0000
	2bab20->19f0ec 19f7ec - to je to co hledam

	

sub_5B8D0_initialize()//23c8d0
	nacteni:
	sub_5BF50_load_psxdata - 23cf50
	prvni zmena dalsi zmena je v:
	sub_6EB90 - 24fb90

toto je cil!
Pathstruct x_DWORD_E9B20 = filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct;

sub_7AC00_load_and_set_graphics_and_pallette - tady to nutne zkontrolovat
v 19f0ec
25cc28

sub_77980_exit_dialog()
2b2cb4
80892500 00000000 01002601 19003400
2c009801 40380b01 00006001 1a000000

sub_7BF20_draw_scroll_dialog

  sub_2BD10(a1 + *(unsigned __int8 *)(v8 + 4) - 12, v9, a1 + *(unsigned __int8 *)(v8 + 4) - 12, v9 + a3, v45); // to je ta linka, kterou je treba opravit

  find v33:
  if ( sub_7B200_in_region((x_WORD*)&v33, x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DEE4_mouse_positiony) )//icon OK
  v33
  355134
  00000000 00000000 00006f01 4e002a00
  1c000000 00000000 53026801 68010000

  //adress 25c832

  char* sub_70910() // fix it!

  fix int x_DWORD_EA2C4; // weak

  result = (int)x_DWORD_17DE44; - problem je zde:
  x_DWORD_E9C38_smalltit = x_DWORD_17DE44;

  //test removed x_DWORD_17DE44

  //test removed

find x_DWORD_E9C2C);//2bac2c
	initialize
		23cf50
			sub_83B50();//264B50
				sub_54630_load_psxblock(x_BYTE_D41B5);

sub_5BF50_load_psxdata()

x_DWORD_E9C2C == pstr[psxadatablock32dat].var28_begin_buffer

find &off_D41A8);//2a51a8

find x_DWORD_F6ED0);//2c7ed0 == TMAPS00TAB_BEGIN_BUFFER
find x_BYTE_F6EE0);//2c7ee0

2a9a54
2a9c38
2a9f7c

problém je zde:
      v1 = sub_7EAE0(
             (__int16 *)&unk_17DB76,
             (__int16 *)&unk_17DB78,
             (__int16 *)&unk_17DB7E,
             (__int16 *)&unk_17DB80,
             &x_BYTE_17DB8F,
             (x_WORD*)&unk_17DB90);

find 2bc39e


sub_7AB00(/*v3,*/ v4, a3, 4u);

34eed4 ->43d1be-> 14654000 0000 16654000 5d5f 56874000

43c63c
2bac38->4434af 4483e4 1a101a 00000

43d446

byte_EB39E; 2bc39e
unk_E28A8 2b38a8

x_BYTE_E25ED[0x2BB]

x_WORD_E2008 = 2b3008


//sound

x_WORD_E2A14_sound_active - 2b3a14

x_BYTE_E3798_sound_active2 - 2b4798 - neni aktivni

x_BYTE_E37AD_actual_sound - 2b47ad


x_D41A0_BYTEARRAY_4_struct.setting_216 = 43 = 38d006

char *sub_8F850()//270850
x_BYTE_E388D=0 ?
void sub_8D8F0_sound_proc3()//26e8f0
//

preruseni zvuku sound interupt irq
//fix it:__asm { int     31h; DPMI Services   ax=func xxxxh }

AIL_API_get_real_vect

ac_sound_call_driver x
ac_sound_stop_music - on end
ac_sound_install_dig_ini x
ac_sound_install_dig_driver_file x
ac_sound_get_io_environment x

int sub_8E470_sound_proc17_volume(int a1)
x_BYTE_E379A - find this var

void sub_8E160_sound_proc15_startsequence(__int16 a1, unsigned __int8 a2)//3 64
x_BYTE_E37FC 00 - 2b47fc = 01
x_BYTE_E37FD 00 - 2b47fd = 01
x_WORD_E3804 00 - 2b4804 = 06
x_WORD_E3802 00 - 2b4802 = 00

unk_180A30

      if ( !sub_931F0_AIL_install_DIG_INI(a3, (int **)&x_DWORD_180B48) )
      {
        sub_93480_AIL_uninstall_DIG_driver((unsigned int *)x_DWORD_180B48); -- sem se to musi dostat
        v10 = 1;
      }

sub_931F0_AIL_install_DIG_INI

//3550c8 - SB16.DIG  //355148 - 220

int sub_92190_AIL_read_INI(Bit8u* a1, char* a2)//AIL_read_INI // 273190


int *sub_A2EA0(int a1, const void *a2) //
	int sub_A2C80(int *a1, const void *a2) // problem s a2

sub_91A80_AIL_set_preference
	sub_9E6E0_set_preference
		x_DWORD_181DAC[number];?


unk_F4240 - find and set

351b48

sub_93330_AIL_install_DIG_driver_file

    x_WORD_180B30 = v5.IO;//fixed


355048

1a7758

fix sub_A2EA0
-kde se vzal?

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

v8 = sub_93010_AIL_install_driver(/*a1, */v6, v3); - sledovat
	AIL_DRIVER* sub_9E720_AIL_API_install_driver(/*int a1, */Bit8u* driver_image, Bit32s n_bytes)//27f720

	v7 = sub_A2EA0(v8, IO);
