find  351660;

x_WORD_180662(351662)// neni treba

void sub_46B40()

signed int sub_5BF50()//23cf50

2a9a70
//30ac2b00

eax:1c - 1a0038 -0
edx:24 - 00001000
add =1a1038

typedef struct pathstruct {
	char path[ 1c];
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
[2BB3E0+i*4]=(i*168)+[2a51a0]+ 6E8E

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
		sub_728A9((void *)x_DWORD_180628b,  1E0u, v1);//[351628d]3aa0a4,1E0,0

3)chyab nacitani
	sub_7AA70((char*)"DATA/SCREENS/HSCREEN0.DAT", (Bit8u*)x_DWORD_17DEC0, (int)&unk_13B194, 548);

	najit puvod - 34ee48 - x_DWORD_17DE48c
	predany jako v2 v
	tj. 
	x_D41A0_BYTEARRAY_4[ E2] -38d032

	if ( !(unsigned __int16)sub_5BF50_load_psxdata() )//23C9F7 - 23CF50 //neco se soubory asi jejich naceteni, nebo jen soubor palety
	
	je to tu:
	x_D41A0_BYTEARRAY_4[ E2] = (Bit32s)sub_83CD0_malloc2(x_D41A0_BYTEARRAY_4[ E6]);

	4)
	zasek v:
	v4 = sub_7F7D0((unsigned int *)&v35, (x_DWORD*)&v33, v1, (int)v3);//2607d0

	5)
	zasek v:
	sub_728A9((void *)x_DWORD_180628b,  1E0u, v1);//[351628d]3aa0a4,1E0,0

	6)nedefinovana x_DWORD_D41BC - 2a51bc
	na radku:
	sub_5B870((x_BYTE *)x_DWORD_D41BC, (int)x_DWORD_E9C4C, 471);//fix it

	  sprintf_s(printbuffer,512, "%c%s/%s", x_D41A0_BYTEARRAY_4[ B6], ":/NETHERW/LANGUAGE", a4);
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
		while ( !x_BYTE_1806E4 && !x_WORD_180746 && !x_WORD_180744 && (j___clock(v5, v3, v1) - v4) /  64u <=  14 )

		11)
		zde se vzalo silene cislo
		x_D41A0_BYTEARRAY_4[ 4]

		sprintf_s(printbuffer,512,"%c%s/L%d.txt",x_D41A0_BYTEARRAY_4[ B6],":/NETHERW/LANGUAGE",x_D41A0_BYTEARRAY_4[ 4]);
      while ( v2 < 2 )

char aDataMsprd00Dat[18] = "data/msprd0-0.dat"; // weak
char aDataMsprd00Tab_0[18] = "data/msprd0-0.tab"; // weak
char aDataHsprd00Dat[18] = "data/hsprd0-0.dat"; // weak
char aDataHsprd00Tab_0[18] = "data/hsprd0-0.tab"; // weak

sub_8F8B0(0, 0, a1);//2708B0 super inicializace //a2 ma byt 86 nebo a1 nema nikam ukazovat

sub_8CD27(&x_D41A0_BYTEARRAY_4_ DE_heapbuffer);//26dd27 - najit kde se nastavuje
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

sub_90B27_VGA_pal_fadein_fadeout((Bit8u*)x_DWORD_17DE38,  20u, 0);//271b27 - tady se nacita paleta
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
sub_9A144_memcopy_640line((void*)x_DWORD_E9C38_smalltit, (void*)x_DWORD_180628b,  1E0u);//27b144
kopie z
x_DWORD_E9C38_smalltit - 2bac38->406514

x_DWORD_E9C38_smalltit nastaveno zde:
sub_7A110(x_WORD_180660_VGA_type_resolution, 14);//25b110
kde se to nahrava do x_D41A0_BYTEARRAY_4_ E2_heapbuffer - 91c59
x_D41A0_BYTEARRAY_4_ E2_heapbuffer - nenacita se derenc
stale se lisi na 3aa0a4+ f8a8, tj 3B994C(asi vlajka)
42 87 87 87 87
nahraje se opet zde:
sub_9A144_memcopy_640line((void*)x_DWORD_E9C38_smalltit, (void*)x_DWORD_180628b,  1E0u);//27b144
v39 = sub_7E0E0();//25f0e0 - ale vlajka prida zde
406514+ f8a8=415DBC

repair this function:
signed int sub_7E0E0()//25f0e0

repair this
void sub_8F935(doublebyte a1, Bit16u tilex, int a3, Bit8u* a4, unsigned __int8 a5, char a6)//270935

texture - 47be3b
sub_7AA70((char*)"DATA/SCREENS/HSCREEN0.DAT", &x_D41A0_BYTEARRAY_4_ E2_heapbuffer[ 4D54A], 168849, 102213);
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
          sub_98709_create_index_dattab_power((Bit8u*)(int)x_DWORD_17DED4, (Bit8u*)x_DWORD_17DED8, &x_D41A0_BYTEARRAY_4_ E2_heapbuffer[ 4D54A], (new posistruct));
        else
          sub_9874D_create_index_dattab((Bit8u*)(int)x_DWORD_17DED4, (Bit8u*)x_DWORD_17DED8, &x_D41A0_BYTEARRAY_4_ E2_heapbuffer[ 4D54A], (new posistruct));
        
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

//loadfromsnapshot2((char*)"0160-00262360", (Bit8u*)x_DWORD_17DED4,  34eed4,  300);//3e
  

x_DWORD_17DED4 - 34eed4? - 3dc74800d7c9480000000000ffffffff4001c8
v 257a40
v 2589e0
v 25b110

x_DWORD_17DED4_spritestr - 

x_DWORD_17DED4_spritestr[ 44].sizex - tohle uz je spatne

48c73d
48c9d7

fix this procedure:
int sub_7FCB0(int a1, Bit8u* a2, Bit32s a3, Bit32s a4, int a5, Bit8u a6, unsigned __int8 a7, Bit32u a8)//260cb0
v ní opravit:
sub_7FAE0(v87, v99, a4, v98, v86);

problemek:
v12 = sub_5BE80_index_pallette(x_DWORD_17DE38, 0, 0, v86);

v12 = sub_5BE80_index_pallette(x_DWORD_17DE38, 0, 0, v86);//mozny problem

 260ae0

v15 = x_D41A0_BYTEARRAY_4[v13 + 5]; - chyba, ma byt 07, ale je 00
x_D41A0_BYTEARRAY_4[ 1ec + 5]
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
chyba je v nastaveni x_DWORD_18062C_resolution_x(35162c) - coz ma byt  280 - 640

problém zde:

while ( v141 );
      }
      else if ( x_WORD_E36D4 &  40 )
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
 251c3 - 152003 323x237  143 x ed 
386x289 x182 x x121

-dosbox  3d74a6


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
          if ( (v130 &  80u) == 0 )
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
          if ( (v127 &  80u) == 0 )
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
                    if ( (v82 &  80u) == 0 )
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
                    if ( (a1 &  80u) == 0 )
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
                  if ( (v88 &  80u) == 0 )
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
                  if ( (a1 &  80u) == 0 )
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
                  if ( (v94 &  80u) == 0 )
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
                  if ( (a1 &  80u) == 0 )
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
                    if ( (v69 &  80u) == 0 )
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
                if ( (v69 &  80u) == 0 )
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
              if ( (v9 &  80u) == 0 )
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
              if ( (v16 &  80u) == 0 )
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
              if ( (v30 &  80u) == 0 )
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
              if ( (v39 &  80u) == 0 )
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
                if ( ((i &  80u) != 0) ^ v46 )
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
              if ( ((i &  80u) != 0) ^ v46 )
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
              while ( (v48 &  80u) != 0 );
              if ( !v48 )
                break;
              a4 += v48;
            }
          }
          v142 += dword_18062C;
          v41 = v142;
        }
      }
      else if ( word_E36D4 &  20 )
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
      else if ( word_E36D4 &  40 )
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
              if ( (v58 &  80u) == 0 )
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
              if ( (v63 &  80u) == 0 )
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
            if ( (v23 &  80u) == 0 )
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
              if ( (v97 &  80u) != 0 )
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
          if ( (v97 &  80u) != 0 )
            *v98++ = v104;
          --v100;
        }
        while ( v100 );
      }
      LOBYTE(v97) = v101 + v97;
      if ( (v97 &  80u) != 0 )
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
              if ( (v117 &  80u) != 0 )
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
          if ( (v117 &  80u) != 0 )
            *v118++ = v124;
          --v120;
        }
        while ( v120 );
      }
      LOBYTE(v117) = v121 + v117;
      if ( (v117 &  80u) != 0 )
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
            if ( (v109 &  80u) == 0 )
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
            if ( (a1 &  80u) == 0 )
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
            if ( (v115 &  80u) == 0 )
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
            if ( (a1 &  80u) == 0 )
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
if ((x_WORD_E1F84[i+12]& ff) && sub_7B200_in_region(&x_WORD_E1F84[i], x_DWORD_17DEE4_mouse_position, SHIWORD(x_DWORD_17DEE4_mouse_position)) )


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

	 35172c;

	pozor na toto:
	sub_8CD27_set_cursor(*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct/*filearray_2aa18c[0]*/);//26dd27
  x_WORD_17DEEC = 0;

  	//[34eed4]+de? =48c81b ->3abe47001315
	//25b110

	x_DWORD_17DED4_spritestr[ 25]

	x_DWORD_180700 - 351700 // tady mozna nevadi - nealokovane pole
	x_DWORD_1806F0 - 3516f0 // tady mozna take ne - nealokovane pole
	x_DWORD_180730 - 351730 - 191919 fefefefe		
	x_DWORD_18062C_resolution_x - 35162c musi byt 40

	vlajka se prekresli v 258cba - v prvnim kroku je puvodni vlajka, ale v druhem
	se vlajka nastavi

	4527bf->451516

	misto vlajky:
	 152C0

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
	sub_2EBB0_draw_text_with_border_63 340(x_DWORD_E9C4C_langindexbuffer[0]);//fix it
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
    sub_76260_read_intro_pallette( 100);
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
      v19 = sub_5BE80_test_pallette(unk_17D838,  3Fu,  3Fu,  3Fu);
      sub_2EC90(v19);//20fc90

zkontrolovat paletu:
unk_17D838
versus
34e838

barva
805c30 versus
000000

radek cca 169
posouva se o 31 radku -  26C0

Kafkar... 0 276 154 0
Kafkar... 0 276 154 0

setbyte is color

void sub_8F935_bitmap_draw_final(Bit8u a1byte1,Bit8u a1byte2, Bit16u tiley, int tilex, Bit8u* texture, Bit8u setbyte, char a6)//270935

	memset(x_DWORD_180628b_screen_buffer, 127, 320*200);//debug
		//VGA_Blit(320, 200, x_DWORD_180628b_screen_buffer);
	sub_2EBB0_draw_text_with_border_63 340(x_DWORD_E9C4C_langindexbuffer[a1]);//fix it - edx
	sub_90478_VGA_Blit320();//debug
}

void sub_loc_1B5BC(Bit16u a1) {
	memset(x_DWORD_180628b_screen_buffer, 200, 320*200);//debug
	Set_basic_pallette();
	sub_2EBB0_draw_text_with_border_63 340(x_DWORD_E9C4C_langindexbuffer[a1]);//fix it - edx
	sub_90478_VGA_Blit320();//debug
}

void sub_76160_play_intro(__int16 a1, __int16 a2, Bit8u* a3)//257160

x_DWORD_180628b_screen_buffer

 3aa0a4

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
 19f0ec, 19f7ec
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
 257160
	1fc280
		20fbb0(2ebb0)

2BB578
in
sub_5C1B0_set_any_variables2 - 23d1b0
sub_5C330(); - 23d330
a pote:

Bit32s x = *x_D41A0_BYTEARRAY_0 +  6e8e + 168 * i;//fix

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
 257160
	1fc280
		20fbb0(2ebb0)
			250c50
x_DWORD_EA3D4=x_DWORD_E9B20[0] nebo x_DWORD_E9B20[1]
Pathstruct x_DWORD_E9B20[2] = { xafonts0tab,xafonts1tab };?
sub_5B8D0_initialize_ 0023C8D0
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

x_BYTE_E25ED[ 2BB]

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

x_DWORD_17DEC0+ 18a

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
*pstr[ c].var28_begin_buffer

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

2bb3bc + 6 * 11

find
x_DWORD_EA3E4 -  2bb3e4
x_DWORD_EA3E8 -  2bb3e8
sub_5C330();//23D330

for (i = (int)&x_DWORD_EA3E0[2]; i < (int)&x_DWORD_EA3E0[ 3e9]; i += 168)
	{
		if (*(x_BYTE *)(i + 63) && *(x_BYTE *)(i + 13) & 4)
			sub_57F20(i);
	}

x_DWORD_EA3E0 +  6e8e - 35cec6

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
x_DWORD_EA3E0[1]= &x_D41A0_BYTEARRAY_0[ 6e8e];//28302
x_D41A0_BYTEARRAY_0=356038, rozdil  6e8e

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
 35cf6e -00000000 1e000000
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

 211d50 - maybe 40d50

5 2A5C57

 2a75c6-2a5c52=1974

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

 1e - find 1a713c e2d43900
call 281863
call 2798a7
call 234e60
call 27b32d
call 265250
call 23cf50

Bit8u* x_DWORD_EA3E0[ 3E9];//2bb3e0
27b453
call 27b32d
call 265250
call 228160

			sub_47FC0(v7);//vga smaltitle
			sub_56A30_init_game_level(a3);
			sub_47160();

find 363286 ->c6ce3500
238989 -int sub_57730()?
for (j = &x_DWORD_EA3E0[2]; j < &x_DWORD_EA3E0[ 3e9]; j += 168)
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

v20 = &x_DWORD_D4C52ar[14 * (*m)[69] + *(Bit32u*)(x_DWORD_D4C52ar + 18 * v19)- 2a5c52];
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
x_DWORD_EA3E4[ 98] not 363286 ->c6ce35
238989
void sub_57730()//238730
	for (j = &x_DWORD_EA3E4[1]; j < &x_DWORD_EA3E4[ 3e8]; j += 168)

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

355214 - data2-> 002212a0

22b375
void sub_4A310(unsigned __int16 *a1)//22b310

22b36c - adress maybe 8a36c
22b310
posxy = v11 + (v12 << 16);
v13 = sub_10C40((Bit16u*)&posxy);

22b379 adress

int sub_B5C60(Bit16u a1, Bit16u a2)//296c60


x_BYTE_11B4E0[ df4d]= 98
versus
[ df4d+2ec4e0]= 95 //2FA42D
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
	-bad count  6d5c

	void  (Bit8u* a1)//218240

	 bdaf - remc2
	 b14b - dosbox

	&x_BYTE_11B4E0[ b452] - 57555144
	versus
	002ec4e0 +  b452- 57555244

	2183f3
	tj.
		case  218240: {
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
 	&x_BYTE_11B4E0[ c35a] - 6d6d6a62
	versus
	002ec4e0 +  c35a- 7676726e

	&x_BYTE_11B4E0[ be54] - 55504949
	versus
	002ec4e0 +  be54- 675b4e4e

	 10 - ok
	 12 - ok
	 15 - error?
	 25 - ok
	 50 - ok
	 75 - ok
	 87 - ok
	 93 - ok
	 96 - ok
	 98 - ok
	 99 - ok
	 c0 - ok
	 d0 - ok
	 d8 - ok
	 d9 - error comp3 and comp4 e5e4
	 da - error comp3 and comp4 e3dd
	 dc - error comp3 and comp4 e3dd
	 e0 - error comp3 and comp4 bba9
	 100 - error comp3 and comp4 b93b
	 250 - error comp3 and comp4 b14b
	 580 - error?

	&x_BYTE_13B4E0[ e5e4] - 30c4e0 01414161
	&x_BYTE_10B4E0[ e5e4] - 2dc4e0 01010124
	pote
	&x_BYTE_13B4E0[ e3e4] - 30c4e0 d1b1b1b1/81818181
	&x_BYTE_10B4E0[ e3e4] - 2dc4e0 1a1a1a1a/ff010101

				x_BYTE_10B4E0[a3] = *v7;
			x_BYTE_13B4E0[a3] = x_BYTE_13B4E0[a3] &  8F | v7[1];
			sub_45DC0(/*v16, v12,*/ v48, v15[0]);
			sub_37240(a1);

find 2a5a30 unk_D4A30

	 10 - ok
	 12 - ok
	 15 - ok
	 25 - ok
	 50 - ok
	 75 - ok
	 87 - ok
	 93 - ok
	 96 - ok
	 98 - ok
	 99 - ok
	 c0 - ok
	 d0 - ok
	 d8 - ok
	 d9 - ok
	 120 - ok
	 200 - ok
	 400 - ok
	 500 - ok
	 540 - ok
	 560 - ok
	 562 - ok
	 563 - ok
	 564 - ok
	 565 - ok
	 570 - error comp2 c143 comp5 c544
	 582 - error comp2 a546 comp5 b24a

	&x_BYTE_13B4E0[ e3e2] - 30c4e0 41615131
	&x_BYTE_10B4E0[ e3e2] - 2dc4e0 01010101
	pote
	&x_BYTE_13B4E0[ e3e2] - 30c4e0 21514101
	&x_BYTE_10B4E0[ e3e2] - 2dc4e0 01010101

	e3e2

	e6e5
	void sub_45DC0(Bit8u a1, Bit8u a2, unsigned __int16 a3, unsigned __int8 a4)//226dc0
void sub_462A0(unsigned __int16 a1, __int16 a2)//2272a0
d24f
c648

comp2
	&x_BYTE_11B4E0[ c648] - 2ec4e0 01010101		

comp2
	&x_BYTE_11B4E0[ c445] - 82829d71  2ec4e0	tj.7f888582	
	
filearray_2aa18c[filearrayindex_BUILD00DATTAB]
versus
1a7124->24cf39

(debugcounter2 >=  2340)
az
(debugcounter2 >=  2520)

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
 c749
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

x_WORD_15B4E0,  32c4e0
2856
x_WORD_15B4E0+ 2856 = 32ED36
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
 99 - ok
 9a -  34e(dbox) -  34e
 9b -  34d(dbox) -  3ba
 9c -  34c(dbox) -  34c
 a0 -  348(dbox) -  3b5
 b0 -  338(dbox) -  338
 c0 - je to preskoceno  328
100 - za

unsigned int sub_6F150(Bit8u* a1)//250150 - kontrola

find 363286
238989 void sub_57730()//238730
[2a51a4]
*(Bit32u*)&x_DWORD_EA3E4[ 98]
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
*(Bit32u*)(x_D41A0_BYTEARRAY_0 + 4 *  376+ 582)

211f60
2130e0

sub_47320(a2);

sub_47560(index, v4, v1, a1, x_DWORD_17DB54_game_turn2);
sub_57730();

find
x_DWORD_E7EE0 - 2b8ee0

void sub_101C0()//1f11c0
x_DWORD_E998 [k].pointer = &dword_E9C30[0][l];

dword_E9C30
x_DWORD_E998 

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

unk_D89A6;  2a99a6
unk_D849A;  2a949a

void sub_47760(/*int a1,*/Bit32u user/* int a2, int a3*/)//228760

sub_2E260_draw_spell(//draw spell1

eax+ 333
tj.
35932f-> 99
*(*(Bit32s*)(v3 + 164)+ 333)
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

2c7ee0+ 14300
x_BYTE_F6EE0_tables
x_BYTE_10B1E0==x_BYTE_F6EE0_tables[ 14300]

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
 3aa0a4

29dd67 - void /*__spoils<>*/ sub_BCD45(Bit8u* a1, int a2, int a3, int a4, int a5)//29dd45

buffer - 3aa0a4 + 320*105 (3875812 = 3B23E4)

22304e __int16 sub_41BD3(unsigned int a1)//222bd3
sub_3E360(Bit8u* a1, Bit8u* a2)//20f360
unsigned __int16 sub_3FD60(/*int a1, */Bit8u* a2)//220d60

299a8e - call void /*__spoils<ecx>*/ sub_B6253(x_DWORD *a1, x_DWORD *a2, x_DWORD *a3)
21f00c
void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080

a1[1] versus 3550c0 -  51

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

2ab7fc  1300

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
			v19 = v18 * x_BYTE_F6EE0_tablesx[ 14300+v64 >> 16] >> 8;
			v17[0] = v18 + v19;
			v68[0] = v18 + v19;
			v17[1] = v18 - v19;
			v68[1] = v18 - v19;
			v68 -= 2;
			v17 += 2;
			v64 +=  1000000 / (v13 >> 1);
			v67--;
		}

x_BYTE_F6EE0_tablesx[ 14300] versus 2dc1e0 - ffffffff
2818ac
call 281863
call size_t sub_988A7_read(FILE* a1, Bit8u* a2, int a3)//2798a7
call int sub_53E60_readfile_and_decompress(const char* path, Bit8u** a2)//234E60 
void sub_54800_read_and_decompress_tables(Bit8u a1)//235800

x_DWORD_180628b_screen_buffer+ 141
versus
3aa0a4+ 141

void sub_2D710_draw_menu(Bit8u* a1)//20e710
x_DWORD_F01E8(2, 2, filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[40]);

filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[40]
versus a67ac8-> a5c78e->10898a8b8c8f
281f26
 17e -  90, ale má být  37

20ef35 void sub_2DE80_draw_bitmapxx(__int16 a1, __int16 a2, posistruct a3)//20ee80
20cbc4 void sub_2BBB0(__int16 a1, __int16 a2, posistruct a3)//20cbb0
20e7c9 void sub_2D710_draw_menu(Bit8u* a1)//20e710

x_DWORD_180628b_screen_buffer+ 210
versus
3aa0a4+ 210  31(ma byt) versus  20

20ef2b void sub_2DE80_draw_bitmapxx(__int16 a1, __int16 a2, posistruct a3)//20ee80
20cbca void sub_2BBB0(__int16 a1, __int16 a2, posistruct a3)//20cbb0
20ed4e void sub_2D710_draw_menu(Bit8u* a1)//20e710

x_DWORD_180628b_screen_buffer+ 39a
versus
3aa0a4+ 39a  0c(ma byt) versus  f0

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

x_DWORD_180628b_screen_buffer+ 4ed
versus
3aa0a4+ 4ed  99(ma byt) versus  34

20ef2b call void sub_2DE80_draw_bitmapxx(__int16 a1, __int16 a2, posistruct a3)//20ee80

270a97 call void sub_8F935_bitmap_draw_final(Bit8u a1byte1, Bit8u a1byte2, Bit16u tiley, int tilex, Bit8u* texture, Bit8u setbyte, char a6)//270935


*(char *)(a3 + 64) tj  3633d6+ 40
	
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
358ffc+ 333(35932f)->990001

22b169 call Bit8u* sub_4A050()//22b050

Bit8u unk_D83AC[100]; // weak//2a93ac
Bit8u unk_F42B0[ 470]; // weak//2c52b0

235cd1 call int sub_54A50(unsigned __int16 a1, Bit8u* a2)//235a50
aaaaa

23dfb1 call void sub_5CF40(Bit8u* a1, char a2)//23df40

x_D41A0_BYTEARRAY_0 + 11230 + 1817=1//x_D41A0_BYTEARRAY_0 + 11230 +  3e6+ 333=1

358c16+1817=35932f


x_D41A0_BYTEARRAY_0 + 11230

unk_F42B0+819

unk_F42B0 versus 2c52b0
363286+a4=36332A->358ffc+ 333(35932f)->010001

sub_54A50 a2 versus 358c16
v0 = x_D41A0_BYTEARRAY_0 + 11230(2BDE)= 358c16;
versus
x_D41A0_BYTEARRAY_0=356038
v int sub_53160()//234160

358c16+1817( 719)=35932F
tj.
x_D41A0_BYTEARRAY_0 + 11230+ 719=x_D41A0_BYTEARRAY_0 + 11230+ 3e6  + 333
x_D41A0_BYTEARRAY_0 + 11230+ 3e6 je *(Bit32s*)(a1 + 164)

void sub_54A50(unsigned __int16 a1, Bit8u* a2)//235a50

x_WORD_D93C0_bldgprmbuffer[2+ 13d+v5];
versus
byte_d94ff[v5]
versus [esi+2aa4ff]->0001020304

x_DWORD_180628b_screen_buffer+ 51d
versus
3aa0a4+ 51d  84(ma byt) versus  33

2451b9 call 244c90
call 244600
20d1af call int sub_2BE30_draw_game_frame()//20ce30

24342b call void sub_61A00(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242a00
void sub_61880_draw_minimap_entites(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242880

v55 = ((v73 * v53 + v86 * v52) >> 16) + v76;

(jx-28470-x_D41A0_BYTEARRAY_0)/168

__int16 x_WORD_F4962[830]; // idb 2c5962

244d86 void sub_63C90_draw_minimap_b(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244c90

x_DWORD_180628b_screen_buffer+ 59b
versus
3aa0a4+ 59b  34(ma byt) versus  3d

20ef2b void sub_2DE80_draw_bitmapxx(__int16 a1, __int16 a2, posistruct a3)//20ee80
20cbc4 call void sub_2BBB0(__int16 a1, __int16 a2, posistruct a3)//20cbb0
20ecaf void sub_2D710_draw_menu(Bit8u* a1)//20e710


void sub_2D710_draw_menu(Bit8u* a1)//20e710
x_DWORD_F01E8(v8, 2, filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[41]);
(i-v4 - x_DWORD_180628b_screen_buffer)/40 ==  21

a3.data -  cd

differrence:
a5d570->95963031 3b3a3939
a3.data+ e5

281f26
291c75
28b658
279786
291c75
void sub_47160()//228160

a5b3b0+ 21c0
*a2+ 21c0

 25239DA8  40 42 89 8a 8b 8c 8f 90 92
 25239DEA  40 42 89 8f 92 93 93 95 95
 25239E2C  40 8a 91 95 96 96 30 30 31

a5d48c  40 -42
a5d510  40 -42
a5d4ce  40 -42
a5d552  40

A5D48C[20dc]
a5d56c 95 95 94 95[21bc] spatny(95953b3b)
a5d570( e4) 95 96 30 31 31 [21c0]

6c

20db versus 2020

 24F80038

 24F82113

c7 21a3

 24BB8F36
filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[41].data  24BB8E6F
filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[41].data+ e7
*path.var28_begin_buffer+ 20dc+ e0

x_DWORD_180628b_screen_buffer+ 125d
versus
3aa0a4+ 125d  a1(ma byt) versus  a4

20ef2b
228557
20ed48

x_DWORD_F01E8(v12, 2, filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[41]);

edx=363286
[edx+8c]=[38d046]= 3e8
38d046
38d04a

v18
38cf50+f6=38d046->4c79
24203d call Bit8u* sub_61000(Bit8u* a1)//242000
241f00 call void sub_60F00()//241f00
void sub_56A30_init_game_level(unsigned int a1)//237a30

void sub_60F00()//241f00

find init x_DWORD_EA3E4 versus 2bb3e4

x_DWORD_EA3E4[ 3e8]
2bb3e4
[2bb3e8]
[2bc384]->385f06
//fa0

23d36f call 23d330

196302  6604u=222418
224791  36e17

2bb3e8->000000001e
2bc384->065f3800

[2a51a0]->356038+2fece->385f06 tj. a2
385f06

2bb3e8->35cf6e->000000001e
35d016->00000000

problem je zde:
Bit8u* sub_48690(__int16 a1, __int16 a2, __int16 a3, __int16 a4)//229690

x_DWORD_180628b_screen_buffer+ 1cde
versus
3aa0a4+ 1c  1cde  24(ma byt) versus  d1

před
20d1af call void sub_63600_draw_minimap(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244600
void sub_63C90_draw_minimap_b(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244c90
2451b9

void sub_61880_draw_minimap_entites(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242880

ebp-5c

v6 = 30 * (signed __int16)a4; ?a4

addprocedurestop( 242cf9,  a3, true, true,  3aa0a400 +  1cde);
-------------------
x_D41A0_BYTEARRAY_0 + 28470+168* a3
versus
363a66
 c  e  14  15
35CB30
22b137 call 22b050
 00, 00, 00, 00, 08, 00, 00, 00, 07, 00, 00, 00, 86, 00, 02, 00,
 00, 00, 00, 00, 7C, 6F, 00, 00, 00, 00, 7A, 00, 00, 00, 00, 00,
 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 90, 01, E5, FF, 00, 00,
 00, 00, 00, 00, 00, 00, 00, 00, 00, FA, 00, 00, 00, 00, A5, 0A,
 00, FF, FF, 0A, 00, 00, 00, 00, 00, 00, 00, 00, E0, 4A, 4C, DC,
 35, 12, 7D, 00, 80, 00, 80, 00, 80, 00, 07, 00, 01, 10, 00, 00,
 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,

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

x_DWORD_180628b_screen_buffer+ 26e2
versus
3aa0a4+ 26e2  32(ma byt) versus  35

x_D41A0_BYTEARRAY_0 + 28470+168* a3
versus
363a66
+ 1a

2120b3 call void sub_30F60(Bit8u* a1)//211f60
myprintf("xxx");
addprocedurestop( 238a3d,  7a, true, true,  363a6600 +  c);

void sub_30F60(Bit8u* a1)//211f60
x_D41A0_BYTEARRAY_0 + 28470+168* a3
versus
363a66(35cf6e+6af8)
 c - 84 versus 04
 1a - 7a versus a4

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
 35cf6e
after
void sub_47560_draw_and_events_in_game(Bit8u* a1, int a2, Bit32u a3, signed int a4, __int16 a5)//228560
sub_57730

x_DWORD_180628b_screen_buffer+ 26e2
versus
3aa0a4+ 26e2  32(ma byt) versus  35

2451b9 void sub_63C90_draw_minimap_b(int a1, int a2, int a3, int a4, int a5, int a6, __int16 a7, int a8, int a9, int a10)//244c90

v60 3aa0a4
 038A6EF8

x_BYTE_10B4E0
versus
2dc4e0

3ac786-3aa0a4 - 1f=26c0
2580=v71-x_DWORD_180628b_screen_buffer

x_BYTE_F6EE0_tablesx[ 2302]
versus
2c7ee0->40404040

edi 3ac765

xx
423a384242423a3a423a333702 3a3a23

 1d

 21
        //addprocedurestop( 244c90,  0, true, true,  3aa0a400 +  26c0);
---------------------
2ea12b - 48014848
versus
x_BYTE_10B4E0+ dc4b

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
363b0e+ 4c=184c07dc nikoli a34b13dc
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
x_BYTE_E7EE 
x_BYTE_E998 
---------------------------------
analyze
signed int sub_9A32D_malloc_open_unpack(Pathstruct path)//27B32d

ebx=2a9a54
2a5365617263684400000000000000000000
00000000000000000000000000030ac2b->0000000000000
40ac2b->00000    0010        
---------------------------------
x_DWORD_180628b_screen_buffer+ 8f09
versus
3aa0a4+ 8f09  30(ma byt) versus  35

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

addprocedurestop( 21f1b5,  2, true, true,  2c3c3000);
-------------------------------
problem in 
3640f6
versus
x_DWORD_EA3E4[ ae]+ 56
+ 16 ,  2c,  50,  56

 56
&x_DWORD_E9C38_smalltit[35200- 6384]+ 56

16
void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
Bit8u* sub_4E320(Bit8u* a1)//22f320
 a


zm2nu hledat zde:
void sub_47320_in_game_loop(signed int a1)//228320
debugcounter== b
-----
32c4e0+ d94f*2
versus
x_WORD_15B4E0[ d94f]
-problem je i predtim

void sub_57D70(Bit8u* a1, Bit8u* a2)//238d70
Bit8u* sub_4E320(Bit8u* a1)//22f320
 9

----------------
3622c6+ 14 =c237
versus
&x_DWORD_D4C52ar[ 17bc]+6+ 14=0385;

211fec call void sub_30F60(Bit8u* a1)//211f60

--------------------

void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080
v205[42] = v248x[32] + (((signed int)(unsigned __int8)x_BYTE_13B4E0[v204] >> 2) &  1C);

//x_D41A0_BYTEARRAY_0

3f78a0+ 56
versus
*(Bit32u*)(&x_DWORD_E9C38_smalltit[ 25fc]+ 56)
0002, ale má být nula
---
3640f6+ 50 - 9c107d
versus
x_DWORD_EA3E4[ ae]+ 50 88107d
if (debugcounter5 >=  0a)


x_DWORD_F2C5C - d4db
versus
2c3c5c - 10000

21d121 call void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080

x_DWORD_F2C7C - 67be
versus
2c3c7c - 00000


x_DWORD_DB350 alespon 100
versus
2ac350
-----------------------------------
step 2
x_DWORD_180628b_screen_buffer+ 51d
versus
3aa0a4+ 51d
62 versus 84
1309 tj radek 4 sl. 29

24342b call void sub_61A00_draw_minimap_entites_b(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242a00

363286
versus
x_DWORD_EA3E4[ 98]
+ 1c-00000000 -void sub_5D530(Bit8u* a1)//*(x_DWORD *)(a1 + 160)//23e530
+ 3c-00009903 -sub_57730(); pod

!!!!!!! zkontrolovat - x_D41A0_BYTEARRAY_0 + 28470; jx < x_D41A0_BYTEARRAY_0 + 196302
------------------------------------------
adress 22787a
x_D41A0_BYTEARRAY_0
versus
 356038
---36e16 - ok //x_D41A0_BYTEARRAY_0+ 36e16
------------------------------------------

unk_F42B0[ 470];
------------------------------------------
x_D41A0_BYTEARRAY_0
versus
 356038
--- 235
227934
--- 246
--- 2f7d
2342f8 void sub_53160()//234160 //x_D41A0_BYTEARRAY_0+ 2f7d
356038
--- 314d
//x_D41A0_BYTEARRAY_0+ 314d
23db6d void sub_5C950(Bit8u* a1, Bit8u* a2)//23d950
---x_D41A0_BYTEARRAY_0+ 6f32 void sub_49EC0(Bit16u* a1, __int16 a2)//22aec0
---x_D41A0_BYTEARRAY_0+ 796e //x_D41A0_BYTEARRAY_0+ 796e
1f1e82 int sub_10C80(Bit8u* a1, unsigned __int8 a2, unsigned __int16 a3)//1f1c80
void sub_30D50(Bit8u* a1, Bit8u v2, Bit8u v4)//211d50
---x_D41A0_BYTEARRAY_0+ 8fa6 // 356038+ 8fa6
1f33a7 void sub_12330(Bit8u* a1, __int16 a2)//1f3330
1f3203 __int16 sub_12100(int a1, Bit8u* a2, char a3)//1f3100

1f33aa void sub_12330(Bit8u* a1, __int16 a2)//1f3330

--- 356038+ 365fc //x_D41A0_BYTEARRAY_0+ 365fc
+8=38C63C
adress 1f2f61 signed int sub_11EE0(Bit8u* a1)//1f2ee0

---x_D41A0_BYTEARRAY_0+ 904e // 356038+ 904e
adress 1f33aa void sub_12330(Bit8u* a1, __int16 a2)//1f3330
1f3208
void sub_12100(Bit8u* a1, Bit8u* a2, char a3)//1f3100
****
35ef96+ 10
versus
xx
***adress 22b257
zjisti kolikaty pruchod 4a310 zpusobi zmenu 35ef96+ 10
void sub_4A310(unsigned __int16 *a1)//22b310
-predtim to che otestovat 386399 !!!!!!
---------------------------------------
---x_D41A0_BYTEARRAY_0+ 10 // 356038+ 10

---------------------------------------
---------------------------------------
-----------------------------------
step 2
x_DWORD_180628b_screen_buffer+ 51d
versus
3aa0a4+ 51d
62 versus 84
1309 tj radek 4 sl. 29

24342b call void sub_61A00_draw_minimap_entites_b(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242a00
test druheho pruchodu x_D41A0_BYTEARRAY_0

x_BYTE_E88E0 -- 2b98e0
22920f
--------------------------------
x_DWORD_180628b_screen_buffer+ ddd0
versus
3aa0a4+ ddd0


unk_F0E20 2c1e20
----
x_D41A0_BYTEARRAY_4_struct.byteindex_121[2]
versus
38cfcb
if (debugcounter2 >=  46a)
232d0a void sub_51BB0_game_events(Bit8u* a1)//232bb0
---------------------------------------------------
--nejprve zpet k testovani pole x_D41A0_BYTEARRAY_0

---x_D41A0_BYTEARRAY_0+ 2fc8 // 356038+ 2fc8
00 versus c2
v63 = (Bit8u*)(*(int(**)(Bit8u*))((char *)&off_D781E + 14 * v112))(v113 + 76);
---------------------------------------------------
358ffc
versus
*(Bit32s*)(*x_DWORD_EA3E4+ 63c0+164)
------------------------------------
35ce76
versus
x_D41A0_BYTEARRAY_0 + 28222

sub_17060
x_D41A0_BYTEARRAY_0[10 * *(Bit16u*)&x_D41A0_BYTEARRAY_0[12] + 28226] = v4;
sub_17060(x_DWORD_1805B0, x_DWORD_1805B4);
sub_17A00((Bit8u*)a3, a4, a5);//fix it//nothing draw
---------------------------------------------------
35ce76 -01
versus
x_D41A0_BYTEARRAY_0[10 * x_D41A0_BYTEARRAY_4_struct.byteindex_12 + 28222]-00


----------------------------
test druhy pruchod //adress 228583
---x_D41A0_BYTEARRAY_0+ 6e41 // 356038+ 6e41
-orig 00
-remake 81


unk_180560
versus
351560

35ce76 -01
versus
&x_D41A0_BYTEARRAY_0[10 * x_D41A0_BYTEARRAY_4_struct.byteindex_12 + 28222]
--------------------------------
x_DWORD_180628b_screen_buffer+ 51d
versus
3aa0a4+ 51d  62(ma byt) versus  84

24342b call void sub_61A00_draw_minimap_entites_b(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//242a00

x_D41A0_BYTEARRAY_4_struct.byteindex_121[2]
versus
38cfcb

322d0a void sub_51BB0_game_events(Bit8u* a1)//232bb0

byteindex_12
*(Bit16s*)&x_D41A0_BYTEARRAY_0[12]

x_D41A0_BYTEARRAY_4_struct.byteindex_12
v16 = *(x_DWORD *)(2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + x_D41A0_BYTEARRAY_0 + 11248) / (unsigned int)v15 & 1;

---x_D41A0_BYTEARRAY_0+ 7b // 356038+ 7b

sub_47760(/*int a1,*/Bit32u user/* int a2, int a3*/)//228760

252973 int sub_71930()//252930

x_DWORD_F66F0+ 70  2c76f0+ 70
 78
00 00 poi poi-orig
poi poi poi poi
poi poi poi poi
poi poi poi poi

00 00 00 00
poi poi poi poi
poi poi poi poi
00 00 00 00

---------------------------------------
void sub_70F50(unsigned __int16 a1)//251f50
252842 void sub_71780()//252780

x_DWORD_F66F0[ 42]
2c6340+ 108

251ffa
---------------
2c77f8(2c76f0+ 108) ->006d4000
versus
x_DWORD_F66F0[ 42]
---------------
x_BYTE_F5538[ 42]
versus
2c657a

252771 call void sub_71730(unsigned __int16 a1)//252730
252712 void sub_716C0(unsigned __int16 a1, unsigned __int16 a2, unsigned __int16 a3)//2526c0

---x_D41A0_BYTEARRAY_0+ 120a // 356038+ 120a
26-orig versus 66-remake
12b2

22b046 call void sub_49F90()//22af90
*(x_DWORD *)(v7 + 582) = (Bit32s)x_DWORD_EA3E4[v3]

unk_D83AC - zrusit - sloucit

---x_D41A0_BYTEARRAY_0+ 12aa // 356038+ 12aa
7e versus 00
22b046 call void sub_49F90()//22af90
*(x_DWORD *)(v7 + 582) = (Bit32s)x_DWORD_EA3E4[v3]
2360e4 call char sub_55080(unsigned __int8 a1, int a2)//236080
238663 call void sub_57640()//238640
2288a1 void sub_47760(/*int a1,*/Bit32u user/* int a2, int a3*/)//228760
228564 void sub_47560_draw_and_events_in_game(Bit8u* a1, int a2, Bit32u a3, signed int a4, __int16 a5)//228560
-------------------------
--*(x_DWORD *)(v4 + 4582);
Bit8u* sub_57F20(Bit8u* a1)//238f20
void sub_57730()//238730

predtim
 362026+ 14, 18, 20, 4d, 4e, 4f
00000000 557f0000 a9007c00 00000000
versus
x_D41A0_BYTEARRAY_0+ bfee

557f
21205f call void sub_30F60(Bit8u* a1)//211f60


x_D41A0_BYTEARRAY_0+ 6f36

x_D41A0_BYTEARRAY_0+ 8f7e// 356038+ 8f7e

2306
8b06

x_D41A0_BYTEARRAY_0+ 8f97// 356038+ 8f97

249d37 call int sub_68C70(int a1)//249c70
249c1b void sub_68BF0()//249bf0

x_D41A0_BYTEARRAY_4_struct.bytearray_38403
versus
396553+4

238849 void sub_57730()//238730

x_D41A0_BYTEARRAY_0+ d886// 356038+ d886
56 versus 51

238e42 d60f
238e42 d60f19
238e42 561019 call void sub_57CF0(Bit8u* a1, Bit8u* a2)//238cf0
190

x_D41A0_BYTEARRAY_0+ d570// 356038+ d570
99 versus 94
---------------
x_D41A0_BYTEARRAY_0+ 36df6// 356038+ 36df6
ac versus f6
2362ae call char sub_55250(unsigned __int8 a1)//236250

char sub_55250(unsigned __int8 a1)//236250

2a93ac
------------------
x_D41A0_BYTEARRAY_0+ 12aa// 356038+ 12aa
7e versus 00

37
--------------------------------------------
x_DWORD_180628b_screen_buffer+ 9b6e//3aa0a4+ 9b6e
37 versus 77

299b6c call void /*__spoils<ecx>*/ sub_B6253_draw_quad(x_DWORD *a1, x_DWORD *a2, x_DWORD *a3)//297253
21f064 call void sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9)//21d080

	if (*( 36E03 + x_D41A0_BYTEARRAY_0 + 1))
		sub_30630();

----------------------------
x_D41A0_BYTEARRAY_0+ 12aa// 356038+ 12aa
7e versus 00
//pruchod 3
22b046 call void sub_49F90()//22af90
2360df call char sub_55080(unsigned __int8 a1, int a2)//236080
2288a1 call void sub_57640()//238640
228564 call void sub_47760(/*int a1,*/Bit32u user/* int a2, int a3*/)//228760

problem je zde
Bit8u* sub_57F20(Bit8u* a1)//238f20
void sub_57730()//238730
------------------------------------------------
x_DWORD_180628b_screen_buffer+ 1f71//3aa0a4+ 1f71
b8 versus 37
-blinking rectangle
245db2 call void char sub_64CE0(int a1)//245ce0
24595b void sub_644F0(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//2454f0
v void sub_644F0(int a1, int a2, __int16 a3, __int16 a4, int a5, int a6, __int16 a7, int a8)//2454f0

difference:
 2c(44), 2d(45)

-------------------
pruchod 7
x_D41A0_BYTEARRAY_0+ 9022// 356038+ 9022
c2 versus ee
2d - signed int sub_1B8C0(Bit8u* a1)//1fc8c0
83 - signed int sub_1B8C0(Bit8u* a1)//1fc8c0
d8 - signed int sub_1B8C0(Bit8u* a1)//1fc8c0
ee - signed int sub_1B8C0(Bit8u* a1)//1fc8c0

8003
9603 - 1fc9d8 call sub_1B8C0(Bit8u* a1)//1fc8c0
--
ac03
--
c203 - 1fc9d8 call sub_1B8C0(Bit8u* a1)//1fc8c0
1ff2cf - char sub_1E1C0(Bit8u* a1, __int16 a2)//1ff1c0
1ff02e - char sub_1E020(Bit8u* a1, __int16 a2)//1ff020

porovnat x_DWORD_EA3E4 tj 2bb3e4

void sub_41A90_VGA_pallette_install(Bit8u* buffer)//222a90
271c10 call Bit16s sub_90B27_VGA_pal_fadein_fadeout(Bit8u *newpalbuffer, Bit8u shadow_levels, bool singlestep)//271B27 init and nightfall
void sub_47760(/*int a1,*/Bit32u user/* int a2, int a3*/)//228760
fix:
outbuffer[i] = x_BYTE_181544_oldpalbuffer[i] + (j* (newpalbuffer[i] - x_BYTE_181544_oldpalbuffer[i]) / shadow_levels);

2288da call void sub_47760(/*int a1,*/Bit32u user/* int a2, int a3*/)//228760

 1000 - ok
 13b5 - ok
 179c - ok
 1b83
2d05
 1f00

-----------------------------
krok2
unk_1804B0ar+ 86 versus 3514b0+ 86
4f versus 13
---------
array_E2A74+ 22 versus 2b3a74+ 22
1e versus 32
---------
array_E2A74+ 21f versus 2b3a74+ 21f
04 versus 00
---------
array_E2A74+ 246 versus 2b3a74+ 246
76 versus 1e
---------
array_E2A74+ 88d versus 2b3a74+ 88d
76 versus 1e

void sub_88B20()//269b20

x_D41A0_BYTEARRAY_0[10] versus  38cf50+ a
1 versus 0
23cd17

---------
array_E2A74+ 22 versus 2b3a74+ 22
1e versus 1f

Bit8u* sub_87B70()//268b70
-----------------------------
krok2
unk_1804B0ar+ 7a versus 3514b0+ 7a
0 versus 32

void sub_884D0(int a1, __int16 a2, Bit8u* a3, char a4, char a5)//2694d0

int sub_88450()//269450
-----------------------------
krok2
unk_1804B0ar+ 86 versus 3514b0+ 86

4f versus 13

2b3aa2->5802
versus &array_E2A74[ 2e]

test
unk_1804B0ar+ 86+ 4a

2b3cae+ 10
versus &array_E2A74[ 1e]+ 22c
a0 versus a1

unk_1804B0ar+ 86 versus 3514b0+ 86

4f versus 13

int sub_871F0()//2681f0

2b43b6+2
versus
array_E2A74+ 942 +2

int sub_87CF0()//268cf0

char x_BYTE_E33B8 = '\x04'; // weak//fix
---------
krok2
array_E2A74+ 06 versus 2b3a74+ 06
01 versus 0
--------
krok4(3)
unk_1804B0ar+ 8e versus  3514b0+ 8e
55 versus 5

&array_E2A74[ 944] versus 2b43ba

void sub_884D0(int a1, __int16 a2, Bit8u* a3, char a4, char a5)//2694d0
---------
krok65
array_E2A74+ 06 versus 2b3a74+ 06
01 versus 0
---------
krok 41
x_DWORD_180628b_screen_buffer+ 1e29
versus
3aa0a4+ 1e29  77(ma byt) versus  3a

3a - draw minimap

37-22304e
29-223051
36-2451b9

29e851 call void /*__spoils<ecx>*/ sub_BD542(unsigned __int16 a1, unsigned __int16 a2, unsigned __int16 a3, unsigned __int16 a4, char a5)//29e542
20cd54

*(Bit16s*)&unk_1804B0ar[ 8e] - proc je nula

&array_E2A74[30 *  5c + 4]

int sub_88450()//269450

-----------------------------
krok 15f
unk_1804B0ar+ 0 versus 3514b0+0
7d versus 64
26a88d call int sub_89830(x_WORD *a1)//26a830
268721 call void sub_87610()//268610

35153e

155 155
11f 11f
157 157
133 133
127 127
12c 12c
12d
157
133
128
157
136
133 133
0   127
    144
    157
    133
    145
    155
    11f
    157
    133
    12c
0
 7D, 00, 76, 00, 78, 00, 0E, 00, 28, 00, 7D, 00, DC, 00, 90, 01,
1
 08, 00, 0E, 00, 28, 00, 58, 02, 50, 00, 04, 01, 64, 00, 34, 00,
2
 64, 00, 79, 00, 24, 00, 7B, 00, 80, 00, 12, 00, 00, 00, 00, 00,
3
 B0, 3D, 77, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
4
 00, 90, 01, 08, 00, 0E, 00, 1E, 00, 62, 02, 1E, 00, 72, 01, 00,
5
 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
6
 00, 00, 00, 00, 00, 00, 66, 00, 00, 00, 00, 00, 00, 00, 14, 00,
7
 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 32, 00, 00, 00, 00, 00,
8
 00, 00, 00, 00, 00, 00, 51, 00, 00, 00, 5F, 01, 00, 00, 57, 01,
9                                                                       e   f
 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 08, 03,
 01, 00, 01, 02, 00, FF, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,

int sub_89830(x_WORD *a1)//26a830
[ecx],si 7d

3514cc 64
268709 - void sub_87610()//268610

int sub_89520(signed __int16 *a1)//26a520
int sub_89830(x_WORD *a1)//26a830

unk_1804B0ar+ 1c
void sub_87610()//268610

void sub_884D0(int a1, __int16 a2, Bit8u* a3, char a4, char a5)//2694d0
int sub_871F0()//2681f0

void sub_2CE30_pause_end_level(int a1, int a2)//20de30
void sub_51BB0_game_events(Bit8u* a1)//232bb0
x_DWORD_E9C4C_langindexbuffer

char sub_1A970_change_game_settings(char a1, int a2, int a3)//1fb970

--
unk_1804B0ar+ 1e

Bit8u* sub_87B70()//268b70

64003400 - ok
30007a00 - ok
30007800 - ok
021c0020 - 

&array_E2A74[ ad8]

fix
x_BYTE_180664
char x_BYTE_18067C; // weak
char x_BYTE_18068E; // weak
char x_BYTE_180693; // weak
char x_BYTE_180696; // weak
char x_BYTE_18069A; // weak
char x_BYTE_18069C; // weak

x_DWORD_180730_cursor_data
82c714
270aa0 call void sub_8F935_bitmap_draw_final(Bit8u a1byte1, Bit8u a1byte2, Bit16u tiley, int tilex, Bit8u* texture, Bit8u setbyte, char a6)//270935
2708d7 call void sub_8F8B0_draw_bitmap320(Bit16s posx, Bit16s posy, posistruct temppstr)//2708B0
26de71 call void sub_8CD27_set_cursor(posistruct a2)//26dd27
1f9c2c call void sub_18BB0()//1f9bb0

ca5d45-d1ae42 13 15
fc5135
e4002b

17df70
xy_DWORD_17DED4_spritestr[39]

x_DWORD_EB394 - find - 2bc394

Bit8u* v1102; // [esp+0h] [ebp-88h] - fix

adress 1f9c12

void sub_693F0(Bit8u* a1)//24a3f0
(__int16 *)&*xadataspellsdat.var28_begin_buffer[80 * *(char *)(a1 + 64) + 2 + 26 * *(char *)(a1 + 70)],

2482e0

filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct[x_BYTE_D419E]

x_D41A0_BYTEARRAY_0[8589]->28
versus
3581c5->1f

1fc123 call char sub_1A970_change_game_settings(char a1, int a2, int a3)//1fb970

x_WORD_180660_VGA_type_resolution
versus
351660


x_BYTE_EB3A8
versus
2bc3a8

compare - *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] ==1
x_DWORD_EA3E4[*(unsigned __int16 *)(2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + x_D41A0_BYTEARRAY_0 + 11240)]

void sub_752C0()//2562c0

x_BYTE_180664
versus


Bit32u sub_64E20(Bit8u* a1)//245e20

x_WORD_1805C2 nesmi byt 7

x_BYTE_E36D1 - musi byt 1

Bit32u dwordindex_188;//x_D41A0_BYTEARRAY_4_struct.byteindex_188
	Bit8u byteindex_189;//x_D41A0_BYTEARRAY_4_struct.byteindex_189
	Bit8u byteindex_190;//x_D41A0_BYTEARRAY_4_struct.byteindex_190

	Bit8u byteindex_192;// c0//x_D41A0_BYTEARRAY_4_struct.byteindex_192
	Bit8u byteindex_193;// c1//x_D41A0_BYTEARRAY_4_struct.byteindex_193
	Bit8u byteindex_194;// c2//x_D41A0_BYTEARRAY_4_struct.byteindex_194

x_D41A0_BYTEARRAY_4_struct.byteindex_8618 by melo byt 1

char sub_1A970_change_game_settings(char a1, int a2, int a3)//1fb970

2124 * 1 + x_D41A0_BYTEARRAY_0 + 11240

356044
versus
*(Bit16s*)&x_D41A0_BYTEARRAY_0[12]

x_DWORD_17DB54_game_turn2
versus
34eb54

247b30

mezi
case  246f60: {
case  2482e0: {

if (v114[5] &  40)
				*(x_BYTE *)(*(x_DWORD *)(v113 + 164) + 1112) = *(x_BYTE *)(v114 + 1);
			if (v114[5] < 0 && !v18[2118])
				v18[2118] = 1;
			goto LABEL_215;

x_WORD_EB398ar - position and rotation


x_BYTE_EB39E_keys[0]
x_WORD_1805C0
x_D41A0_BYTEARRAY_0[10 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 28227]

*(x_WORD *)(a1 + 28)
------------------
void sub_69640(Bit8u* a1)//24a640

--------------------------
x_DWORD_180628b_screen_buffer+ 2168( 00)
versus
 3aa0a4+ 2168( 86)

x_DWORD_180628b_screen_buffer+ 6ef9( 00)
versus
 3aa0a4+ 6ef9( 86)


cyklus3
x_DWORD_180628b_screen_buffer+ f99a( 1f)
versus
 3aa0a4+ f99a( f7)

&x_DWORD_E9C38_smalltit[ 1f40]
x_WORD_15B4E0[ da4f]

void sub_51BB0_game_events(Bit8u* a1)//232bb0


void sub_191B0(__int16 a1, char a2)//1fa1b0 //set spell


if (v114[5] &  40)
	*(x_BYTE *)(*(x_DWORD *)(v113 + 164) + 1112) = *(x_BYTE *)(v114 + 1);
if (v114[5] < 0 && !v18[2118])
	v18[2118] = 1;
goto LABEL_215;

LABEL_215:
		v99 = *(x_DWORD *)(v113 + 164);
		v100 = 2 * *(char *)(v114 + 3) - *(signed __int16 *)(v99 + 341);
		*(x_WORD *)(v99 + 4) = (signed int)(v100 - (__CFSHL__(HIDWORD(v100), 2) + 4 * HIDWORD(v100))) >> 2;
		v101 = *(x_DWORD *)(v113 + 164);
		v102 = 2 * *(char *)(v114 + 4) - *(signed __int16 *)(v101 + 343);
		LODWORD(v102) = (signed int)(v102 - (__CFSHL__(HIDWORD(v102), 2) + 4 * HIDWORD(v102))) >> 2;
		HIDWORD(v102) = (int)v114;
		*(x_WORD *)(v101 + 6) = v102;
		**(x_DWORD **)(v113 + 164) = *(unsigned __int8 *)(HIDWORD(v102) + 5);
		*(x_WORD *)(*(x_DWORD *)(v113 + 164) + 24) = *(x_WORD *)(v114 + 6);
		*(x_WORD *)(*(x_DWORD *)(v113 + 164) + 26) = *(x_WORD *)(v114 + 8);
		v103 = x_DWORD_EA3E4[*(unsigned __int16 *)(v18 + 10)];
		sub_57B20(v18, v103);
		if (*(x_BYTE *)(v18 + 2118))
			sub_55C60(a1, (int)v103, v18);
		a1 = v114;
		v18 += 2124;
		v104 = v116 + 1;
		memset((void*)v114, 0, 10);
		v116 = v104;
		v114 = (Bit8u*)a1 + 10;

v18 = x_D41A0_BYTEARRAY_0 + 11230; - projektily?

x_DWORD_EA3E4[ 25a1]

--------------------------
x_DWORD_180628b_screen_buffer+ 2168( 00)
versus
 3aa0a4+ 2168( 86)

Bit8u x_BYTE_F6EE0_tablesx[ 14600];// (Bit8u*)&x_BYTE_F6EE0_tablesbuff;
//2cbee0- 4000= 2c7ee0
Bit8u x_BYTE_10B4E0[ 10000]; // idb// x_BYTE_10B1E0[ 300] //map array1
//2DC4E0
Bit8u x_BYTE_11B4E0[ 10000]; // idb						//map array2
//2EC4E0
Bit8u x_BYTE_12B4E0[ 10000]; // fix it -  weak				//map array3
//2FC4E0
Bit8u x_BYTE_13B4E0[ 10000]; // idb						//map array4
//30C4E0
__int16 x_WORD_15B4E0[ 20000]; // idb						//map array5
//32C4E0


2230a1 - void sub_41BD3(unsigned int a1)//222bd3
v122 = (int)(x_DWORD_F2C20ar[ 03] * x_DWORD_DE560 + x_DWORD_F2C20ar[ 04] + x_DWORD_DE558);
x_DWORD_F2C20ar[ 03] - Y
x_DWORD_F2C20ar[ 04] - X

x_DWORD_F2C20ar


x_DWORD_EA3E4[ ce]- 1c, 20, 4c, 50
versus
3655f6- 1c, 20, 4c, 50
 00 00 00 00 0A 00 00 00 09 00 00 00 06 00 00 00
 C8 00 00 00 D3 13 00 00 00 00 98 00 BC 07 E2 07
 BC 07 E2 07 00 00 9A 00 00 00 64 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 FA 00 00 00 00 CF 09
 01 0A FF 0A 0C 01 00 00 00 00 00 00 30 4E 0B DD
 28 14 4B 00 B4 00 B4 00 BB 00 D1 00 00 01 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

 0x1c-bc07
 246fbf - void sub_65F60(Bit8u* a1)//246f60

 &x_D41A0_BYTEARRAY_0[0xf60a+4] versus 365642+4
 je 30cd
 ma byt 0414


 x_DWORD_E9C28
 versus
 2bac28

 252319 call void sub_712F0()//2522f0

 2530c0 void sub_720C0(int *a1)//2530c0
 void sub_713A0()//2523a0

 x_BYTE_F3FA0 - pallette

 castle procedures:
 22e900
 247b30
 22ba40
 240a70

 fire proceudres:
 22e2e0
 246b30

 22fb50
 2133e0
 213160

 fire tree
 22f5f0
 212760
 245f60

 posses procedures
 22e3b0
 246f60

 22f8c0
 2130e0

 splash procedures
 22f570
 2128b0


 find this:
 void sub_5FA70(Bit8u* a1)//240a70


 player position:
*(Bit16s*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 14 * *(Bit16u*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 11244) + 11709),//position of player
*(Bit16s*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 14 * *(Bit16u*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 11244) + 11711),//position of player
*(Bit16s*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 14 * *(Bit16u*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 11244) + 11715),//rotation of player z 
*(Bit16s*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 14 * *(Bit16u*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 11244) + 11717),//rotation of player
*(Bit16s*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 14 * *(Bit16u*)(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 11244) + 11719),//rotation of player



*(Bit32u*)(&x_DWORD_D4C52ar[0x1856] + 6)
0x2a5c52+0x1856+6
misto 0x2130e0 tam dat 240a70


-------------------
x_DWORD_180628b_screen_buffer+ 0x21df
versus
3aa0a4+ 0x21df  b2(ma byt) versus  83

3a-minimap
63-minmap
83-kriz

83
21d3db - 00
2451b9 - 42 25 3b 3a
2436c2 - 83
--
21d3db - 00
2451b9 - 42 25 3b 3a
2436c2 - 83
---
21d3db - 00
223051 - 3e 3e 3e 3e
2451b9 - 62 25 3b 39
2436c2 - 83
---
21d3db - 00
2451b9 - 42 25 3b 3a
2436c2 - 83
---
21d3db - 00
2451b9 - 42 25 3b 3a
24342b - a4
2436c2 - ff
----
21d3db - 00
223051 - 3a 3a 3a 3a -ok
2451b9 - 63 26 39 38 -ok
242cdc - b2 - sem se to nedostane!
---

compare
x_BYTE_F6EE0_tablesx
versus
0x2c7ee0

fix end //end 17B4E0

test  sub_61A00_draw_minimap_entites_b asi 7 pruchod

-------------------
x_DWORD_180628b_screen_buffer+ 0x1415
versus
3aa0a4+ 0x1415  39(ma byt) versus  b0

00
63 63 a2 a2 - particles
39 37  minimap
17 -minimap ent
b0 -minimap ent

------------------------------------------pruchod 9
x_D41A0_BYTEARRAY_0+0x1154e(0x12)-debugcounter11==9
versus
 356038+0x1154e(0x11)

 -void sub_37BC0(Bit8u* a1)//218bc0 - 13
 -void sub_37BC0(Bit8u* a1)//218bc0 - 12

 218be5 - 13
 218bf9 - 12
 367586 - 12

 218bfc - 11

&x_D41A0_BYTEARRAY_0[0x1153e]

&x_D41A0_BYTEARRAY_0[0x1153e+0x45] - 0x2c-debugcounter11==9
versus  356038+0x1153e+0x45//11583

Bit8u* sub_50370(Bit8u* a1)//231370
void sub_60480(Bit8u* a1)//241480
void sub_5F8F0(Bit8u* a1)//2408f0
void sub_5FA70(Bit8u* a1)//240a70

&x_D41A0_BYTEARRAY_0[0x11496+0x45]-predtim 0xc-debugcounter11==8
versus  356038+0x11496+0x45//114DB

-return (int)sub_4E8C0(a1);//posses spell drop1
-pod sub_65F60(a1);//fly possess projectile

void sub_65F60(Bit8u* a1)//246f60 - test this


367586 - 13
218bfe - 12 call void sub_37BC0(Bit8u* a1)//218bc0

13
12
13
12

------------------------------------------pruchod a
x_DWORD_180628b_screen_buffer+ 0x1415
versus
3aa0a4+ 0x1415  39(ma byt) versus  b0
------------------------------------------pruchod a
x_D41A0_BYTEARRAY_0+0x7922(0xad)-debugcounter11==a
versus
356038+0x7922(0x3a)

void sub_385C0(Bit8u* a1)//2195c0

------------------------------------------pruchod a
x_DWORD_180628b_screen_buffer+ 0x1a5a
versus
3aa0a4+ 0x1a5a  9e(ma byt) versus  3b

7e 7e - particles
7e 7e 43 43 -minimap
3b 63 - minimap
3b 63 a2 a2- minimap entites

--------
22304e - 3d 3d 24 24
2451b9 - 63 3a 31 36
--------
223039 - 7e 7e 43 43
2451b9 - 9e 3b 3c a2

x_BYTE_10B4E0 - 2dc4e0
x_BYTE_12B4E0 - 2fc4e0
---------------------------------------pruchod 8
x_BYTE_10B4E0+1(0x01)
versus
0x2dc4e0+1(0x24)
x_BYTE_12B4E0(0x1e)
versus
0x2fc4e0(0x20)

char sub_46570(unsigned __int16 a1, __int16 a2)//227570
void sub_37BC0(Bit8u* a1)//218bc0

2276b8 -2401

---------------
sub_46570
0100-ok
2500-ok
2501-ok
0101-ok
2401-ok
2425-ok
242501-ne 012500
240101
242401
242425
24242501
24240101
24242401
24242425
24242401
2424242401
01242424
27242424
01242424
01012424
01272424
01012424
01010124
01012724
01010124
01010101
01010127
01010101

x_BYTE_11B4E0 - rozdil zde

x_WORD_15B4E0 - zde je to ok

void sub_37BC0(Bit8u* a1)//218bc0

---------------------------------------pruchod 8
x_BYTE_12B4E0+0xfe(0x1f)
versus
0x2fc4e0+0xfe(0x20)

2277bf - call char sub_46570(unsigned __int16 a1, __int16 a2)//227570
eax=ffff
alah=0000
218f79 - call void sub_37BC0(Bit8u* a1)//218bc0

---------------------------------------pruchod 8
x_BYTE_13B4E0+0x2fe(0x68)
versus
0x30c4e0+0x2fe(0x60)

2277f2-toto je az pote

---------------------------------pruchod 9
x_BYTE_10B4E0(0x1)
versus
0x2dc4e0(0x0)
char sub_46570(unsigned __int16 a1, __int16 a2)//227570
0101 =1
0124 =v14

0124
----
2275cd - 0101
2276b8 - 0124
----
0124

---------------------------------pruchod b
x_BYTE_13B4E0+0x2(0xc0)
versus
0x30c4e0+0x2(0x40)

void sub_45DC0(Bit8u a1, Bit8u a2, unsigned __int16 a3, unsigned __int8 a4)//226dc0
void sub_37BC0(Bit8u* a1)//218bc0 -  - problem uz zde
40 18 48 38
-----------
40 10 48 38
40 10 40 38
c0 10 40 38
c0 90 40 38

40 18 48 38
-----------

--------------------- pruchod b
x_BYTE_13B4E0+0x2(0xc0)
versus
30c4e0+0x2(0x40)

--------------------- pruchod 7
x_D41A0_BYTEARRAY_0 + 0x1158e(cc)
versus
0x356038 + 0x1158e (00)

--------------------- pruchod 7
x_D41A0_BYTEARRAY_0 + 0x115dc(40)
versus
0x356038 + 0x115dc (00)

213520
--------------------- pruchod 18
x_D41A0_BYTEARRAY_0 + 0x115f6(ff)
versus
0x356038 + 0x115f6 (1)

void sub_37BC0(Bit8u* a1)//218bc0
01
void sub_37BC0(Bit8u* a1)//218bc0
=-1

02
------
01 218bfc

--------------------- pruchod 1a
x_D41A0_BYTEARRAY_0 + 0x1156c(4)
versus
0x356038 + 0x1156c (2)

238f10
21922b call void sub_37BC0(Bit8u* a1)//218bc0

(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 12244)-position y
(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 12593)-position x
(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 12149)-rotate 1
(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 12568)-rotate 2
(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 12573)-rotate 3
(x_D41A0_BYTEARRAY_0 + 2124 * *(Bit16s*)&x_D41A0_BYTEARRAY_0[12] + 12593)-rotate 4

void sub_17A00(Bit8u* a1, signed int a2, __int16 a3)//1f8a00

--------------------- pruchod 1
x_DWORD_180628b_screen_buffer + 0x2167(00)
versus
0x3aa0a4 + 0x2167 (86)

2230a1 - call void sub_41BD3(unsigned int a1)//222bd3
void sub_3E360_draw_particles(Bit8u* a1, Bit8u* a2)//21f360

find it
void sub_18F80(Bit8u* a1)//1f9f80

void sub_17060(__int16 a1, __int16 a2)//1f8060
relativní x a y
x_D41A0_BYTEARRAY_0[10 * D41A0_BYTESTR_0.word_0xc + 28225] = v3;
x_D41A0_BYTEARRAY_0[10 * D41A0_BYTESTR_0.word_0xc + 28226] = v4;

--------------------- pruchod 1
x_DWORD_180628b_screen_buffer + 0xe051(38)
versus
0x3aa0a4 + 0xe051 (3a)

--------------------- pruchod 8
x_DWORD_180628b_screen_buffer + 0x5306(a2)
versus
0x3aa0a4 + 0x5306 (3d)

--------------------
3d - particles(sub_41BD3)
3d 00 36 3c - particles(sub_41BD3)
a2 00 36 3c - particles(sub_41BD3)
a2 3d 36 3c - particles(sub_41BD3)
a2 3d 36 64 - particles(sub_41BD3)

38 00 00 00
38 38 00 00
38 38 3a 00

orig:
38 38 3a 3b
299ab7 - sub_B6253_draw_quad
3a 38 3a 3b
223bd1 - particles
remake:
38 3a 3a 3b - 88
particles -222bd3- sub_41BD3

x_BYTE_F2CC6(01)
versus
2C3CC6(01)

x_DWORD_F2C20ar[0x01] - typ rotace

x_DWORD_F2C20ar[0x01] = v45;

(Bit16s*)(&x_DWORD_E9C38_smalltit[45920])
versus
[2bac38]+b360=400604

remake
91

orig
91 - 223dba

orig
222bd3
84 38 00 39
2a 00 39 2a

90 21 00 90
21 00 90 21

x_D41A0_BYTEARRAY_0[0x2f75+4] - rotation of player

--------------------- pruchod 0
x_DWORD_180628b_screen_buffer + 0xdf11(39)
versus
0x3aa0a4 + 0xdf11 (3b)

39 - quad
39 39 00 00 -quad
39 39 39 00 - quad

39 3b 39 39 - particles

--------------------- pruchod 8
x_DWORD_180628b_screen_buffer + 0x508e(a2)
versus
0x3aa0a4 + 0x508e (3d)

3d
a2 - particles

x_DWORD_F2C20ar[0x10/4] = 8a

x_DWORD_F2C20ar[0x27] by melo byt f5ff
versus
2c3cbc

21cec8 call x_DWORD *sub_3B560(__int16 a1)//21c560

v69 = *(Bit32s*)&x_DWORD_D9F50ar[v1*4];
//2aaf50
*(Bit32s*)&x_DWORD_D9F50ar[v1*4];
v70 = *(Bit32s*)&x_DWORD_DA750ar[v1*4];
//2ab750

&x_DWORD_D9F50ar[1*4]
versus
2aaf50+4
252426

x_WORD_D951C

void sub_71410_process_tmaps()//252410


*(Bit16s*)&x_BYTE_D951C[

test all x_BYTE_D951C
fix:
				v39 = (char*)&((*xadataspellsdat.var28_begin_buffer)[80 * v44 + 2 + 26 * v10]);
				if (x_BYTE_D951C[0x12fd+80 * *(char *)(v9 + 64)] & 4)

x_BYTE_D951C+0x12fd(00)
versus
0x2aa51c+0x12fd(10)

2818ac

1e1000 - read
2798c4 - void sub_9874D_create_index_dattab(Bit8u* tabbuffer, Bit8u* tabbufferend, Bit8u* datbuffer, posistruct* dattabindex)//27974d
234e94 - int sub_53E60_readfile_and_decompress(const char* path, Bit8u** a2)//234E60
23ca55 - void sub_5B8D0_initialize()//23c8d0

--------------------- pruchod 8
x_DWORD_180628b_screen_buffer + 0x5807(7e)
versus
0x3aa0a4 + 0x5807 (4)
22304e

0020
7700


0x00,0x20,0x00,0x00,0x02,0x00,0x00,0x00,0x40,0xEE,0x48,0x00,0x3C,0x00,0x00,0x00,
10
0x83,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x35,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
20
0x40,0x00,0x00,0x00,0x27,0x00,0x00,0x00,0xF8,0xFF,0x34,0x00,0xFC,0xFF,0x3F,0x00,
30
0x21,0x00,0x00,0x00,0xB6,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
40
0x64,0x00,0x00,0x00,0xF5,0xFF,0x00,0x00,0x00,0x00,0x88,0x00,0x00,0x00,0xE1,0x00,
50
0x5E,0x51,0x36,0x00,0x00,0x00,0x90,0x01,0x00,0x00,0x69,0x01,0x00,0x00,0x00,0x00,
60                  64
0xA4,0x00,0x00,0x00,0xC3,0x00,0x00,0x00,0x94,0x3A,0x40,0x00,0xB6,0x04,0x00,0x00,
ok                  02
0x08,0x02,0x00,0x00,0x05,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0xBA,0x04,0x00,0x00,
0x20,0x14,0x00,0x00,0xFB,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x40,0x01,0x00,0x00,
0xA0,0x00,0x00,0x00,0xC8,0x00,0x00,0x00,0x40,0x01,0x00,0x00,0xF5,0xFF,0x00,0x00,

&x_DWORD_F2C20ar[0x19]
versus
2c3c20+0x64

c3 - 21d01c - x_DWORD *sub_3B560(__int16 a1)//21c560

*(Bit32s*)&x_BYTE_D951C[0xa34+0x7fa*4] - 9a96
versus 2acf38 - f5ff


x_DWORD_E9C38_smalltit

unk_D4350

(Bit32s*)x_DWORD_DDF50[0x1a]+8
(Bit32s*)x_DWORD_DDF50x[0x1a]+8

BLOCK32DAT_BEGIN_BUFFER

*(Bit32s*)x_DWORD_DDF50x[0x7b]
alias ((Bit8u*)x_DWORD_DDF50x)+0x1ec

*(Bit32s*)(((Bit8u*)x_DWORD_DDF50x)+0x1ec)

x_DWORD_DB350_ret

void sub_2ECC0_draw_bottom_menu()//20fcc0

if(debugcounter==2)
LOBYTE(unk_18058Cstr.x_DWORD_18059C) = unk_18058Cstr.x_DWORD_18059C | 0x10;

26bc24 call int sub_89D10_analyze_keys()//26ad10


draw spell:
sub_2BB40_draw_bitmap(a1, a2, filearray_2aa18c[filearrayindex_MSPRD00DATTAB].posistruct[(*(char *)(a3 + 64) + 123)]);

x_WORD_D93C0_bldgprmbuffer[0x13f+v54]
versus
2aa4ff

char x_BYTE_D94FF[29]

01020304050607..

int sub_89920(x_WORD *a1, __int16 a2, Bit8u* a3)//26a920
draw spell text

find in this
void sub_87610()//268610


x_DWORD_F2C20ar
2000
0
1c000 !!
fffff6d

x_DWORD_F2C20ar[0x25] nesmi byt cccccccc

test 
v123

206d50

not true exit
x_WORD_E29D8


&array_E2A74[0x9f6+0xa](9001)
versus
2b446a+0xa(2601)


26a021 char sub_88D40()//269d40

array_E2A74[30*0x45]
06 00 03 00 4b 01 32 00 21 00 00 00 00 00 00 00
8b 00 00 00 00 00 00 00 50 00 00 00 00 05 06 00
versus

2b3a74+30*0x45=2b428a
06 00 03 00 4b 01 32 00 21 00 43 01 00 00 00 00
6c 00 00 00 00 00 00 00 50 00 00 00 00 01 06 00

*(Bit32s*)&x_D41A0_BYTEARRAY_0[0x341c]
-selected bottom spell

sub_692A0(x_DWORD_EA3E4[0xc4e0/168]);
+64
+69

castle1
filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct[1].data

castle1
filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct[2].data

&(*a1[i].dat_buffer+0x100)

filearray_2aa18c[filearrayindex_BUILD00DATTAB][2].dat_buffer

*(&filearray_2aa18c[filearrayindex_BUILD00DATTAB])[0].dat_buffer

void sub_49EC0(Bit16u* a1, __int16 a2)//22aec0 // get castle data
-test

create castle
unsigned int sub_60810(Bit8u* a1)//241810



void sub_37240(Bit8u* a1)//218240

begin of heightmap:
3065056 - 2ec4e0 - x_BYTE_11B4E0

x_BYTE_11B4E0+3287
0x02D3DB00-0x348

groove castle void sub_37BC0(Bit8u* a1)//218bc0

quad error
x_DWORD_DDF50[0x19]
x_DWORD_DDF50[0x19]


23ac80

1fc6b0
void sub_loc_1B2E6() {//1fc2e6


int sub_58F00_game_objectives()//239f00
23a4f6

38c5d4:
000100 0201010101 00 - point 1

000200 0202010101 00 - get jar

000300 0202020101 00 - kill soldiers

000400 0202020201 00 - svetlusky

239006
299be7 - b8be7

sub_A5040(x_DWORD *a1)//286040


								v18 = *(Bit32u*)&x_D41A0_BYTEARRAY_4_struct.byteindex_242ar[4];
								if (v18 > 0)
								{
									v7 = (int)x_D41A0_BYTEARRAY_0;
									if ((unsigned int)(100
										* (*(x_DWORD *)(v6 + 316)
											+ *(x_DWORD *)(x_DWORD_EA3E4[*(unsigned __int16 *)(v6 + 58)] + 144))
										/ v18) >= *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 10 * v3 + 222546))
									{
										v0[v3 + 3] = 2;
										v4 = 1;
										*(x_WORD *)(0x2FED5 + v7) = 0;
									}
								}



compare
int sub_58F00_game_objectives()//239f00

358ffc +4 +6 
versus
v6

358ffc 9c
versus
*(Bit32s*)(v6 + 316)//0x13c


sub_56A30_init_game_level


x_D41A0_BYTEARRAY_0 + 28222
versus
35ce76

1f8136 - call void sub_17060_compute_mouse_relative_pos(__int16 a1, __int16 a2)//1f8060

01
1f8136

233ce1 - void sub_51BB0_game_events(/*Bit8u* a1*/)//232bb0

rozdil je uz v 35ce76 - 01(06) a 05(20) - kde se to meni?

27e1cc 00->01
1f8136 00000000 - 00000044
1f8176 00000000 - 000000447f
27e201 0000447f -> 00000000
1fa279 00000000 -> 06000000 - mouse 20  take mouse
1fa279 void sub_191B0(__int16 a1, char a2)//1fa1b0 //set spell
1fa0a5 void sub_18F80(Bit8u* a1)//1f9f80
1f8dd8 void sub_17A00_mouse_and_keys_events(Bit8u* a1, signed int a2, __int16 a3)//1f8a00


asi zde:
		if (x_D41A0_BYTEARRAY_0[10 * D41A0_BYTESTR_0.word_0xc + 28222] != a1 && x_D41A0_BYTEARRAY_0[10 * D41A0_BYTESTR_0.word_0xc + 28222])
			return;
		//v7 = (int)x_D41A0_BYTEARRAY_0;
		x_D41A0_BYTEARRAY_0[10 * D41A0_BYTESTR_0.word_0xc + 28222] = a1;


1fa290 void sub_191B0(__int16 a1, char a2)//1fa1b0 //set spell

ecx ebp-18 000000000000000
eax ebp-14 0100000000000000000000000000001
ecx [ecx+a4] 0000000000000000000000
eax [eax+3] 0
edx [ecx+155] 0
add
sub
mov
sar
shl
sbb
sar
mov [ecx+4],ax 0
ecx ebp-18 000000000000000
eax ebp-14 0100000000000000000000000000001
ecx [ecx+a4] 0000000000000000000000
eax [eax+4] 0
edx [ecx+157] 0
add
sub
mov
sar
shl
sbb
sar
edx ebp-14 0100000000000000000000000000001
[ecx+6],ax 0
xor eax

-------------
ecx ebp-18 c6ce3500 1027 0000 1027
eax ebp-14 0000000000000000000000000000001
ecx [ecx+a4] 0000000000000000000000
eax [eax+3] 0
edx [ecx+155] 0
add
sub
mov
sar
shl
sbb
sar
mov [ecx+4],ax 0
ecx ebp-18 000000000000000
eax ebp-14 0100000000000000000000000000001
ecx [ecx+a4] 0000000000000000000000
eax [eax+4] 0
edx [ecx+157] 0
add
sub
mov
sar
shl
sbb
sar
edx ebp-14 0100000000000000000000000000001
[ecx+6],ax 0
xor eax
---------------
ecx ebp-18 c6ce3500 1027 0000 1027
eax ebp-14 0600000000200000000000000000001
ecx [ecx+a4] 0000000000000000000000
eax [eax+3] 0
edx [ecx+155] 0
add
sub
mov
sar
shl
sbb
sar
mov [ecx+4],ax 0
ecx ebp-18 000000000000000
eax ebp-14 0100000000000000000000000000001
ecx [ecx+a4] 0000000000000000000000
eax [eax+4] 0
edx [ecx+157] 0
add
sub
mov
sar
shl
sbb
sar
edx ebp-14 0100000000000000000000000000001
[ecx+6],ax 0
xor eax


3
edx=3*10+bytear0
eax=ebp-48 30c
[ebp-40]=edx 
edx,[ebp-38] 0
add edx,eax = 30c

239fe8
eax=ebp-44 - 363286
eax=[eax+a4] - 358ffc
eax=[eax+0x13c] 30c
eax=[[[ebp+var_44]+0A4h]+13c]

359138 - 0232 - possed buildings

23a02d - int sub_58F00_game_objectives()//239f00

241fe4  - call void sub_60F00()//241f00

22d6b0 - 123 creating
22bd70 - 1
22d6b0 - 45
206d50 - 123 creating mana?
246040 - 1
206d50 - 45
-----
206d50 - 678
246040 - 2
206d50 - 9abcd
246040 - 3
206d50 - ef10
246040 - 4
206d00
206d20 - 12
246040 - 5
----
206d20 - 34567
---- zlute
246040 - 6
206d20 - 89ab
246040 - 7
206d20 - cd
246040 - 8
206d20 - e
246040 - 9a
232610 - 1
232570 - 1
231c10 - 1
232610 - 2345
22c6f0 - 1
231c90 - 1
x23ac80 - 1
246040 - b
23ac40 - 1
2502b0 - 1
x23ac80 - 234
200970 - 1
1fc6b0 - 123456789abcdef10
250480 - 1
x23ac80 - 5
246040 - b
23ac40 - 2

a1 + 4 - max life 
a1 + 8 - act life
a1 + 16 - other pointer
a1 + 20 - random
a1 + 28 - a1+20 - rotation
a1 + 30 - a1+20
a1 + 32 - a1+20
a1 + 34 - 0
a1 + 42 - 300
a1 + 56 - 1
a1 + 63 - exam.5 - type of entity
a1 + 64 - exam.19 - subtype of entity
a1 + 65 - 3
a1 + 66 - 0
a1 + 69 - exam. 0x99
a1 + 76 - position
a1 + 130 = a1 + 132 // speed
a1 + 132 - exam. 76 // speed
a1 + 134 - exam. 8
a1 + 144 - max life >>1
a1 + 160 - unk_D7BD6 pointer

if (*(x_BYTE *)(*(x_DWORD *)(a1 + 160) + 32) & 1 || sub_104D0((signed __int16 *)(a1 + 76)) == 1)
here firefly died

a1 + 80 not set

a1 + 60
362026 +0x50

fix sub_57FA0

fix unk_D7BD6

36226e

last procedure 23f8c0

if (v4 == 8 && v15 != *(Bit32s*)(x_D41A0_BYTEARRAY_0 + 4))
v15? 0 =! 0xe1b7

v15=ebp-8

problem in
x_DWORD_DE558
x_DWORD_E9C3C - max 0xfa00

x_DWORD_DE560 je 0x140

if (v123 - x_DWORD_DE558<0)
	v123 = v123;

mouseturnoff = true;

missing adress 0x24c3a0

void sub_8E160_sound_proc15_startsequence(__int16 a1, unsigned __int8 a2)//26f160

*(Bit16u*)(x_D41A0_BYTEARRAY_0 + 565) musi byt 1
[2a51a0]+235

356038 +235

*(Bit16u*)(x_D41A0_BYTEARRAY_0 + 565)
D41A0_BYTESTR_0.dword_0x235
versus
35626d
kde se meni

227932

find change unk_17E078