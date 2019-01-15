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
-kde je defonovana?

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

	nesmi vratit null:
	sub_93330_AIL_install_DIG_driver_file(/*a1, */a2.driver_name, a2.IO);
	musi neco vratit:
	int *sub_A2EA0(AIL_DRIVER* a1, IO_PARMS IO)//283ea0

kde se bere:
  *(x_DWORD *)(a1 + 84);//21
  *(x_DWORD *)(a1 + 96); i++ )//24

  find 00352DB4
	void sub_8D290_sound_proc13(char* a1/*, int a2, int a3*/)//26e290
		sub_917D0(v3);

find x_DWORD_181DAC - 352dac - c8


sub_917D0 - fix this parametres

x_WORD_E2A14_sound_active - 2b3a14
sub_8D290_sound_proc13
	x_WORD_E37B6_sound_number
		void sub_8D800_sound_proc2()//26E800
			 v0 = sub_84300_play_sound(x_BYTE_E37AC); 265300 - mel by vratit 0
			 //5 0 1
				sub_844F0_read_and_decompress_sound(v3, v8) - asi neprobehne uspesne
					fix this:
					x_DWORD_E37A8_sound_buffer1 = (Bit8u*)sub_83CD0_malloc2(*(x_DWORD *)&_C[v2] + 256);
problem:
x_DWORD_180B48 = sub_93330_AIL_install_DIG_driver_file(/*(int)a1, */unk_180AB0, unk_181D90);
	HDIGDRIVER sub_A3600_AIL_API_install_DIG_driver_file(/*int a1,*/ char* filename, IO_PARMS IO)//284600

x_DWORD_180B48 = sub_93330_AIL_install_DIG_driver_file(/*(int)a1, */unk_180AB0, unk_181D90);
	unk_180AB0 -filename
	v6

void sub_8F710_sound_proc21(int a1, __int16 a2, signed int a3, unsigned __int8 a4, char a5)
int **sub_8F100_sound_proc19(int *a1, __int16 a2, int a3, int a4, unsigned __int16 a5, char a6, unsigned __int8 a7)//270100 -asi pley sound
signed int sub_916F0_sound_proc24()

unk_180750 - 351750
2097a2 00000
b49fa2 000

*(x_DWORD *)v8 = (x_DWORD)sub_93510_AIL_allocate_sample_handle(x_DWORD_180B48);

fix this:
void sub_A38E0_init_sample(HSAMPLE S/*S32     format,U32     flags*/) - ok 2848e0
void sub_A3B40_set_sample_volume(HSAMPLE S, float volume) -ok 284b40
void sub_A3BB0_set_sample_volume_pan(HSAMPLE S, float volume) - ok 284bb0
void sub_A3CB0_start_sample(HSAMPLE S) - ok 284cb0
void sub_A3DA0_end_sample(HSAMPLE S) - ok - fix call fix 284da0
int sub_A3EB0_register_EOS_callback(HSAMPLE S, int EOS)-ok- 284eb0
void sub_A3F00_set_sample_user_data(HSAMPLE S, int a2, int a3)-ok- 284f00
int sub_A3F30_sample_user_data(HSAMPLE S, int index)-ok- 284f30
x_DWORD *sub_A4970(int a1, int a2, int a3)?ok 285970
void sub_A3AF0_set_sample_playback_rate(HSAMPLE S, int a2) - ok 284af0
x_DWORD *sub_A47C0(int a1, x_DWORD *a2)?ok 2857c0
Bit32s sub_A4B20_set_sample_file(HSAMPLE S, Bit8u* file_image, Bit32s block)?ok 285b20
void sub_A9950(int a1, int a2);??? 28a950
int sub_A3A70(x_DWORD *a1, int a2, int a3) 284a70
sub_A2110((HSAMPLE)(x_DWORD *)(2196 * i + a1[23])); 283110
sub_A2070((unkstr1*)S->sam_var[0]);//? 283070
void sub_A43E0(HSAMPLE S) 2853e0

int sub_A4920()

sub_938C0_AIL_set_sample_file(*v14, (Bit8u*)*(x_DWORD *)(x_DWORD_E37A0_sound_buffer2 + 32 * a2 + 18), 1);
-cycle

void sub_8F710_sound_proc21(int a1, __int16 a2, signed int a3, unsigned __int8 a4, char a5)
-unk_180750_sound_buffer3 - nektere nejsou inicializovany

unkstr1* sub_A2070(unkstr1* a1)//283070
  if ( !a1->var24_aildrv ) - mozna ma byt false

    S->sam_var[sam_var2_sample_address_start] = (Bit32s)start;
	S->sam_var[sam_var4_sample_address_len] = len;

set file sample
S - a29720->1a7884 02 876940 00 005ec01 00 00 00
1a7884->1a7758 02c78c 02c87c 00 800 5622 02 00

0
355168
	01 025622 1a6f44 355188
401
1a7758

401
a20002
2b6f44
35001a

call driver

find 2b27cc

analyze:
char sub_77980_exit_dialog(Bit8u* a1)//258980

a1->var21_aildrv - ma byt 0

sam_var[0] - test set
1a78d8

PREPREPREPREPRECALL0160:00272F70/00091F70 - 002B59C8 - sub_91F70_AIL_call_driver
PREPREPREPRECALL0160:0028258B/000A158B - 002B5998 - sub_A158B_AIL_API_call_driver
PREPREPRECALL0160:00272BD0/00091BD0 - 002B5940
PREPRECALL0160:00281EEC/000A0EEC - 002B5930
PRECALL0160:00272BF0/00091BF0 - 002B593C -sub_91BF0();
CALL0160:00281EF9/000A0EF9 - 002B592C
ADR0160:002B59DC/000D49DC
NEW VALUE0168:001A78D8 - 0001,0000
AFTER 04X:00000160/002B59DC

call near
int *sub_A20D0(int *a1)

1a78d8

fix 283110
co je S->sam_varw18 - a29ffc
-128 hodnot
con

char x_BYTE_17DF3C[1]; // fix it -  weak
char x_BYTE_17DF3D[1]; // fix it -  weak
char x_BYTE_17DF3E[302]; // idb

fix sub_A6530 - 287530 timer predelat na AILTIMERCB?
fix HMDIDRIVER sub_A6FB0_sound_proc26(AIL_DRIVER a1, const void *a2)-ok?
int sub_A7970_AIL_API_uninstall_MDI_driver(HMDIDRIVER mdi)-ok
HSEQUENCE sub_A7B30_AIL_API_allocate_sequence_handle(HMDIDRIVER mdi)-ok
void /*__fastcall*/ sub_8D970_sound_proc1(char* a1/*int a1, int a2, char* a3*/)//26e970
int sub_A6F30(HDIGDRIVER a) { stub_fix_it();return 0; }; // weak
for ( i = 0; i < 16; i++ )
            {
              v12[i + 8] = 0;
              v12[i + 24] = 0;
              v12[i + 40] = 0;
              v12[i + 56] = 0;
              v12[i + 72] = 0;
              v12[i + 88] = 0;
            }

v12->var0_aildrv->AIL_DRIVER_var5

zkontroluj:
x_DWORD_181EC8 = 16000000 / x_DWORD_181DAC[10];
v12->var0_aildrv->AIL_DRIVER_var5 - find 1a7a24
354f44 00000000 3c781a00 604f3500
2f630
274010

2f0b0

sub_A8050_AIL_API_stop_sequence

void sub_B0C1A(HDIGDRIVER a1)

fix // x_DWORD_E4E1C
	return ((int(*)(int, x_BYTE *, int *))off_AEB40[x_DWORD_E4E1C])(v4, (int8*)v5, v3);
//dma sub_B0B87(v26, 0);//fix

x_BYTE_E37FC - musi byt >1 // 2b47fc - 1
x_BYTE_E37FD - musi byt >1 // 2b47fd - 1
3 <= x_WORD_E3804 0 //2b4804 - 06
x_WORD_E3802//2b4802 00
if ( x_BYTE_E37FC && x_BYTE_E37FD && a1 <= x_WORD_E3804 && x_WORD_E3802 != a1 )


x_DWORD_E3810 je null // 2b4810

void sub_A5850(HSEQUENCE S, char a2, unsigned int a3, signed int a4, int a5)

v7->event_trap

fix: sub_8EAD0_load_music(tempebx);

x_BYTE_E37FC - 2b47fc
x_BYTE_E37FD - 2b47fd

start sequence:
find it
x_WORD_E3802 - 2b4802

void sub_8E160_sound_proc15_startsequence(__int16 a1, unsigned __int8 a2)//26f160

1fc2a3 - 1e1000
call 270710 - 1e1000

-melo byt to byt zde
void sub_loc_1B54A(Bit16u a1, Bit8u* a2) {
problem je zde -
if ( x_WORD_17DB60 != *(Bit16u*)v2 )

a2 = 0;//debug


v8 = sub_824E0(x_DWORD_17DB70str.x_WORD_17DB8A);
fix v8

find 
x_BYTE_F6EE0 - 2c7ee0 \

find
x_BYTE_13B4E0 - 30c4e0 nebo spis 30c500 v sub_44E40
vychazi z
x_BYTE_11B4E0 - 2ec4e0 nebo spis 2ec500 v sub_44DB0 - asi ok
vychazi z
x_WORD_15B4E0 - 32c4e0 f5fd58 v sub_B5EFA -zde je to vyreseno


find 355220

sub_43830 - v2


dalsi zasek v sub_44EE0

dalsi zasek v sub_440D0


find x_WORD_E2572 - 2B3572 - ok
find x_WORD_E2970 - 2B3970 - ok
find x_WORD_E24BE - 2B34be
a dalsi

zkontrolovat tento radek
if (v99 + v25 * (k - v96) <= a4 - 3 * v25)//adress 2610c2

špatně je a6 i v8
[ebp+24] - 5
if (!a6)//adress 261023
		v98 = v8 + 12;

edx=[ebp-14] - 9
[ebp-40]=edx - 9
edx=[ebp-24] - 0
ecx=[ebp-40] - 9
sub ecx,edx - 9
cbw eax=8
edx=ecx - 9
imul edx,aex - 48
lea ecx,[0+eax*4] -18
mov eax,ecx - 18
movsx ecx,[ebp-1c] - 1f2
movsx esi,[ebp-18] - 92
mov edi,ecx - 1f2
add edx,esi - da
sub edi,eax - 1da



a pak tento -color ma byt nula
void sub_7FB90_draw_text(char* textbuffer, Bit16s posx, Bit16s posy, Bit8u color)

xy_DWORD_17DEC0_spritestr[0]
se neshoduje s [34eec0]->4531c7 + 394 = 453351

xy_DWORD_17DEC0_spritestr[65].sizex (394)
se neshoduje s [34eec0]->4531c7+18a
34eec0
-trva

problem:
sub_7FCB0_draw_text_with_border(v3, x_DWORD_E9C4C_langindexbuffer[23 + v3], (signed __int16)(a1 + 4 * v9), v8, a2, 5, v7, 1);		

3800000

signed int sub_7E320_draw_bitmaps_and_play_sounds(__int16 a1, int a2)//25f320
helps

int sub_82510(__int16 a1, int *a2)//263510
sub_7E840_draw_textbox_with_line((Bit16u*)&v10, 238, 264);//draw help

find 34eba8 respektive 34ebb6
&unk_17DBA8str.unk_17DBA8

xy_DWORD_17DEC0_spritestr[65].sizex - 08 34eec0->4551d6 455360 0707
ale 34eec0+18a je 07

v 279a59

x_DWORD_17DEC0+0x18a

25e6cb

void sub_7D400_draw_texts_and_play_sounds(int a1, __int16 a2, __int16 a3, char a4)//25e40
if (a4 == 3 && x_D41A0_BYTEARRAY_4_struct.byteindex_A == 1)

260ae0
2611b1 - 801b1
sub_7FAE0_draw_text(v87, v99, a4, v98, 0/*v86*/);//adress 261197 (80197)
int sub_7FCB0_draw_text_with_border(int a1, Bit8u* a2, Bit32s a3, Bit32s a4, int a5, Bit8u a6, unsigned __int8 a7, Bit32u a8)//260cb0
25f8b1 - 7e8b1
int sub_7E840_draw_textbox_with_line(Bit16u* a1, __int16 a2, __int16 a3)//25f840
25e8c2 - 7d8c2

sub_7E840(&v22, 238, 264);
void sub_7D400_draw_texts_and_play_sounds(int a1, __int16 a2, __int16 a3, char a4)//25e400
25e8c2

black line -sub_7C140(v99, a1, v104, v29, v27)

x_WORD_180660_VGA_type_resolution - 8
versus
351660 -1
355244

find
x_BYTE_13B4E0 - 30c4e0 nebo spis 30c500 v sub_44E40
vychazi z
x_BYTE_11B4E0 - 2ec4e0 nebo spis 2ec500 v sub_44DB0 - asi ok
vychazi z
x_WORD_15B4E0 - 32c4e0 f5fd58 v sub_B5EFA -zde je to vyreseno

x_BYTE_12B4E0 - 2fc4e0

	sub_44E40(*(x_DWORD *)(v2 + 39), *(x_WORD *)(v2 + 43));//225e40
	- problem x_BYTE_13B4E0
	treto=i radek ma byt:
	00 00 05 00 00 05, ale je
	00 00 05 05 05 05

	-sem se to ani nedostane
void sub_44EE0(int a1, Bit16u a2)//225ee0
x_BYTE_10B4E0 - 2dc4e0

find
unk_D47E0 - 2a57e0

find 
dword_EA3BC - 2bb3bc
27b3d8 z call 27b32d -sub_9A32D_malloc_open_unpack
sub_84250_load_file_array
sub_5BF50_load_psxdata -23cf50
->1a7088 - 000000 ... 1515f203000015156407
*pstr[0xc].var28_begin_buffer

find
x_WORD_17B4E0 - 34c4e0
sub_44E40 - 225e40
f33b - ok

find 
unk_F42B0

1a70f2 - find
call 279709 - sub_98709_create_index_dattab_power
call 24fb90 - sub_6EB90
	init:sub_6EB90(&filearray_2aa18c[filearrayindex_BUILD00DATTAB]);//24fb90
	init:sub_101C0();//23CA36 - 1f11c0 //sem se to vubec nedostane

2aa1ec
2bb3b0

2bb3bc
1a7088

2bb3bc + 6 *0x11

find
x_DWORD_EA3E4 -  2bb3e4
x_DWORD_EA3E8 -  2bb3e8
sub_5C330();//23D330

for (i = (int)&x_DWORD_EA3E0[2]; i < (int)&x_DWORD_EA3E0[0x3e9]; i += 168)
	{
		if (*(x_BYTE *)(i + 63) && *(x_BYTE *)(i + 13) & 4)
			sub_57F20(i);
	}

x_DWORD_EA3E0 + 0x6e8e - 35cec6

sub_567C0(); - zde se inicializuje znovu

Bit8u sub_6FC30_get34_height()//250c30 - neni inicializovano
{
	return x_DWORD_EA3D4[34].height;
}
sub_6FC50(1);

text:
sub_7C120_draw_bitmap_640(tempposx, tempposy, xy_DWORD_17DEC0_spritestr[temptextbuffer[0]]);

x_BYTE_3659F
x_BYTE_3659F[v8 + *(unsigned __int8 *)(x_D41A0_BYTEARRAY_0 + 224769)] = 1;

find
35cf6a ->b0 52 2c
x_D41A0_BYTEARRAY_0 -> 356038

x_DWORD_EA3E0

find
2bb3e4->c6 ce 35
x_DWORD_EA3E0[1]

35cec6+150
35cf6a ->2c52b0
*(Bit32s*)(v2 + 164)

*(Bit32u*)(x_D41A0_BYTEARRAY_0 + 28466) = (Bit32u)&unk_F42B0;

void sub_60F00()//241f00

void sub_5C330()//23D330
2bb3e4->35cec6
x_DWORD_EA3E0[1]= &x_D41A0_BYTEARRAY_0[0x6e8e];//28302
x_D41A0_BYTEARRAY_0=356038, rozdil 0x6e8e

void sub_49F30()//22af30
35cec6+a4(35cf6a)->2c52b0


2c52b0+150->0


find
x_D41A0_BYTEARRAY_4_struct.player_name_57
void sub_51BB0_game_events(int a1)//232bb0

void sub_5C950(Bit8u* a1, Bit8u* a2)//23d950

unsigned int sub_7277A(unsigned int a1)//25377a

result = sub_78730_save_game_dialog(a1);
char sub_7E800(x_WORD *a1)//25f800
signed int sub_7E320_draw_bitmaps_and_play_sounds(__int16 a1, int a2)//25f320

355184 -> 60592c+2->1b
v58=200

void sub_49F90()//22af90

&x_DWORD_EA3E0[2]
versus
0x35cf6e -00000000 1e000000
-find 35cf72
231266 -near - x_WORD *sub_50250(int a1)//231250
int pre_sub_4A190(Bit16u functionnumber, Bit8u* adress)//22a97e
void sub_49540(Bit8u* a1, Bit8u* a2)//22a540


		case 0:return (int)sub_51530((int)adress);
		case 1:return sub_51660((int)adress);
		case 2:return sub_516C0((int)adress);
		case 3:return sub_51570((int)adress);
		case 4:return sub_515C0((int)adress);
		case 5:return (int)sub_51610((int)adress);

test void sub_498A0()//22a8a0

find 2153f0 - maybe 443f0

2a75c0 - 2A5c44

0x211d50 - maybe 40d50

5 2A5C57

0x2a75c6-2a5c52=1974

problem
void sub_49540(Bit8u* a1, Bit8u* a2)//22a540

problem:
x_WORD_EB398ar[0] = *(Bit16s*)(a2 + 4) << 8;//adress 22a5af
1a70ee - 75a5390060607eb7
27971f - call 279709
call 24fb90
tj. sub_6EB90(&filearray_2aa18c[filearrayindex_BUILD00DATTAB]);//24fb90
in previous
call 281863
call 2798a7
call 234e60
call 23cf50 - 23c9f7
&filearray_2aa18c[filearrayindex_BUILD00DATTAB]

problem
int sub_B5C60(__int16 a1, __int16 a2)

0x1e - find 1a713c e2d43900
call 281863
call 2798a7
call 234e60
call 27b32d
call 265250
call 23cf50

Bit8u* x_DWORD_EA3E0[0x3E9];//2bb3e0
27b453
call 27b32d
call 265250
call 228160

			sub_47FC0(v7);//vga smaltitle
			sub_56A30_init_game_level(a3);
			sub_47160();

find 363286 ->c6ce3500
238989 -int sub_57730()?
for (j = &x_DWORD_EA3E0[2]; j < &x_DWORD_EA3E0[0x3e9]; j += 168)
x_DWORD_EA3E0[2] - 000000001e000000d007
35cf6e - 000000001e000000a861
find 35cf74 0000a861
void sub_37240(int a1)//218240

-pak se vrat sem

next problem:
35d02a(35d016)=2789 versus 9494
eb2a - 22b17b in 22b050-sub_4A050
2789 - 22b17b in 22b050-sub_4A050
*(Bit16u*)(v1 + 20) = v6;//this is it line

356040->e92a711b - *(Bit32u*)(x_D41A0_BYTEARRAY_0 + 8)
22a8c2 - void sub_498A0()//22a8a0

35d016 + 2a=35d040->f401 nikoli 0300
22b146
22abb6 - void sub_49A30(Bit8u* a1, unsigned __int16 a2)//22aa30
*(x_WORD *)(a1 + 42) = x_WORD_D93C0_bldgprmbuffer[2 * a2];

int sub_38330(Bit8u* a1)//219330

find 35cf6e->16d03500
238989
2388db
void sub_57730()//238730 - test

v20 = &x_DWORD_D4C52ar[14 * (*m)[69] + *(Bit32u*)(x_DWORD_D4C52ar + 18 * v19)-0x2a5c52];
versus		

find
stav x_DWORD_EA3E0 - 
void sub_57730()//238730

x_DWORD_EA3E0 - 2bb3e0
27b453
call 27b32d - signed int sub_9A32D_malloc_open_unpack(Pathstruct path)//27B32d
call 265250 - bool sub_84250_load_file_array(int psindex)//265250
void sub_47160()//228160

nekde pod
sub_47560

{ &MSPRD00TAB_BEGIN_BUFFER,&MSPRD00TAB_END_BUFFER,&MSPRD00DAT_BEGIN_BUFFER,posistruct5 },
je


signed int sub_5C8D0(int a1)//23d8d0
x_DWORD_EA3E4[0x98] not 363286 ->c6ce35
238989
void sub_57730()//238730
	for (j = &x_DWORD_EA3E4[1]; j < &x_DWORD_EA3E4[0x3e8]; j += 168)

3632d2->804d80de
238e40
void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70

void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
a2
neni
3550d8 -data tj v2 = word_15B4E0[(*(_BYTE *)(a2 + 3) << 8) + *(_BYTE *)(a2 + 1)];
find
238e40
void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70

23d99f - adress

35839a - data

22b839 - adress
int sub_4A820(int a1)//22b820

355210 - data

355214 - data2->0x002212a0

22b375
void sub_4A310(unsigned __int16 *a1)//22b310

22b36c - adress maybe 8a36c
22b310
posxy = v11 + (v12 << 16);
v13 = sub_10C40((Bit16u*)&posxy);

22b379 adress

int sub_B5C60(Bit16u a1, Bit16u a2)//296c60


x_BYTE_11B4E0[0xdf4d]=0x98
versus
[0xdf4d+2ec4e0]=0x95 //2FA42D
225e34
void sub_44DB0()//225db0

unsigned int sub_43970(unsigned int a1)//224970

2183f3
void sub_37240(Bit8u* a1)//218240

229c84
__int16 sub_48B90(int a1)//229b90

sub_44D00();//225d00

sub_70910_print_string((char*)"Initialise Models\0");
	memset(x_WORD_EB398ar, 0, 6);
	sub_49F90();
	v2 = x_D41A0_BYTEARRAY_0;
	x_D41A0_BYTEARRAY_0[4582] = -1;
	sub_71A70(v2[196308]);
	if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & 4))
	{
		sub_58940(&x_D41A0_BYTEARRAY_0[196302]);
		sub_11EE0(&x_D41A0_BYTEARRAY_0[196302]);
		sub_84790();
	}
	sub_4A1E0(0, 1);

	x_BYTE_11B4E0 - 2ec4e0 - difference

	void sub_49290(Bit8u* a1, char a2)//22a290
	-je to treba projit a zkontrolovat krok po kroku

	void sub_462A0(unsigned __int16 a1, __int16 a2)//2272a0
	-bad count 0x6d5c

	void  (Bit8u* a1)//218240

	0xbdaf - remc2
	0xb14b - dosbox

	&x_BYTE_11B4E0[0xb452] - 57555144
	versus
	002ec4e0 + 0xb452- 57555244

	2183f3
	tj.
		case 0x218240: {
		sub_37240(a1);

	100 - 58585548 - ok
	400 - 69696661 - ok
	570 - 71717171 - ok
	573 - tam je prvni rozdil
	575
	579
	57a - 696c6c6c - re versus 7171717 - dos

 0 0
 e7 07
 4c 37
 48 5e

 ca 40
 80 80

 355184

 55504949
 	&x_BYTE_11B4E0[0xc35a] - 6d6d6a62
	versus
	002ec4e0 + 0xc35a- 7676726e

	&x_BYTE_11B4E0[0xbe54] - 55504949
	versus
	002ec4e0 + 0xbe54- 675b4e4e

	0x10 - ok
	0x12 - ok
	0x15 - error?
	0x25 - ok
	0x50 - ok
	0x75 - ok
	0x87 - ok
	0x93 - ok
	0x96 - ok
	0x98 - ok
	0x99 - ok
	0xc0 - ok
	0xd0 - ok
	0xd8 - ok
	0xd9 - error comp3 and comp4 e5e4
	0xda - error comp3 and comp4 e3dd
	0xdc - error comp3 and comp4 e3dd
	0xe0 - error comp3 and comp4 bba9
	0x100 - error comp3 and comp4 b93b
	0x250 - error comp3 and comp4 b14b
	0x580 - error?

	&x_BYTE_13B4E0[0xe5e4] -0x30c4e0 01414161
	&x_BYTE_10B4E0[0xe5e4] -0x2dc4e0 01010124
	pote
	&x_BYTE_13B4E0[0xe3e4] -0x30c4e0 d1b1b1b1/81818181
	&x_BYTE_10B4E0[0xe3e4] -0x2dc4e0 1a1a1a1a/ff010101

				x_BYTE_10B4E0[a3] = *v7;
			x_BYTE_13B4E0[a3] = x_BYTE_13B4E0[a3] & 0x8F | v7[1];
			sub_45DC0(/*v16, v12,*/ v48, v15[0]);
			sub_37240(a1);

find 2a5a30 unk_D4A30

	0x10 - ok
	0x12 - ok
	0x15 - ok
	0x25 - ok
	0x50 - ok
	0x75 - ok
	0x87 - ok
	0x93 - ok
	0x96 - ok
	0x98 - ok
	0x99 - ok
	0xc0 - ok
	0xd0 - ok
	0xd8 - ok
	0xd9 - ok
	0x120 - ok
	0x200 - ok
	0x400 - ok
	0x500 - ok
	0x540 - ok
	0x560 - ok
	0x562 - ok
	0x563 - ok
	0x564 - ok
	0x565 - ok
	0x570 - error comp2 c143 comp5 c544
	0x582 - error comp2 a546 comp5 b24a

	&x_BYTE_13B4E0[0xe3e2] -0x30c4e0 41615131
	&x_BYTE_10B4E0[0xe3e2] -0x2dc4e0 01010101
	pote
	&x_BYTE_13B4E0[0xe3e2] -0x30c4e0 21514101
	&x_BYTE_10B4E0[0xe3e2] -0x2dc4e0 01010101

	e3e2

	e6e5
	void sub_45DC0(Bit8u a1, Bit8u a2, unsigned __int16 a3, unsigned __int8 a4)//226dc0
void sub_462A0(unsigned __int16 a1, __int16 a2)//2272a0
d24f
c648

comp2
	&x_BYTE_11B4E0[0xc648] -0x2ec4e0 01010101		

comp2
	&x_BYTE_11B4E0[0xc445] -0x82829d71 0x2ec4e0	tj.7f888582	
	
filearray_2aa18c[filearrayindex_BUILD00DATTAB]
versus
1a7124->24cf39

(debugcounter2 >= 0x2340)
az
(debugcounter2 >= 0x2520)

2345 -ok
2347 -ok
2348 -ok
2349 -ok
2350 -ok
2355 -ok
2362 -ok
2366 -ok
2368 -ok
2369 -ok
2370 -ok
2450 -ok
0xc749
comp2 = compare_with_snapshot((char*)"0160-00229B94", x_BYTE_11B4E0

signed int sub_5C8D0(Bit8u* a1)//23d8d0
363286->35cec6
versus
x_DWORD_EA3E4[*(Bit16u*)(2124 * x_D41A0_BYTEARRAY_4_struct.byteindex_12 + x_D41A0_BYTEARRAY_0 + 11240)];
238989 in void sub_57730()//238730

find
3632d6->a0136400
tj.
(Bit16u*)(v1 + 76)//+4
238e42 - void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70

fix
x_WORD *sub_4A920(int a1)//22b920
x_WORD *sub_4A9C0(int a1)//22b9c0
Bit8u* sub_4AA40(int a1)//22ba40
atd.

add adress 213400

char sub_32420(Bit8u* a1)//213420
compare
a1 with 3618ee - elespon 100

find 3618f2
adress 22b137 -2c01 - Bit8u* sub_4A050()//22b050 - asi ok
adress 22fc77 -7b03 - Bit8u* sub_4EC10(int a1)//22fc10

(int)x_DWORD_EA3E4

sub_498A0();//22a383 - problem - test it

find
35d026->02000000 versus 04000000
3d000000 - 2297ac
04000000 - 23127b
02000000 - 22aa7a -void sub_49A30(Bit8u* a1, unsigned __int16 a2)//22aa30
void sub_49540(Bit8u* a1, Bit8u* a2)//22a540

find
36193e->5012
22aa20 int sub_49A20(Bit8u* a1)//22aa20
xxxx
22b190
22b392 - call sub_4A310(unsigned __int16 *a1)//22b310
void sub_4A1E0(int a1, char a2)//22b1e0

38635d
386c09


24a3f0 // 693f0

x_WORD_15B4E0, 0x32c4e0
2856
x_WORD_15B4E0+0x2856 = 32ED36
238e3c - void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
Bit8u* sub_4B490(int a1)//22c490
Bit8u* sub_4A190(Bit8u* a1, int a2, int a3)//22b190
void sub_4A310(unsigned __int16 *a1)//22b310
void sub_4A1E0(int a1, char a2)//22b1e0
void sub_56A30_init_game_level(unsigned int a1)//237a30

36193e

find
36193e->5012
22aa20 int sub_49A20(Bit8u* a1)//22aa20
xxxx
22b190
22b392 - call sub_4A310(unsigned __int16 *a1)//22b310
void sub_4A1E0(int a1, char a2)//22b1e0

38635d
386c09

jina adresa 22fc10 versus 22b1ca

find
35517c

Bit8u* sub_4A050()//22b050

find 35606d->34c 39e
22af6d - void sub_49F30()//22af30 - ok
22b002 - void sub_49F90()//22af90 - ok
22b100 - Bit8u* sub_4A050()//22b050

0 - ok
10 - ok
50 - ok
75 - ok
87 - ok
94 - ok
0x99 - ok
0x9a - 0x34e(dbox) - 0x34e
0x9b - 0x34d(dbox) - 0x3ba
0x9c - 0x34c(dbox) - 0x34c
0xa0 - 0x348(dbox) - 0x3b5
0xb0 - 0x338(dbox) - 0x338
0xc0 - je to preskoceno 0x328
100 - za

unsigned int sub_6F150(Bit8u* a1)//250150 - kontrola

find 363286
238989 void sub_57730()//238730
[2a51a4]
*(Bit32u*)&x_DWORD_EA3E4[0x98]
363286

void sub_5C8D0(Bit8u* a1)//23d8d0

find 3632d6
238e46 - void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
23d9d4 - void sub_5C950(Bit8u* a1, Bit8u* a2)//23d950
91

find 2537b0 versus x_WORD_727B0

void sub_32420(Bit8u* a1)//213420

unsigned int sub_6F150(Bit8u* a1)//250150
			|| !x_BYTE_3659C

find 3618ee->0000007b0300007a03
3618f0
22fc77 - Bit8u* sub_4EC10(int a1)//22fc10
22b1ca - 22b190
*(Bit32u*)(x_D41A0_BYTEARRAY_0 + 4 * 0x376+ 582)

211f60
2130e0

sub_47320(a2);

sub_47560(index, v4, v1, a1, x_DWORD_17DB54_game_turn2);
sub_57730();

find
x_DWORD_E7EE0 - 2b8ee0

void sub_101C0()//1f11c0
x_DWORD_E9980x[k].pointer = &dword_E9C30[0][l];

dword_E9C30
x_DWORD_E9980x

signed int sub_10130(int a1, x_DWORD *a2, x_DWORD *a3)//1f1130

!!!!!!! - re test
Bit8u* sub_4A050()//22b050
!!!!!!! - re test

246080
23f010

remove evets fo debug !!!!

x_BYTE_3659C

void sub_2D710(Bit8u* a1)//20e710

find 38d046
x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[4]

241f60

24203d - sub_61000(Bit8u* a1)//242000

find a67dea - edx+412
-&filearray_2aa18c[filearrayindex_MSPRD00DATTAB]

2818ac

279739

279722 - void sub_98709_create_index_dattab_power(Bit8u* tabbuffer, Bit8u* tabbufferend, Bit8u* datbuffer, posistruct* dattabindex)//279709
24fbc2 - 24fb90
228160

1]
void sub_30630()//211630
v6? - 3551bc->4001000002000002

2]-fix  **file - ok
char sub_6D4C0(Bit8u* a1) - ok

a44dc6 - ."Nasleduj -v2
ebp+76 35519c->a44dad "Let smerem -v14

v12 ma byt 19
chce to zjistit puvod
*(signed __int16 *)(a1 + 4) a *(signed __int16 *)(a1 + 16)
3551bc

void sub_30630()//211630 

find 38d046->794c
void sub_2D710(Bit8u* a1)//20e710
v18 = x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[4]; = [2a51a4]+f6
241f68 call 242000

position of graphics buffer 655360 nebo 3842212

void sub_2E260_draw_spell(__int16 a1, __int16 a2, Bit8u* a3, char a4)//20f260
[2bb3e4]->c6ce3500 6ecf3500 -ok
ebx=36332e - a3 ne
cmp ebx,[2bb3e4]

void sub_5D530(Bit8u* a1)//*(x_DWORD *)(a1 + 160)//23e530

a1+160
363326->9a942a

22b15f - sub_4A050()//22b050
Bit8u* sub_4A920(Bit8u* a1)//22b920

*(x_DWORD *)(a1 + 160)
2a949a - 0700010000

unk_D89A6; 0x2a99a6
unk_D849A; 0x2a949a

void sub_47760(/*int a1,*/Bit32u user/* int a2, int a3*/)//228760

sub_2E260_draw_spell(//draw spell1

eax+0x333
tj.
35932f->0x99
*(*(Bit32s*)(v3 + 164)+0x333)
235cd9 int sub_54A50(unsigned __int16 a1, int a2)//235a50
	*(_WORD *)(a2 + 2103) = result;

*(x_WORD *)(x_D41A0_BYTEARRAY_0 + 11230 + 1817) = 1;


test
int sub_2BE30()//20ce30
		v3 = x_DWORD_EA3E4[*(unsigned __int16 *)(2124 * x_D41A0_BYTEARRAY_4_struct.byteindex_12 + x_D41A0_BYTEARRAY_0 + 11240)];
		20ced9

		problem je zde:
		for (i = 0; i < 26; i++)
	{
		v3 = 2 * i;
		if (*(x_WORD *)(2 * i + *(x_DWORD *)(a1 + 164) + 819))
		{
			v4 = (Bit8u*)sub_4A190((Bit8u*)(a1 + 76), 15, i);
			if (v4)
			{
				v5 = &x_D41A0_BYTEARRAY_0[28302];
				*(x_WORD *)(*(x_DWORD *)(a1 + 164) + v3 + 819) = (v4 - &x_D41A0_BYTEARRAY_0[28302]) / 168;
				*(x_WORD *)(v4 + 40) = (a1 - v5) / 168;
				v4[12] |= 1u;
			}
			else
			{
				*(x_WORD *)(v3 + *(x_DWORD *)(a1 + 164) + 819) = 0;
			}
		}
	}

2a1000


x_WORD_F4960[40]
versus
2c5960

x_BYTE_10B1E0
versus
2dc1e0-> ffffff
2818ac
call 281863
size_t sub_988A7_read(FILE* a1, Bit8u* a2, int a3)//2798a7
int sub_53E60_readfile_and_decompress(const char* path, Bit8u** a2)//234E60
void sub_54800_read_and_decompress_tables(Bit8u a1)//235800
void sub_56A30_init_game_level(unsigned int a1)//237a30

2c7ee0+0x14300
x_BYTE_F6EE0_tables
x_BYTE_10B1E0==x_BYTE_F6EE0_tables[0x14300]

pallette
1a7358 -> 000000 000101 000203 000305

35d7d8->7ffbffff40020000
LOBYTE(v56b) = sub_3E360(v217, v227);//21f01b

_UNKNOWN unk_D4328; // weak //2a5328
find 2a5328->ed01000000ffd8

x_DWORD_E9C38_smalltit 2bac38->a4523f00
BSCREEN2_BEGIN_BUFFER
27b3b1
signed int sub_9A32D_malloc_open_unpack(Pathstruct path)//27B32d
bool sub_84250_load_file_array(int psindex)//265250

3f52a4->7fecffff
281f26
int sub_A0F06(int a1, int a2, int a3, int a4, int a5, int a6, int a7)//281f06
27984f
__int16 sub_98B2C(unsigned __int8 a1, int a2)//180804 279b2c
__int16 sub_98AE9(__int16 *a1, int a2)//279ae9

21d2df
void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080

x_DWORD_DE558 - 2af558->3aa0a4
x_DWORD_180628b_screen_buffer
0x3aa0a4

29dd67 - void /*__spoils<>*/ sub_BCD45(Bit8u* a1, int a2, int a3, int a4, int a5)//29dd45

buffer - 3aa0a4 + 320*105 (3875812 = 3B23E4)

22304e __int16 sub_41BD3(unsigned int a1)//222bd3
sub_3E360(Bit8u* a1, Bit8u* a2)//20f360
unsigned __int16 sub_3FD60(/*int a1, */Bit8u* a2)//220d60

299a8e - call void /*__spoils<ecx>*/ sub_B6253(x_DWORD *a1, x_DWORD *a2, x_DWORD *a3)
21f00c
void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080

a1[1] versus 3550c0 - 0x51

x_DWORD_DE554
versus
2af554:649f3a00 a4a03a00
29dd6c call sub_BCD45(Bit8u* a1, int a2, int a3, int a4, int a5)//29dd45

3aa0a4-140=3a9f64
3a9f64:0204020402040204
2818ac

int sub_53E60_readfile_and_decompress(const char* path, Bit8u** a2)//234E60
v 23cf50

build0-0.dat
2bb3a8
2bb3ac
0138c0
build0-0.tab
2bb3bc
2bb3b0
01ce

3551c8->2a9c38
2bb3a8->3966e4
2a9c38

unk_DE56C - 2af56c
297b08

x_DWORD_DE55C-2af55c->34c68000

21efc5

x_DWORD_DDF50
2aef50
23c809 sub_5B7A0_prepare_pallette()//23C7A0

x_DWORD_B8845 299845:00000000 f1ffffffff
299961

1042c040
1042c4f0 - 4b0
49d

void sub_70F50(unsigned __int16 a1)//251f50
find i

x_D41A0_BYTEARRAY_4_struct.byteindex_26 [eax+1a] 38cf6a

musi nejdriv spustit void sub_718F0()//2528f0

porovnat chod
void sub_718F0()//2528f0

x_BYTE_F5340 2c6340
2528d9

kontrola char sub_71890()//252890

x_WORD_D951C 2aa51c

2ab7fc 0x1300

void sub_71410_process_tmaps()//252410

unk_D4350 2a5350

compare
void sub_63C90_draw_minimap_b(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244c90
v74[1] 18 versus 19
x_WORD_F4960 versus 2c5960
v67 = v13 / 2;
		v17 = &x_WORD_F4960[2 * (v13 / 2)];
		v68 = v17 - 2;
		v64 = 0;
		v18 = v12 >> 1;
		while (v67)
		{
			v19 = v18 * x_BYTE_F6EE0_tablesx[0x14300+v64 >> 16] >> 8;
			v17[0] = v18 + v19;
			v68[0] = v18 + v19;
			v17[1] = v18 - v19;
			v68[1] = v18 - v19;
			v68 -= 2;
			v17 += 2;
			v64 += 0x1000000 / (v13 >> 1);
			v67--;
		}

x_BYTE_F6EE0_tablesx[0x14300] versus 2dc1e0 - ffffffff
2818ac
call 281863
call size_t sub_988A7_read(FILE* a1, Bit8u* a2, int a3)//2798a7
call int sub_53E60_readfile_and_decompress(const char* path, Bit8u** a2)//234E60 
void sub_54800_read_and_decompress_tables(Bit8u a1)//235800

x_DWORD_180628b_screen_buffer+0x141
versus
3aa0a4+0x141

void sub_2D710_draw_menu(Bit8u* a1)//20e710
x_DWORD_F01E8(2, 2, filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[40]);

filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[40]
versus a67ac8->0xa5c78e->10898a8b8c8f
281f26
0x17e - 0x90, ale má být 0x37

20ef35 void sub_2DE80_draw_bitmapxx(__int16 a1, __int16 a2, posistruct a3)//20ee80
20cbc4 void sub_2BBB0(__int16 a1, __int16 a2, posistruct a3)//20cbb0
20e7c9 void sub_2D710_draw_menu(Bit8u* a1)//20e710

x_DWORD_180628b_screen_buffer+0x210
versus
3aa0a4+0x210 0x31(ma byt) versus 0x20

20ef2b void sub_2DE80_draw_bitmapxx(__int16 a1, __int16 a2, posistruct a3)//20ee80
20cbca void sub_2BBB0(__int16 a1, __int16 a2, posistruct a3)//20cbb0
20ed4e void sub_2D710_draw_menu(Bit8u* a1)//20e710

x_DWORD_180628b_screen_buffer+0x39a
versus
3aa0a4+0x39a 0x0c(ma byt) versus 0xf0

21d3db void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080

void sub_2E260_draw_spell(__int16 a1, __int16 a2, Bit8u* a3, char a4)//20f260

270ca4 
call void sub_8F935_bitmap_draw_final(Bit8u a1byte1, Bit8u a1byte2, Bit16u tiley, int tilex, Bit8u* texture, Bit8u setbyte, char a6)//270935
call void sub_8F920(Bit8u a1byte1, Bit8u a1byte2, Bit16s posx, Bit16s posy, Bit8u* a4, unsigned __int8 a5, char a6)//270920
call void sub_72C40_draw_bitmap_640_setcolor(Bit16s posx, Bit16s posy, posistruct a3, Bit8u color)//253c40
call void sub_6F940_sub_draw_text(Bit8u* textbuffer, int posx, int posy, Bit8u color)//250940
call void sub_2BC10_draw_text(Bit8u* textbuffer, Bit16s posx, Bit16s posy, Bit8u color)//20cc10
20f401 void sub_2E260_draw_spell(__int16 a1, __int16 a2, Bit8u* a3, char a4)//20f260

void sub_2E260_draw_spell(__int16 a1, __int16 a2, Bit8u* a3, char a4)//20f260

x_DWORD_180628b_screen_buffer+0x4ed
versus
3aa0a4+0x4ed 0x99(ma byt) versus 0x34

20ef2b call void sub_2DE80_draw_bitmapxx(__int16 a1, __int16 a2, posistruct a3)//20ee80

270a97 call void sub_8F935_bitmap_draw_final(Bit8u a1byte1, Bit8u a1byte2, Bit16u tiley, int tilex, Bit8u* texture, Bit8u setbyte, char a6)//270935


*(char *)(a3 + 64) tj 0x3633d6+0x40
	
232143 call Bit8u* sub_51120(Bit8u* a1, char a2, char a3)//232120

Bit8u* sub_51120(Bit8u* a1, char a2, char a3)//232120
2321c3 call Bit8u* sub_511C0(Bit8u* a1)//2321c0
call near Bit8u* sub_4A190(Bit8u* a1, int a2, int a3)//22b190
void sub_5CF40(Bit8u* a1, char a2)//23df40 - 23df72 - ecx=3 esi=1 edi=2
void sub_5C950(Bit8u* a1, Bit8u* a2)//23d950
void sub_51BB0_game_events(Bit8u* a1)//232bb0 //232dc5

355088

2321c7 Bit8u* sub_511C0(Bit8u* a1)//2321c0
23df77


	*(x_DWORD *)(v2 + 4) = 10000;
	sub_5CF40(v2, v8); - zde

if (*(x_WORD *)(2 * i + *(x_DWORD *)(a1 + 164) + 819))

*(Bit32s*)(a1 + 164)
versus
36332a->358ffc
358ffc+0x333(35932f)->990001

22b169 call Bit8u* sub_4A050()//22b050

Bit8u unk_D83AC[100]; // weak//2a93ac
Bit8u unk_F42B0[0x470]; // weak//2c52b0

235cd1 call int sub_54A50(unsigned __int16 a1, Bit8u* a2)//235a50
aaaaa

23dfb1 call void sub_5CF40(Bit8u* a1, char a2)//23df40

x_D41A0_BYTEARRAY_0 + 11230 + 1817=1//x_D41A0_BYTEARRAY_0 + 11230 + 0x3e6+0x333=1

358c16+1817=35932f


x_D41A0_BYTEARRAY_0 + 11230

unk_F42B0+819

unk_F42B0 versus 2c52b0
363286+a4=36332A->358ffc+0x333(35932f)->010001

sub_54A50 a2 versus 358c16
v0 = x_D41A0_BYTEARRAY_0 + 11230(2BDE)=0x358c16;
versus
x_D41A0_BYTEARRAY_0=356038
v int sub_53160()//234160

358c16+1817(0x719)=35932F
tj.
x_D41A0_BYTEARRAY_0 + 11230+0x719=x_D41A0_BYTEARRAY_0 + 11230+0x3e6  +0x333
x_D41A0_BYTEARRAY_0 + 11230+0x3e6 je *(Bit32s*)(a1 + 164)

void sub_54A50(unsigned __int16 a1, Bit8u* a2)//235a50

x_WORD_D93C0_bldgprmbuffer[2+0x13d+v5];
versus
byte_d94ff[v5]
versus [esi+2aa4ff]->0001020304

x_DWORD_180628b_screen_buffer+0x51d
versus
3aa0a4+0x51d 0x84(ma byt) versus 0x33

2451b9 call 244c90
call 244600
20d1af call int sub_2BE30_draw_game_frame()//20ce30

24342b call void sub_61A00(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242a00
void sub_61880_draw_minimap_entites(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242880

v55 = ((v73 * v53 + v86 * v52) >> 16) + v76;

(jx-28470-x_D41A0_BYTEARRAY_0)/168

__int16 x_WORD_F4962[830]; // idb 2c5962

244d86 void sub_63C90_draw_minimap_b(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244c90

x_DWORD_180628b_screen_buffer+0x59b
versus
3aa0a4+0x59b 0x34(ma byt) versus 0x3d

20ef2b void sub_2DE80_draw_bitmapxx(__int16 a1, __int16 a2, posistruct a3)//20ee80
20cbc4 call void sub_2BBB0(__int16 a1, __int16 a2, posistruct a3)//20cbb0
20ecaf void sub_2D710_draw_menu(Bit8u* a1)//20e710


void sub_2D710_draw_menu(Bit8u* a1)//20e710
x_DWORD_F01E8(v8, 2, filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[41]);
(i-v4 - x_DWORD_180628b_screen_buffer)/40 == 0x21

a3.data - 0xcd

differrence:
a5d570->95963031 3b3a3939
a3.data+0xe5

281f26
291c75
28b658
279786
291c75
void sub_47160()//228160

a5b3b0+0x21c0
*a2+0x21c0

0x25239DA8 0x40 42 89 8a 8b 8c 8f 90 92
0x25239DEA 0x40 42 89 8f 92 93 93 95 95
0x25239E2C 0x40 8a 91 95 96 96 30 30 31

a5d48c 0x40 -42
a5d510 0x40 -42
a5d4ce 0x40 -42
a5d552 0x40

A5D48C[20dc]
a5d56c 95 95 94 95[21bc] spatny(95953b3b)
a5d570(0xe4) 95 96 30 31 31 [21c0]

6c

20db versus 2020

0x24F80038

0x24F82113

c7 21a3

0x24BB8F36
filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[41].data 0x24BB8E6F
filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[41].data+0xe7
*path.var28_begin_buffer+0x20dc+0xe0

x_DWORD_180628b_screen_buffer+0x125d
versus
3aa0a4+0x125d 0xa1(ma byt) versus 0xa4

20ef2b
228557
20ed48

x_DWORD_F01E8(v12, 2, filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[41]);

edx=363286
[edx+8c]=[38d046]=0x3e8
38d046
38d04a

v18
38cf50+f6=38d046->4c79
24203d call Bit8u* sub_61000(Bit8u* a1)//242000
241f00 call void sub_60F00()//241f00
void sub_56A30_init_game_level(unsigned int a1)//237a30

void sub_60F00()//241f00

find init x_DWORD_EA3E4 versus 2bb3e4

x_DWORD_EA3E4[0x3e8]
2bb3e4
[2bb3e8]
[2bc384]->385f06
//fa0

23d36f call 23d330

196302 0x6604u=222418
224791 0x36e17

2bb3e8->000000001e
2bc384->065f3800

[2a51a0]->356038+2fece->385f06 tj. a2
385f06

2bb3e8->35cf6e->000000001e
35d016->00000000

problem je zde:
Bit8u* sub_48690(__int16 a1, __int16 a2, __int16 a3, __int16 a4)//229690

x_DWORD_180628b_screen_buffer+0x1cde
versus
3aa0a4+0x1c 0x1cde 0x24(ma byt) versus 0xd1

před
20d1af call void sub_63600_draw_minimap(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244600
void sub_63C90_draw_minimap_b(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244c90
2451b9

void sub_61880_draw_minimap_entites(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242880

ebp-5c

v6 = 30 * (signed __int16)a4; ?a4

addprocedurestop(0x242cf9, 0xa3, true, true, 0x3aa0a400 + 0x1cde);
-------------------
x_D41A0_BYTEARRAY_0 + 28470+168*0xa3
versus
363a66
0xc 0xe 0x14 0x15

22b137 call 22b050
0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x86,0x00,0x02,0x00,
0x00,0x00,0x00,0x00,0x7C,0x6F,0x00,0x00,0x00,0x00,0x7A,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x01,0xE5,0xFF,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFA,0x00,0x00,0x00,0x00,0xA5,0x0A,
0x00,0xFF,0xFF,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x4A,0x4C,0xDC,
0x35,0x12,0x7D,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x07,0x00,0x01,0x10,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

22b137 call Bit8u* sub_4A050()//22b050
22f329 void sub_4E320(Bit8u* a1)//22f320

2120cf
void sub_30F60(Bit8u* a1)//211f60
*(x_DWORD *)(v5 + 12) |= v6;//here

211eba
void sub_30D50(Bit8u* a1, Bit8u v2, Bit8u v4)//211d50

238a3d
void sub_57730()//238730

2003c0

--sub_21030
204340

x_DWORD_180628b_screen_buffer+0x26e2
versus
3aa0a4+0x26e2 0x32(ma byt) versus 0x35

x_D41A0_BYTEARRAY_0 + 28470+168*0xa3
versus
363a66
+0x1a

2120b3 call void sub_30F60(Bit8u* a1)//211f60
myprintf("xxx");
addprocedurestop(0x238a3d, 0x7a, true, true, 0x363a6600 + 0xc);

void sub_30F60(Bit8u* a1)//211f60
x_D41A0_BYTEARRAY_0 + 28470+168*0xa3
versus
363a66(35cf6e+6af8)
0xc - 84 versus 04
0x1a - 7a versus a4

04
------
238e46 call void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
void sub_4E320(Bit8u* a1)//22f320
void sub_30F60(Bit8u* a1)//211f60

2120cf call void sub_30F60(Bit8u* a1)//211f60
238a8e call void sub_57730()//238730

adress after 361f7e(35cf6e+7a*168)

22f320


test memory
x_D41A0_BYTEARRAY_0 + 28470
0x35cf6e
after
void sub_47560_draw_and_events_in_game(Bit8u* a1, int a2, Bit32u a3, signed int a4, __int16 a5)//228560
sub_57730

x_DWORD_180628b_screen_buffer+0x26e2
versus
3aa0a4+0x26e2 0x32(ma byt) versus 0x35

2451b9 void sub_63C90_draw_minimap_b(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244c90

v60 3aa0a4
0x038A6EF8

x_BYTE_10B4E0
versus
2dc4e0

3ac786-3aa0a4 - 1f=26c0
2580=v71-x_DWORD_180628b_screen_buffer

x_BYTE_F6EE0_tablesx[0x2302]
versus
2c7ee0->40404040

edi 3ac765

xx
423a384242423a3a423a333702 3a3a23

0x1d

0x21
        //addprocedurestop(0x244c90, 0x0, true, true, 0x3aa0a400 + 0x26c0);
---------------------
2ea12b - 48014848
versus
x_BYTE_10B4E0+0xdc4b

//03
225ef6 call void sub_44EE0(int a1, Bit16u a2)//225ee0
void sub_44E40(int a1, Bit8u a2)//225e40

//00040506
2ea12b - 2x
22622b call void sub_45210(Bit8u a1, Bit8u a2)//226210
void sub_43830_generate_level_map(unsigned int a1, Bit8u* a2)//224830
//2e
225cc7 call void sub_44580()//225580

//01017c
2272e1 call unsigned __int16 sub_46180(unsigned __int16 a1, char a2)//227180

//48
227411 call unsigned __int16 sub_46180(unsigned __int16 a1, char a2)//227180

//238730 call void sub_57730()//238730

------------------
363b0e+0x4c=184c07dc nikoli a34b13dc
versus
xxxx
238e40 call void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
22b190
21209f call void sub_30F60(Bit8u* a1)//211f60
238a8b call void sub_57730()//238730
361f7e-35cf6e=5010/168=122//7a
------------------
ebp-8
ebp-4

signed int sub_10130(int a1, x_DWORD *a2, x_DWORD *a3)//1f1130 - druhy pruchod?
-----------------------
test 2bac30->1a0038 000000 3f52a4 00000000 1a1038 000000 000000 a43aa4
xasearchd
versus
dword_E9C30

pstr[xx].var28
pstr[xx].var32
pstr[xx].var36

27b381

signed int sub_9A32D_malloc_open_unpack(Pathstruct path)//27B32d

fix:
void sub_101C0()//1f11c0
a kontola:
x_BYTE_E7EE0x
x_BYTE_E9980x
---------------------------------
analyze
signed int sub_9A32D_malloc_open_unpack(Pathstruct path)//27B32d

ebx=2a9a54
2a5365617263684400000000000000000000
00000000000000000000000000030ac2b->0000000000000
40ac2b->00000    0010        
---------------------------------
x_DWORD_180628b_screen_buffer+0x8f09
versus
3aa0a4+0x8f09 0x30(ma byt) versus 0x35

223039 call __int16 sub_41BD3(unsigned int a1)//222bd3

unk_F0E20?

-------------
x_DWORD_F2C30 a7
versus
2c3c30 //f6
21f517 call unsigned __int16 sub_3E360_draw_particles(Bit8u* a1, Bit8u* a2)//21f360
21f1b6 call void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080
222708 call void sub_411A0_draw_world(Bit8u* a1, int a2, int a3, __int16 a4, signed int a5, int a6, __int16 a7, int a8)//2221a0

3fdc24 - v227
3fdc50
3fdc7c
3fdca8
3fdcd4
3fdd00
3fdd2c
3fdd58
3fdd84
3fddb0
3fdddc
3fde08
3fde34
3fde60
3fde8c
3fdeb8

3f7110

if (*(x_WORD *)(v243 + 36))//adress 21f1b5 aex 360000 ebx 3f78a0 ecx 0 edx 414eb0
0 0 e4

addprocedurestop(0x21f1b5, 0x2, true, true, 0x2c3c3000);
-------------------------------
problem in 
3640f6
versus
x_DWORD_EA3E4[0xae]+0x56
+0x16 , 0x2c, 0x50, 0x56

0x56
&x_DWORD_E9C38_smalltit[35200-0x6384]+0x56

16
void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
Bit8u* sub_4E320(Bit8u* a1)//22f320
0xa


zm2nu hledat zde:
void sub_47320_in_game_loop(signed int a1)//228320
debugcounter==0xb
-----
32c4e0+0xd94f*2
versus
x_WORD_15B4E0[0xd94f]
-problem je i predtim

void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
Bit8u* sub_4E320(Bit8u* a1)//22f320
0x9

----------------
3622c6+0x14 =c237
versus
&x_DWORD_D4C52ar[0x17bc]+6+0x14=0385;

211fec call void sub_30F60(Bit8u* a1)//211f60

--------------------

void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080
v205[42] = v248x[32] + (((signed int)(unsigned __int8)x_BYTE_13B4E0[v204] >> 2) & 0x1C);

//x_D41A0_BYTEARRAY_0

