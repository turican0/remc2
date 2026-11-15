#include "EventsFunctions.h"
#include "Basic.h"
#include "Castle.h"
#include "Effects.h"
#include "Sound.h"
#include "Spells.h"
#include "Terrain.h"
#include "TextureMaps.h"
#include "Vr.h"

#include "CommandLineParser.h"
#include "GameUI.h"
#include "Graphics.h"
#include "MenusAndIntros.h"
#include "Network.h"
#include "Player.h"
#include "PlayerInput.h"
//#include "engine_support.h"
//#include "CompareMaps.h"
#include "Events.h"
#include "Level.h"
#include "../utilities/StateMonitor.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "DatTabIndexes.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"


#if defined(__linux__) || defined(__APPLE__)
#include <strings.h>
#include <cstdlib>
#include <cstring>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define strnicmp strncasecmp
#define __cdecl
#include <ctype.h>
#include <iostream>
#include <functional>
#include <type_traits>
#else
#endif //__linux__


#include <filesystem>
//first multi is 50(51) 10
//first hide level is 30(31) 5

//adress 2285ff

int save_debugcounter = 0;

//(set in bool sub_558E0_InGameLoad(uint8_t fileindex)//2368e0)
int count_begin = 1;//1
//int stage__4A190_0x6E8E = 0;
//int debugnextlevel = 0;

bool config_EDITOR = false;

/*
fix sub_3C080_draw_terrain_and_particles_old
MoveEntity_57FA0(&x_WORD_EB398ar, v4, 0, v3);
(char*)&world_D41A0+0x13de2
addprocedurestop(0x238cf3, 0x348, true, true, 0x356038 + 0x13de2, 0x12345678);

2a000000 - ok
d6ff0000


test void FreeTmapCacheEntry_71F20(type_x_DWORD_E9C28_str* a1y, subtype_x_DWORD_E9C28_str* a2x)//252f20
arrays for fix:
fix memcpy,memset, malloc
add regressions tests
find changes Entities_EA3E4[48]->word_0x1A_26

fix x_WORD_F4FE0 lenght 6 - ok
fix sub_4A050_new_event
fix CreateLinkedLevelFeatures_49090
fix x_WORD_D93C0_bldgprmbuffer

x_DWORD_F66F0x[504]; ok, rewrited to Sprite// idb//2c76f0 //Sprite
int tmapGroupLastUse_F5730[504]; must rewrite later!!!
int x_DWORD_F5F10[504]; not used? // idb ?
x_BYTE_D4750 ok
unk_D47E0//terrain gen
unk_D4A30//terrain gen
xx211d50ar//removed
unk_D7BD6 - ok, rewrited to str_D7BD6 (I must test FixPointersAfterLoad_57680 and fix in future)
spriteKindFrames_D8A2E - ok this is array
x_WORD_D93C0_bldgprmbuffer
x_WORD_D951C - ok, rewrited to str_WORD_D951C, xxx->word_8 mayby must (unsigned __int64)
dataBlock_D9F50 - ? used only byte 0x87A,0x5b6,0x126 (error?)
balloonChainTable_DB038 - ok
x_BYTE_DB080 - ok, rewrited to str_BYTE_DB080
x_BYTE_DB4EE - ok
x_BYTE_DB507 - ok
objectiveTextIndexes_DB520 - ok
array_E1328 - ok, remowed at now, but maybe must rewrite in future
byte_E16E0 - ok
x_BYTE_E1711 - ok, rewrited to playerColors_E1711
unk_E1748x - ok
off_E1BAC - rewrited multiplayerButtons_E1D64 mainMenuButtons_E1BAC_0x3c4//buttons pos - must fix mainMenuButtons_E1BAC_0x3c4
x_WORD_E1F70 //ok
x_WORD_E1F84 - ok, rewrited to type_menuButtons_E1F84
x_WORD_E2008 - ok, rewrited to regionButtons_E2008 //type_menuButtons_E1F84
x_WORD_E20A4 - ok, rewrites to dragonAnims_E20A4
off_E23E0 - ok, rewrited to mapMenuButtons_E23E0
unk_E24BCx - ok, rewrited to textBoxStr_E24BCx
x_WORD_E24BE ok ? only for clock? remove it?
unk_E24F2 - ok, rewrited to textBoxStr_E24F2
unk_E2516 - ok, rewrited to textBoxStr_E2516 and textBoxStr_E2570
unk_E25DC - ok, rewrited to unk_E25DC
x_BYTE_E25ED_0x - ok, rewrited to goldMenuItems_E25ED
x_BYTE_E25ED_a3 - ok, rewrited to str_BYTE_E25ED_a3
x_BYTE_E25ED_2BB - ok, rewrited to keysDialogItems_E28A8
keyToButton_E36DC - not needed - sound
ar_D404C - ok, rewrited to str_D404C
unk_D4350 - ok
sub_7BF20_draw_scroll_dialog - ok
x_DWORD_E36C4 - rewrited to externControl_E36C4
x_DWORD_E12AE - rewrited to str_DWORD_E12AE
x_DWORD_E12AA - rewrited to mainConnection_E12AA
TMAPS00TAB_BEGIN_BUFFER rewrited to str_TMAPS00TAB_BEGIN_BUFFER
xadatapald0dat2.colorPalette_var28
x_DWORD_17ECA0


x_BYTE_17DE68x - rewrited to array_BYTE_17DE68x
colorPalette_var28 - ok, not now only renamed to var28_Palette

fix cast spell

fix sub_7E8D0
fix FixPointersAfterLoad_57680
fix LoadEntitySprites_716C0 - fixed
regression:
level1 - can not build castle (fix wall detections)

must revide sub_loc_1B54A and etc - some events must be fixed
must revide sub_loc_1B37D((type_E17CC_0*)a1_6E8E);//FIX ME
must revide PlayInfoFmv(0, 1, str_E16E0[v3x].dword_0 - 0x2b2328 + (uint8_t*)array_E1328, cutScenePath);//FIXME
must revide if (mapMenuButtons_E23E0[v6y].byte_23 && mapMenuButtons_E23E0[v6y].byte_22 == textBoxStr_E2516[mapHelpText_17DBA8.unk_17DBB4+1].byte_17)// x_BYTE_E2527[18 * *((signed __int16 *)a2 + 6)] )
must revide sub_7E9D0(&dragonAnims_E20A4[v7y].word_18, &dragonAnims_E20A4[v7y].word_20, dragonAnims_E20A4[v7y].array_word_45);
must revide qmemcpy(netPlayerColors_E1B9C, &menuState_17DE38.x_BYTE_17DE68x[11 * sub_74515() + 1], sizeof(netPlayerColors_E1B9C));
fix this (uint8_t*)&mapScreenPortals_E17CC[ix]
fix DrawGameDebugText_6FEC0 v45

//bug hunting - find non click problem
sub_7BF20_draw_scroll_dialog(signed __int16 *a1) - diference in a1
problem with set mapMenuButtons_E23E0[v3x].word_26
&mapMenuButtons_E23E0[v3x].word_26
(uint8_t*)&off_E23E0+26

//bug hunting 2 find difference 0x356038+0xd49a
find in:
void sub_49CD0(type_str_0x6E8E* a1x, __int16 a2)//22acd0
add_compare(0x237B55, CommandLineParams.DoDebugafterload());
add_compare(0x237BB0, CommandLineParams.DoDebugafterload());
add_compare(0x237bb9, CommandLineParams.DoDebugafterload());
add_compare(0x237BC7, CommandLineParams.DoDebugafterload());

problem is in void PlayerEvents_51BB0()//232bb0
//find changes after 228320

//bug hunting 3 find difference 0x356038+57 D41A0_BYTESTR_0.array_0x39
changed in MarkLoadedTmaps_71930 ReloadLevelTmaps_71990
difference in x_BYTE_F5340

(uint8_t*)str_WORD_D951C+18*7
3f versus 3a
fixed

//bug hunting 4 find difference 0x356038+79 D41A0_BYTESTR_0.array_0x39[22]

next debugging:
add_compare(0x0022860F, CommandLineParams.DoDebugafterload() && (count_begin == 1), 0x1a);

//bug hunting 5 can not load save from this revision
gameState_D41A4.byteindex_208 is set to 0

*/

//debug_first_run

//#define debug_hide_graphics

/*
bool localDrive::TestDir(const char * dir) {sub_4ADF0sub_28690
if (nocachedir) EmptyCache();
char newdir[CROSS_LEN];
strcpy(newdir,basedir);
strcpy(newdir,basedir);
strcat(newdir,dir);
CROSS_FILENAME(newdir);
dirCache.ExpandName(newdir);
dirCache.ExpandName(newdir);

// guest to host code page translation
host_cnv_char_t *host_name = CodePageGuestToHost(newdir);
if (host_name == NULL) {
LOG_MSG("%s: Filename '%s' from guest is non-representable on the host filesystem through code page conversion",__FUNCTION__,newdir);
return false;
}

// Skip directory test, if "\"
size_t len = strlen(newdir);
if (len && (newdir[len-1]!='\\')) {
// It has to be a directory !
ht_stat_t test;
if (ht_stat(host_name,&test))		return false;
if ((test.st_mode & S_IFDIR)==0)	return false;
};
int temp=ht_access(host_name,F_OK);
return (temp==0);
}

uint8_t DOS_FindDevice(char const * name) {
// should only check for the names before the dot and spacepadded
char fullname[DOS_PATHLENGTH];uint8_t drive;
//	if(!name || !(*name)) return DOS_DEVICES; //important, but makename does it
if (!DOS_MakeName(name, fullname, &drive)) return DOS_DEVICES;

char* name_part = strrchr(fullname, '\\');
if (name_part) {
	*name_part++ = 0;
	//Check validity of leading directory.
	if (!Drives[drive]->TestDir(fullname)) return DOS_DEVICES;
}
else name_part = fullname;

char* dot = strrchr(name_part, '.');
if (dot) *dot = 0; //no ext checking

static char com[5] = { 'C','O','M','1',0 };
static char lpt[5] = { 'L','P','T','1',0 };
// AUX is alias for COM1 and PRN for LPT1
// A bit of a hack. (but less then before).
// no need for casecmp as makename returns uppercase
if (strcmp(name_part, "AUX") == 0) name_part = com;
if (strcmp(name_part, "PRN") == 0) name_part = lpt;

// loop through devices
for (uint8_t index = 0;index < DOS_DEVICES;index++) {
	if (Devices[index]) {
		if (WildFileCmp(name_part, Devices[index]->name)) return index;
	}
}
return DOS_DEVICES;
}

bool DOS_OpenFile(char const * name,uint8_t flags,uint16_t * entry) {
#if defined(WIN32) && !defined(__MINGW32__)
if(Network_IsNetworkResource(const_cast<char *>(name)))
return Network_OpenFile(const_cast<char *>(name),flags,entry);
#endif
// First check for devices
if (flags>2) LOG(LOG_FILES, LOG_NORMAL)("Special file open command %X file %s", flags, name); // FIXME: Why? Is there something about special opens DOSBox doesn't handle properly?
else LOG(LOG_FILES, LOG_NORMAL)("file open command %X file %s", flags, name);

DOS_PSP psp(dos.psp());
uint16_t attr = 0;
uint8_t devnum = DOS_FindDevice(name);
bool device = (devnum != DOS_DEVICES);
if (!device && DOS_GetFileAttr(name, &attr)) {
	//DON'T ALLOW directories to be openened.(skip test if file is device).
	if ((attr & DOS_ATTR_DIRECTORY) || (attr & DOS_ATTR_VOLUME)) {
		DOS_SetError(DOSERR_ACCESS_DENIED);
		return false;
	}
}

char fullname[DOS_PATHLENGTH];uint8_t drive;uint8_t i;
// First check if the name is correct
if (!DOS_MakeName(name, fullname, &drive)) return false;
uint8_t handle = 255;
// Check for a free file handle
for (i = 0;i<DOS_FILES;i++) {
	if (!Files[i]) {
		handle = i;
		break;
	}
}
if (handle == 255) {
	DOS_SetError(DOSERR_TOO_MANY_OPEN_FILES);
	return false;
}
// We have a position in the main table now find one in the psp table
*entry = psp.FindFreeFileEntry();

if (*entry == 0xff) {
	DOS_SetError(DOSERR_TOO_MANY_OPEN_FILES);
	return false;
}
bool exists = false;
if (device) {
	Files[handle] = new DOS_Device(*Devices[devnum]);
}
else {
	exists = Drives[drive]->FileOpen(&Files[handle], fullname, flags);
	if (exists) Files[handle]->SetDrive(drive);
}
if (exists || device) {
	Files[handle]->AddRef();
	psp.SetFileHandle(*entry, handle);
	Files[handle]->drive = drive;
	return true;
}
else {
	//Test if file exists, but opened in read-write mode (and writeprotected)
	if (((flags & 3) != OPEN_READ) && Drives[drive]->FileExists(fullname))
		DOS_SetError(DOSERR_ACCESS_DENIED);
	else {
		if (!PathExists(name)) DOS_SetError(DOSERR_PATH_NOT_FOUND);
		else DOS_SetError(DOSERR_FILE_NOT_FOUND);
	}
	return false;
}
}

bool localDrive::FileOpen(DOS_File * * file,const char * name,uint32_t flags) {
if (nocachedir) EmptyCache();

if (readonly) {
if ((flags&0xf) == OPEN_WRITE || (flags&0xf) == OPEN_READWRITE) {
DOS_SetError(DOSERR_WRITE_PROTECTED);
return false;
}
}

const host_cnv_char_t * type;
switch (flags&0xf) {
case OPEN_READ:        type = _HT("rb");  break;
case OPEN_WRITE:       type = _HT("rb+"); break;
case OPEN_READWRITE:   type = _HT("rb+"); break;
case OPEN_READ_NO_MOD: type = _HT("rb");  break; //No modification of dates. LORD4.07 uses this
default:
DOS_SetError(DOSERR_ACCESS_CODE_INVALID);
return false;
}
char newname[CROSS_LEN];
strcpy(newname,basedir);
strcat(newname,name);
CROSS_FILENAME(newname);
dirCache.ExpandName(newname);

//Flush the buffer of handles for the same file. (Betrayal in Antara)
uint8_t i,drive=DOS_DRIVES;
localFile *lfp;
for (i=0;i<DOS_DRIVES;i++) {
if (Drives[i]==this) {
drive=i;
break;
}
}
for (i=0;i<DOS_FILES;i++) {
if (Files[i] && Files[i]->IsOpen() && Files[i]->GetDrive()==drive && Files[i]->IsName(name)) {
lfp=dynamic_cast<localFile*>(Files[i]);
if (lfp) lfp->Flush();
}
}

// guest to host code page translation
host_cnv_char_t *host_name = CodePageGuestToHost(newname);
if (host_name == NULL) {
LOG_MSG("%s: Filename '%s' from guest is non-representable on the host filesystem through code page conversion",__FUNCTION__,newname);
DOS_SetError(DOSERR_FILE_NOT_FOUND);
return false;
}

#ifdef host_cnv_use_wchar
FILE * hand=_wfopen(host_name,type);
#else
FILE * hand=fopen(host_name,type);
#endif
//	uint32_t err=errno;
if (!hand) {
if((flags&0xf) != OPEN_READ) {
#ifdef host_cnv_use_wchar
FILE * hmm=_wfopen(host_name,L"rb");
#else
FILE * hmm=fopen(host_name,"rb");
#endif
if (hmm) {
fclose(hmm);
#ifdef host_cnv_use_wchar
LOG_MSG("Warning: file %ls exists and failed to open in write mode.\nPlease Remove write-protection",host_name);
#else
LOG_MSG("Warning: file %s exists and failed to open in write mode.\nPlease Remove write-protection",host_name);
#endif
}
}
return false;
}

*file=new localFile(name,hand);
(*file)->flags=flags;  //for the inheritance flag and maybe check for others.
//	(*file)->SetFileName(host_name);
return true;
}
*/


//bitmap_pos_struct_t var_2BB3E0_x_DWORD_EA3D4_14[0x3e9];

static unsigned long int    _RWD_randnext = 1;

static unsigned long* watcominitrandnext(void)
{
	//_INITRANDNEXT(RETURN_ARG(unsigned long*, NULL));
	return((unsigned long*)&_RWD_randnext);
}

int watcomrand(void)
{
	unsigned long* randptr;

	randptr = watcominitrandnext();
	if (randptr == NULL) {
		return(0);
	}
	*randptr = *randptr * 1103515245 + 12345;
	return((int)((*randptr >> 16) & 0x7FFF));
}

void watcomsrand(unsigned int seed)
{
	unsigned long* randptr;

	randptr = watcominitrandnext();
	if (randptr != NULL) {
		*randptr = seed;
	}
}

int nullsub_3(x_DWORD  /*number*/) { stub_fix_it(); return 0; };
int _no_support_loaded(x_DWORD  /*number1*/, x_DWORD  /*number2*/) { stub_fix_it(); return 0; };

//-------------------------------------------------------------------------

char driverType_E4CB2 = 0; // weak
// Function declarations -weak
//int /*__noreturn*/ sub_10000() { stub_fix_it();return 0; }; // weak
void CopyDriverInfoIfType9_9A1B6(int  /*a1*/, void* a2, void* a3)
{
	if (driverType_E4CB2 == 9)
	{
		//__asm { int     21h; DOS - GET DISK TRANSFER AREA ADDRESS }
		qmemcpy(a2, a3, 0x2Bu);
	}
}

int _wcpp_1_unwind_leave__120(/*x_DWORD a, */x_DWORD  /*b*/, x_DWORD  /*c*/) { /*stub_fix_it();*/return 0; };// weak // fix this
void JUMPOUT(int*  /*adr*/) {
	stub_fix_it(); exit(0);
};//error must fix this
void JUMPOUT(x_WORD  /*cs*/, int*  /*adr*/) {
	stub_fix_it(); exit(0);
};//error must fix this
void JUMPOUT(x_WORD  /*cs*/, int  /*a*/, int*  /*adr*/) {
	stub_fix_it(); exit(0);
};//error must fix this
/*void qmemcpy(void* a, void* b, size_t c) {
	memcpy(a, b, c);
};*/
int _wcpp_1_unwind_leave__62(void) { /*stub_fix_it();*/return 0; }; //weak
//void __outx_WORD(unsigned short Port, unsigned short Data) { stub_fix_it(); };
//void __outx_BYTE(unsigned short Port, unsigned char Data) { stub_fix_it(); };
//unsigned char __inx_BYTE(unsigned short Port) { stub_fix_it(); exit(0); return 0; };//error must fix this
/*char x_toupper(char inputchar) {
	return toupper(inputchar);
}; //weak*/

void gotoxy(int x, int y) //positions text cursor at (x, y) screen position
{
	VGA_GotoXY(x, y);
	/*CONSOLE_SCREEN_BUFFER_INFO csbiInfo; //variable declarations
	HANDLE hConsoleOut;
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsolepdwScreenBufferInfo(hConsoleOut,&csbiInfo);
	csbiInfo.dwCursorPosition.X = x; //cursor position X coordinate
	csbiInfo.dwCursorPosition.Y = y; //cursor position Y coordinate
	SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition); //set cursor at the given screen coordinate */
}
/*-------------------------wherex()------------------------------------*/
int wherex() //returns current text cursor (x) coordinate
{
	return VGA_WhereX();
	/*CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsolepdwScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	return csbiInfo.dwCursorPosition.X;*/
}
/*-------------------------wherey()----------------------------------*/
int wherey() //returns current text cursor (y) coordinate
{
	return VGA_WhereY();
	/*CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsolepdwScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	return csbiInfo.dwCursorPosition.Y;*/
}

x_DWORD settextposition(x_DWORD x, x_DWORD y) {
	gotoxy(x, y);
	Logger->debug("\t");
	//printf("\033[%d;%dH%s\n", x, y, "R");
	/*#ifndef USE_DOSBOX
		COORD coord;
		coord.X = x;
		coord.Y = y;

		// Obtain a handle to the console screen buffer.
		// (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
		// in conjunction with the GetStdHandle() to retrieve the handle.)
		// Note that because it is a standard handle, we don't need to close it.
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Finally, call the SetConsoleCursorPosition function.
		if (!SetConsoleCursorPosition(hConsole, coord))
		{
			// Uh-oh! The function call failed, so you need to handle the error.
			// You can call GetLastError() to get a more specific error code.
			// ...
			return 1;
		}
	#endif*/
	return 0;
};// weak
void outtext(const char* text)
{
	myWriteOut(text);
};// weak

POSITION gettextposition(/*x_DWORD, x_DWORD, x_DWORD*/) {
	return VGA_WhereXY();
};// weak
x_DWORD signal(x_DWORD, x_DWORD) { stub_fix_it(); return 0; };// weak
//x_DWORD unknown_libname_1_atoi(x_DWORD) { stub_fix_it();return 0; };// weak
/*int __cdecl unknown_libname_1_atoi(char *a1)
{
	char *i; // eax
	char v2; // cl
	int v3; // ebx
	int v4; // ebx

	for (i = a1; IsTable[(unsigned __int8)(*i + 1)] & 2; ++i)
		;
	v2 = *i;
	if (*i == '+' || v2 == '-')
		++i;
	v3 = 0;
	while (IsTable[(unsigned __int8)(*i + 1)] & 0x20)
	{
		v4 = (unsigned __int8)*i++ + 10 * v3;
		v3 = v4 - 48;
	}
	if (v2 == '-')
		v3 = -v3;
	return v3;
}*/
//void _disable() { stub_fix_it(); };
//void _enable() { stub_fix_it(); };
/*void(*dos_key_vect_9)();
void(*dos_getvect(x_DWORD number))() {
	if (number == 9)
		return dos_key_vect_9;
	return 0; };// weak
x_DWORD dos_setvect(x_DWORD number, void(*actcall)(), x_DWORD) {
	if(number==9)
		dos_key_vect_9= actcall;
	return 0; };// weak*/

	/*x_DWORD segread(SREGS* a1) {
		int result; // eax
		a1->cs = __CS__;
		//a1[1] = __CS__;
		a1->ds = __DS__;
		//a1[3] = __DS__;
		a1->es = __ES__;
		// *a1 = __ES__;
		a1->ss = __SS__;
		//a1[2] = __SS__;
		a1->fs = __FS__;
		//a1[4] = __FS__;
		result = __GS__;
		a1->gs = __GS__;
		//a1[5] = __GS__;
		return result;
	}; //weak*/

	//uint8_t** pointersdat_buffer; // ebx

	//x_DWORD int386x(x_DWORD, x_DWORD, x_DWORD, x_DWORD) { stub_fix_it();return 0; };// weak
	/*
	uint32 sub_AB59E(SREGS* a1, REGS* a2, int  )
	{
		uint32 v2; // bp
		uint32 result; // eax
		uint32 v4; // ebx
		uint32 v5; // ecx
		uint32 v6; // edx
		uint32 v7; // esi
		uint32 v8; // edi

		// fix it:__ES__ = *a1;
		v2 = a1->gs;
		result = a2->eax;
		v4 = a2->ebx;
		v5 = a2->ecx;
		v6 = a2->edx;
		v7 = a2->edi;
		v8 = a2->eax;
		//esi je typ 21
		return result;
	}
	*/
	/*int _int386x(REGS* a1, SREGS* a2, REGS* a3, int a4) {
		REGS* v4; // ST00_4
		REGS* v5; // edi
		uint32 v6; // ecx
		uint32 v7; // edx
		unsigned __int8 v8; // cf
		int result; // eax
		SREGS v10; // [esp-8h] [ebp-Ch]

		//fix it
		v6 = 0;
		v7 = 0;
		v8 = 0;

		//fix it

		v4 = a3;
		v5 = a1;
		a1->eax = sub_AB59E(a2, a3, a4);
		v5->ebx = a2->cs;
		v5->ecx = v6;
		v5->edx = v7;
		v5->esi = a4;
		v5->edi = v4->eax;
		v5->cflag = v8;
		result = (unsigned __int16)__DS__;
		v10.ds = __DS__;
		v10.es = __ES__;
		return result;
	};
	int int386x(int a2, REGS* a3, REGS* a4, struct SREGS* a5) {
		_int386x(a4, a5, a3, a2);
		return *(x_DWORD *)a4;
	};*/
	//unsigned     int x__readeflags(void) { stub_fix_it(); return 0; };
	//void x__writeeflags(unsigned Value) { stub_fix_it(); };
x_DWORD gets(x_DWORD) { stub_fix_it(); return 0; };// weak
//x_DWORD tell(x_DWORD) { stub_fix_it();return 0; };// weak
void* x_expand(void* memblock, size_t size) {
	return realloc(memblock, size * sizeof(uint8_t));
};
int /*__fastcall*/ _wcpp_1_unwind_leave__132(x_DWORD) { /*stub_fix_it();*/ return 0; };// weak
double /*__fastcall*/ _CHP(x_DWORD) { stub_fix_it(); return 0; };// weak
double IF_DPOW(double, double) { stub_fix_it(); return 0; };// weak
x_DWORD x_dos_setdrive(int32_t  /*a*/, int32_t* b) {
	*b = 0x1a;
	return 0;
};// weak
/*x_DWORD x_outp(x_DWORD, char) { stub_fix_it(); return 0; };// weak
x_DWORD x_inp(x_DWORD) { stub_fix_it(); return 0; };// weak*/

x_DWORD x_setmode(FILE*  /*path*/, int  /*mode*/) { stub_fix_it(); return 0; };// weak
unsigned __CFRCR__(__int16  /*a*/, unsigned __int8  /*b*/) { stub_fix_it(); return 0; };

//x_DWORD dos_read(FILE*, char, x_DWORD, x_DWORD, x_DWORD) { stub_fix_it(); return 0; };// weak
void __inx_BYTEstring(unsigned short  /*Port*/, unsigned char*  /*Buffer*/, unsigned long  /*Count*/) { stub_fix_it(); };
//unsigned int x__getcallerseflags(void) { stub_fix_it(); return 0; };
void x__sidt(void*  /*Destination*/) { stub_fix_it(); };
unsigned long x__readcr0(void) { stub_fix_it(); return 0; };
void x__writecr0(unsigned __int32  /*Data*/) { stub_fix_it(); };

int _nmemneed(uint8_t*) { stub_fix_it(); return 0; };// weak
int _nmemneed_0(uint8_t*) { stub_fix_it(); return 0; };// weak
int /*__fastcall*/ nullsub_1(x_DWORD) { stub_fix_it(); return 0; }; // weak
//int sub_365F8() { stub_fix_it();return 0; }; // weak 222712

int dword_1820E0 = 0;
void j___delay(x_DWORD x) { mydelay(x); }; // weak
int j_j___clock(x_DWORD, x_DWORD, char*) { stub_fix_it(); return 0; }; // weak

// Function declarations
// char sub_3C080_draw_terrain_and_particles(int a1, int a2, __int16 a3, __int16 a4, __int16 a5, signed int a6, int a7, __int16 a8, int a9);
// unsigned __int16 sub_3E360_draw_particles(int a1, int a2);
// unsigned __int16 DrawTileSpriteReflections_3FD60(int a1, int a2);
// int sub_43830_generate_level_map(unsigned int a1, int a2);
// unsigned int SmoothTerrain_43970(unsigned int a1);
// unsigned int GetSmoothedHeight_439A0(unsigned int a1, unsigned __int16 a2);
/*void GenerateCaveCeiling_43B40();
void RandomiseCeilingHeight_43BB0();
void RandomiseTerrainArea_43C60(unsigned __int8 a1, char a2, int a3, int a4);
void MarkWaterEdges_43D50();
void AddRivers_43EE0();
void RemoveIsolatedTerrainClasses_43FC0();
void ApplySnowLine_440D0(unsigned __int16 a1);
void SpreadTerrainClass4_44320();
void SetTerrainAngles_44580();
void ComputeTerrainShading_44D00();*/
//void TruncTerrainHeight_44DB0();
//void MarkLandAndRivers_44E40(int a1, unsigned __int8 a2);
// unsigned __int16 SmoothTiles_44EE0(int a1, int a2);
/*void MarkFlatLowland_45060(unsigned __int8 a1, unsigned __int8 a2);
void MarkFlatLowlandClass2_45210(unsigned __int8 a1, unsigned __int8 a2);
void MarkFlatHighland_454F0(unsigned __int8 a1, unsigned __int8 a2);
void MarkSteepTerrain_45600(unsigned __int8 a1);*/
//void SetMax4Tiles_45AA0();
// char GetTileHeightType_45BE0(char a1, char a2, unsigned __int16 a3);
// char SetTileTerrainType_45DC0(char a1, char a2, unsigned __int16 a3, unsigned __int8 a4);
// int MainLoop_46830(signed __int16 *a1, signed int a2, unsigned __int16 a3);
// int InGameLoop_47320(signed int a1);
//void sub_473B0();
//int HasSkipInput_473E0();
// void sub_47560_draw_and_events_in_game(int a1, int a2, x_BYTE *a3, signed int a4, __int16 a5);
// __int16 DrawTerrainLine_48A20(int a1, char a2, char a3, int a4, int a5, unsigned __int8 a6);
//void sub_49290(type_str_2FECE* a1, char a2);
//void sub_49830(type_str_2FECE* a1);
//void ResetEntityLists_49F30();
// unsigned int ApplyPlayerOptions_55C60(int a1, int a2, int a3);
//int sub_main(int argc, const char **argv, const char **envp);
// char LevelInitGame_56A30(unsigned int a1);
// char LoadLevel_56D60(unsigned int a1, char a2);
//char sub_5B8D0_initialize();
//void SetDefaultKeys_5BCC0();
//uint8_t sub_5BE80_test_Palette(uint8_t* a1, uint8_t a2, uint8_t a3, uint8_t a4);
//signed int LoadPsxData_5BF50();
//signed int SetDefaultSettings_5C1B0();
//void DrawTextRaw_6F940(char* a1, int a2, int a3, uint8_t color);
//uint8_t GetLetterWidth_6FC10();
//uint8_t GetLetterHeight_6FC30();
//void sub_6FC50(__int16 a1);
//unsigned int DrawTextCentred_6FC80(char* a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5);
//int sub_71410_process_tmaps_process_tmaps();
// void /*__spoils<ecx>*/ ClearGraphicsBuffer320(int a1, void *a2, unsigned __int16 a3, char a4);
// void /*__spoils<ecx>*/ ClearGraphicsBuffer640(int a1, void *a2, unsigned __int16 a3, char a4);
//void DrawBitmapWithColour640_72C40(__int16 a1, __int16 a2, bitmap_pos_struct_t a3, unsigned __int8 a4);
//int sub_72CB0(unsigned __int8* a1, int a2);//not used
//void VgaBlit640_75200(uint16_t height);
//int sub_75420();
//void sub_75AE0(int a1);
//void sub_75B50(__int16 a1);
//void WaitForNextAnimationFrame_75CB0();
//void sub_75D70(void *a1, unsigned int a2);
//void sub_75D70(uint8_t* a1, uint32_t a2);
//void sub_75DB0();
//void /*__fastcall*/ sub_75E70();
//void PlayIntoFmv(__int16 a1, __int16 a2, uint8_t* a3, char* path);
//void DecodeFlcDeltaChunk_76300();
//int DecodeFliLineCompressChunk_76430();
//int DecodeFliByteRunChunk_76540();
//char GetMouseAndKeyboardEvents_7A060();
//void DrawBitmap640_7C120(int16_t posx, int16_t posy, bitmap_pos_struct_t tempstr);
//void DrawTextBackground_7C140(int16_t a1, int16_t a2, int16_t a3, int16_t a4, uint8_t a5);
//int32_t sub_A7C20_AIL_API_init_sequence(HSEQUENCE hSequence, void* start, int32_t sequence_num, uint32_t track);
// signed int sub_7E5A0_pre_draw(int a1, int a2, __int16 a3, __int16 a4, int a5, __int16 a6, __int16 a7);
//int sub_7F7D0(uint8_t** a1, uint8_t** a2, uint8_t* a3, char* a4);
//uint32_t DrawText_7FAE0(char* a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int8 a5);
//void DrawText_7FB90(char* a1, int16_t a2, int16_t a3, uint8_t a4);
// int DrawTextWithBoarder_7FCB0(int a1, x_BYTE *a2, int a3, int a4, int a5, char a6, unsigned __int8 a7, __int16 a8);
// int sub_81260(int a1, int a2, int a3, __int16 a4, __int16 a5);
// unsigned int sub_81CA0(int a1, int a2, __int16 a3, __int16 a4, int a5);
//char sub_83E00(int a1, int a2);
//void sub_86550();
// int /*__fastcall*/ _wcpp_1_unwind_leave__132(x_DWORD); weak

// char WaitForJoystickResponse_8B980(int a1, int a2, x_DWORD **a3, int a4);
// char CalibrateJoystick_8BA10(int a1, int a2, int *a3, char *a4, int a5);
// x_DWORD dos_getdrive(x_DWORD); weak
// x_DWORD dos_setdrive(x_DWORD, x_DWORD); weak
// x_DWORD dos_getdiskfree(x_DWORD, x_DWORD); weak
// int sub_8C21F_any_graphics_command();
// x_DWORD memset(x_DWORD, x_DWORD, x_DWORD); weak
// int rand(void); weak
// x_DWORD printf(x_DWORD);
//void sub_8C2DE();
//void sub_8C635_draw_cursor();
//void DrawCursorIfEnabled_8CACD();
// int sub_8D12F_set_mouse_viewport();
//-------------
//void DrawBitmap320_8F8B0(int16_t posx, int16_t posy, bitmap_pos_struct_t temppstr);
//void DrawBitmap640_8F8E8(int16_t posx, int16_t posy, bitmap_pos_struct_t temppstr);
// void DrawBitmapFinalUnscaled_8F920(__int16 a1, signed int a2, int a3, char *a4, unsigned __int8 a5, char a6);
// void DrawBitmapFinal_8F935(__int16 a1, signed int a2, int a3, char *a4, unsigned __int8 a5, char a6);
//void sub_90164(int16_t a1, int16_t a2, int16_t a3, int16_t a4, uint8_t a5);
//void sub_901E4(int16_t a1, int16_t a2, int16_t a3, int16_t a4, unsigned __int16 a5);
//void VgaBlit320_90478();
//unsigned __int8 sub_90530(int a1, int a2, float a3);
//void sub_905EC_any_graphics_command2(char a1);
//int sub_90810();
// int sub_994BA_cursor_move(__int16 a1);
// bool sub_9B540_lock_linear_mem_region(unsigned int a1, unsigned int a2);
// bool sub_9B5B4_unlock_mem_region(unsigned int a1, unsigned int a2);
// void sub_9B628();
// signed int sub_9BC68_allocate_and_lock_memory(x_WORD* a1, uint8_t* a2, unsigned int a3);

uint8_t algn_4BB85[11] = { 0x8d, 0x80, 0x00, 0x00, 0x00, 0x00, 0x8d, 0x52, 0x00, 0x8b, 0x00 };

int x_DWORD_B184C[2] = { 0, 0 }; // weak
void* off_B1854 = &vgaBuffer_A0000; // weak
int x_DWORD_B1858[2] = { 70, 0 }; // weak
int x_DWORD_B3BEB[32] = {
0x7FFFFFFF,0x3FFFFFFF,0x1FFFFFFF,0x0FFFFFFF,
0x07FFFFFF,0x03FFFFFF,0x01FFFFFF,0x00FFFFFF,
0x007FFFFF,0x003FFFFF,0x001FFFFF,0x000FFFFF,
0x0007FFFF,0x0003FFFF,0x0001FFFF,0x0000FFFF,
0x00007FFF,0x00003FFF,0x00001FFF,0x00000FFF,
0x000007FF,0x000003FF,0x000001FF,0x000000FF,
0x0000007F,0x0000003F,0x0000001F,0x0000000F,
0x00000007,0x00000003,0x00000001,0x00000000 }; // weak
__int16 x_WORD_B3D9E = 6222; // weak
__int16 x_WORD_B419A = 18153; // weak
int x_DWORD_B41B8 = 0; // weak
__int16 x_WORD_B440E[7 * 5] = {
0x5359,0x856A,0x3B81,0xCA96,0x3FCE,
0xF88F,0x9DC0,0x9F39,0xD73F,0xBFD6,
0x4BE5,0x4368,0x309D,0xB092,0x3FDE,
0x1C7F,0xAA27,0x2B3F,0xD732,0xBFE5,
0x9C7D,0xB639,0x1D2A,0xB8EF,0x3FEC,
0xD00C,0x0D00,0x00D0,0xD00D,0xBFF2,
0x8888,0x8888,0x8888,0x8888,0x3FF8
}; // weak
int x_DWORD_B4468[19] =
{
  2646669455,
  3611270969,
  3909435346,
  2772829156,
  3218786763,
  3334782378,
  2406926207,
  3818340322,
  2109457658,
  3219756018,
  218157069,
  3490513104,
  3054190575,
  1622543200,
  3220551179,
  2863311530,
  2863311530,
  16378,
  0
}; // weak
__int16 x_WORD_B44C2[9] = { 0x00,0x01,0x02,0x03,0x04,0x04,0x05,0x5,0x6 }; // weak
void* off_D1F10 = (void*)0xD5221; // weak
char bitMasks_D3EA0[6] = { 1, 2, 4, 8, 16, 32 }; // idb

x_DWORD x_DWORD_D41A4_4 = 0;
x_DWORD x_DWORD_D41A4_6 = 127;
x_DWORD x_DWORD_D41A4_8 = 127;
x_BYTE x_BYTE_D41A4_10 = 1;
x_BYTE x_BYTE_D41A4_11 = 0;
x_BYTE x_BYTE_D41A4_12 = 0;
x_BYTE x_BYTE_D41A4_13 = 0;
//int x_DWORD_D4790 = 20; // weak
int x_DWORD_D4794 = 0; // weak
char gameInitialized_D4B80 = 0; // weak

type_D4C52ar2 x_DWORD_D4C52ar_strtest[1280];

char timerInstalled_DB734 = 0; // weak//x_DWORD_DAF50ar[0x7e4]
uint8_t netWizardTypes_E131C[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // idb

type_x_BYTE_E25ED_2BB str_BYTE_E25ED_a3[3] = {//set joy
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01B5,0x0000,0x0001 },
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01B6,0x0000,0x0002 },
{ 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000 }
};
char x_BYTE_E29EC = 0; // weak
char x_BYTE_E29ED = 0; // weak
char x_BYTE_E29EE = 0; // weak
char x_BYTE_E29EF = 0; // weak
char x_BYTE_E29F0 = 0; // weak
char x_BYTE_E29F1 = 0; // weak
char joystickEnabled_E36D1 = 0; // weak
int x_DWORD_E3B40 = 0; // weak
int x_DWORD_E3B44 = 0; // weak
__int16 x_WORD_E3B4A = 0; // weak
int x_DWORD_E3B94 = 0; // weak
__int16 x_WORD_E3B98 = 32; // weak
__int16 x_WORD_E3B9A = 0; // weak
__int16 x_WORD_E3BA6 = 0; // weak
__int16 x_WORD_E3BA8 = 0; // weak
__int16 x_WORD_E3BAA = 0; // weak
uint8_t* testersInfoPtr_E3BAC = 0; // weak
int testersInfoSize_E3BB0 = 0; // weak
int testersInfoFlags_E3BB8 = 1; // weak
FILE* file_E3BE0; // weak
int x_DWORD_E3DF8 = 1024; // weak
int x_DWORD_E3DFC[6] = { 0x00000000,0x00000000,0x00000000,0x00000000,
0x6E676973,0x0027CA00 }; // weak//2B4DFC
__int16 seg_E3E14[5] = { 0xCA2C0160,0x01600027,
0x0027CA58,0xCA840160,0x01600027
}; // weak
char x_BYTE_E3E28 = 0; // weak

char x_BYTE_E4CB4 = 0; // weak
int(/*__fastcall*/ *off_E4CBD)(x_DWORD) = &nullsub_3; // weak
int x_DWORD_E4CD0 = 0; // weak
int x_DWORD_E4D10 = 0; // weak
__int16 x_WORD_E4D14 = 0; // weak
__int16 x_WORD_E4D16 = 0; // weak
char x_BYTE_E4D18 = 0; // weak
int(*off_E4D20)(x_DWORD, x_DWORD) = &_no_support_loaded; // weak
//int x_DWORD_E4E1C = 0; // weak
int x_DWORD_E4E20 = 0; // weak
int x_DWORD_E4E24 = 0; // weak
int x_DWORD_E4E28 = 0; // weak
int x_DWORD_E4E2C = 0; // weak
__int64 qx_WORD_E4E30 = 0; // weak
int x_DWORD_E4E38 = 0; // weak
__int16 x_WORD_E4E3C = 0; // weak
//int32_t unk_E52EE; // weak
int x_DWORD_E731C = 16; // weak
//int x_DWORD_E7B24 = 210828714; // weak//atanTable_E7724[0x100+
__int16 x_WORD_E7C58 = 0; // weak
uint32_t unk_E7C64x = 0; // weak
double dbl_E7C6E = 1.7976931348623157e+308; // weak
float flt_E7C76 = 3.4028235e38; // weak

char* x_DWORD_17E0A0[768]; // fix it -  weak
int x_DWORD_17E0A4[768]; // fix it -  weak
int x_DWORD_17E0A8[768]; // idb

int x_DWORD_17ECA4; // weak
int x_DWORD_17ECA8; // weak
int x_DWORD_17ECAC; // weak
char x_BYTE_17ECB0[1]; // fix it -  weak
char x_BYTE_17ECB1[4590]; // idb
int x_DWORD_17FEA0; // weak
int x_DWORD_17FEB0; // weak
int x_DWORD_17FEC0; // weak
int x_DWORD_17FEC4; // weak
int x_DWORD_17FEC8; // weak
int x_DWORD_17FECC; // weak
__int16 x_WORD_17FF56; // weak
__int16 x_WORD_17FF58; // weak
__int16 x_WORD_17FF5A; // weak
uint8_t unk_1803A8x[16]; // weak
__int16 x_WORD_1803AB; // weak
__int16 x_WORD_1803C3; // weak
__int16 x_WORD_1803EA; // weak
__int16 x_WORD_180455; // weak
__int16 x_WORD_181C44; // weak


#pragma region Methods

int debugcounter_1f1130;

//----- (000101C0) --------------------------------------------------------
void LoadSearchData_101C0()//1f11c0
{
	char tempPalBuffer;
	char tempI;
	char tempJ;
	char searchPath[MAX_PATH];
	sprintf(searchPath, "%s/%s", cdDataPath.c_str(), "DATA/SEARCH.DAT\0");
	DataFileIO::ReadFileAndDecompress(searchPath, xawscreen_351628.colorPalette_var28);//2A1004,351628->3AA0A4
	for (uint16_t k = 1; k < 100; k++)
	{
		str_E7EE0x[k].bitmapIndex_8 = -1;
	}
	uint32_t l = 0;
	for (uint16_t k = 0; k < 32; k++)
	{
		bitmaps_E9980x[k].bitmapData_0 = (int8_t*)(*xasearchd_2bac30.colorPalette_var28 + l);
		bitmaps_E9980x[k].bitmapIndex_4 = 0;
		for (uint16_t j = 0; j < 32; j++)
		{
			for (uint16_t i = 0; i < 32; i++)//1F1212
			{
				tempPalBuffer = (*xawscreen_351628.colorPalette_var28)[i + 32 * j];
				if (k == tempPalBuffer)
				{
					if (!tempPalBuffer && (l == 0))
					{
						tempI = i;
						tempJ = j;
					}
					//adress 1f1267
					*(*xasearchd_2bac30.colorPalette_var28 + l) = (i - tempI);
					*(*xasearchd_2bac30.colorPalette_var28 + 1 + l) = (j - tempJ);
					*(*xasearchd_2bac30.colorPalette_var28 + 2 + l) = k;
					l += 4;
					bitmaps_E9980x[k].bitmapIndex_4++;
				}
			}
		}
	}
}

//----- (0003B4D0) --------------------------------------------------------
void FillD4350Table_3B4D0(int a1)//21c4d0
{
	//uint32_t* result; // eax
	int resultx;
	char v2; // bl
	int v3; // edx

	//result = (uint32_t*)unk_D4350;
	resultx = 0;
	v2 = 32;
	v3 = (a1 << 16) - 1;
	do
	{
		if (UVTable_D4350[resultx][0])
			UVTable_D4350[resultx][0] = v3;
		if (UVTable_D4350[resultx][1])
			UVTable_D4350[resultx][1] = v3;
		if (UVTable_D4350[resultx][2])
			UVTable_D4350[resultx][2] = v3;
		if (UVTable_D4350[resultx][3])
			UVTable_D4350[resultx][3] = v3;
		if (UVTable_D4350[resultx][4])
			UVTable_D4350[resultx][4] = v3;
		if (UVTable_D4350[resultx][5])
			UVTable_D4350[resultx][5] = v3;
		if (UVTable_D4350[resultx][6])
			UVTable_D4350[resultx][6] = v3;
		if (UVTable_D4350[resultx][7])
			UVTable_D4350[resultx][7] = v3;
		//result += 8;
		resultx++;
		v2--;
	} while (v2);
	//return result;
}

void write_pngs2()
{
	//uint8_t buffer[10000];
	if (std::string outdir = GetSubDirectoryPath("outimg"); myaccess(outdir.c_str(), 0) < 0)
	{
		std::string exepath = get_exe_path();
		mymkdir((exepath + "/" + "outimg").c_str());
	}

	for (int k = 0; k < 0x1F8u; k++)
	{
		if (m_ptrLoadedSprites_F66F0x[k] != 0)
		{
			Type_Sprite* actimg = *m_ptrLoadedSprites_F66F0x[k];
			//int lenght = actimg->word_0;
			int width = actimg->width;
			int height = actimg->height;
			/*int lenght = *(uint16_t*)(actimg + 0);
			int width = *(uint16_t*)(actimg + 2);
			int height = *(uint16_t*)(actimg + 4);*/

			//memcpy((uint8_t*)buffer, actimg + 6, lenght);
			if ((width > 0) && (width < 1024) && (width != 0xcdcd) && (height < 768))
			{
				char outname[MAX_PATH];
				sprintf(outname, "test-%03d.bmp", k);
				std::string path = GetSubDirectoryFilePath("outimg", outname);
				write_bitmap_pos_struct_to_png((uint8_t*)&actimg->textureBuffer, width, height, path.c_str());//test write
			}
		}
	}
	/*for (int k = 0; k < 0x1F8u; k++)
	{
		if ((uint32_t*)x_DWORD_F66F0[k] != 0)
		{
			uint8_t* actimg = (uint8_t*) * (uint32_t*)x_DWORD_F5F10[k];
			int lenght = *(uint16_t*)(actimg + 0);
			int width = *(uint16_t*)(actimg + 2);
			int height = *(uint16_t*)(actimg + 4);

			//memcpy((uint8_t*)buffer, actimg + 6, lenght);
			if ((width > 0) && (width < 1024) && (width != 0xcdcd) && (height < 768))
			{
				sprintf(outname, "c:\\prenos\\remc2\\outimg\\testB-%03d.png", k);
				write_bitmap_pos_struct_to_png(actimg + 6, width, height, outname);//test write
			}
		}
	}*/
}

/*
uint8_t testarraymain[168] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xd4,0xf5,0x00,0x00,0x00,0x00,0x11,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x65,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0xfa,0x00,0x01,0x00,0x00,0xd8,0x0f,
0x02,0xff,0xff,0x0a,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc2,0x80,0xd5,
0x00,0x00,0x4b,0x00,0x00,0x03,0x00,0x03,0x00,0x05,0x4d,0x00,0x00,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe8,0x03,0x00,0x00,
0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x5e,0x24,0x57,0x01,0x18,0x91,0xb1,0x02 };*/

int resindex_begin = 0;

//----- (00046830) --------------------------------------------------------
void MainLoop_46830(unsigned __int16 actLevel)//227830
{//graphics already inited
	bool isSecretLevel;
	bool skipMenus = false;
	int16_t setLevel = -1;
	std::string customLevelPath = "";

	if (CommandLineParams.DoDebugSequences()) {
		/*uint8_t origbyte20 = 0;
		uint8_t remakebyte20 = 0;
		int debugcounter11 = 0;
		int comp20 = compare_with_sequence_D41A0((char*)"00227830-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debugcounter11, 224790, &origbyte20, &remakebyte20);
		*/
	}

	gameState_D41A4.setting_30 = 0;//2a51a4
	world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234 = 0;

	setLevel = CommandLineParams.GetSetLevel();
	customLevelPath = CommandLineParams.GetCustomLevelPath();
	if ((setLevel > -1 || customLevelPath.length() > 0) || CommandLineParams.ModeRegressionsTestType() != -1)
	{
		if(CommandLineParams.ModeRegressionsTestType()==-1)
			gameState_D41A4.OptionsSettingFlag_24 |= LEVEL_LOADED_FROM_ARG;
		skipMenus = true;
	}

	if (CommandLineParams.DoStateMonitor()) {
		g_state_monitor.Init();
	}

	while (1)
	{
		g_state_monitor.Update();

		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234)
		{
			return;
		}
		ResetGameSettingsFlags_48350(); //fix it //229350
		//!!!!test area1
		//adress 22787a
		/*uint8_t origbyte3, remakebyte3;
		int comp12 = compare_with_snapshot_D41A0((char*)"0160-0022787A", x_D41A0_BYTEARRAY_0, 0x356038, 224790, &origbyte3, &remakebyte3);
		*/
		//!!!!test area1

		if (CommandLineParams.DoNetworkDebug()) {
			static int menuSeenFor = -1;
			if (menuSeenFor != g_autotest_match) {
				menuSeenFor = g_autotest_match;
				debug_net_printf("MATCHEND: entering MenusAndIntros for match %d (skipMenus=%d)\n",
					g_autotest_match, (int)skipMenus);
			}
		}
		MenusAndIntros_76930(skipMenus);//set language, intro, menu, atd. //257930

		//debug
		if (CommandLineParams.ModeRegressionsSaveIndex()>0)
		{
			//Load Saved Game File
			uint32_t numLevelsCompleted = 0;
			int locSavedGameIndex = CommandLineParams.ModeRegressionsSaveIndex();
			type_menuButtons_E1F84* buttonStr = &mapMenuButtons_E23E0[locSavedGameIndex];
			menuState_17DE38.savedGameIndex_17DF04 = locSavedGameIndex;
			char path[512];
			sprintf(path, "%s", unitTestsPath.c_str());
			std::string loadFilePath = GetSaveGameFile(path, menuState_17DE38.savedGameIndex_17DF04);
			FILE* FILE = DataFileIO::CreateOrOpenFile(loadFilePath.c_str(), 512);
			if (FILE != NULL)
			{
				uint32_t fileSing = 0;
				int unknownVar = 0;
				DataFileIO::Read(FILE, (uint8_t*)&fileSing, 4);
				if (fileSing == 0xFFFFFFF7u)
				{
					DataFileIO::Read(FILE, (uint8_t*)&menuState_17DE38.xx_BYTE_17DF14[(menuState_17DE38.savedGameIndex_17DF04 - 1)][0], 20);
					DataFileIO::Read(FILE, (uint8_t*)gameState_D41A4.player_name_57ar, 32);
					DataFileIO::Read(FILE, (uint8_t*)gameState_D41A4.savestring_89, 32);
					//Load completed Secret Portals
					for (int ii = 0; ii < 6; ii++)
					{
						DataFileIO::Read(FILE, readbuffer, 17);
						secretMapScreenPortals_E2970[ii].activated_12 = *(uint16_t*)(readbuffer + 12);
						if (secretMapScreenPortals_E2970[ii].activated_12 == 1)
							secretMapScreenPortals_E2970[ii].spriteIndex_14 = 305;
						else
							secretMapScreenPortals_E2970[ii].spriteIndex_14 = 270;
					}
					DataFileIO::Read(FILE, (uint8_t*)&world_D41A0.m_GameSettings, 16);
					DataFileIO::Read(FILE, (uint8_t*)&numLevelsCompleted, 4);
					DataFileIO::Read(FILE, (uint8_t*)&unknownVar, 4);
					DataFileIO::Read(FILE, (uint8_t*)&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611, 505);
					DataFileIO::Read(FILE, (uint8_t*)levelScores_17DBC8, 500);
					DataFileIO::Read(FILE, (uint8_t*)secretLevelScores_17DDBC, 100);
					DataFileIO::Close(FILE);
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.word[1] = 0;
					int i = 0;
					//Reset all Portals to inactive
					while (mapScreenPortals_E17CC[i].viewPortPosX_4)
					{
						mapScreenPortals_E17CC[i].activated_18 = 2;
						i++;
					}
					i = 0;
					//Load completed Portals
					while (i < numLevelsCompleted && mapScreenPortals_E17CC[i].viewPortPosX_4)
					{
						mapScreenPortals_E17CC[i].activated_18 = 1;
						i++;
					}
					i = 0;
					//Set current level number
					while (mapScreenPortals_E17CC[i].viewPortPosX_4)
					{
						if (mapScreenPortals_E17CC[i].activated_18 == 1)
							gameState_D41A4.levelnumber_43w = i;
						i++;
					}
					mapMenuState_17DB70.mapState_17DB8F = 1;
					memset(&levelDescription_17DE28, 0, 13);
					mapMenuState_17DB70.selectedLevel_17DB8A = -1;
					if (buttonStr->active_25)
					{
						MapMenuPortalsDraw_81760();
					}
					else
					{
						menuState_17DE38.savedGameIndex_17DF04 = -1;
						NewGameDialog_77350(buttonStr);
						buttonStr->state_4 = 2;
					}
				}
			}
		}
		//debug

		if (!world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234)
		{
			Logger->debug("MainLoop_46830:before load scr");
			isSecretLevel = gameState_D41A4.levelnumber_43w > 24 && gameState_D41A4.levelnumber_43w < 50;
			LoadScreen_47FC0(isSecretLevel);//vga smaltitle
			Logger->debug("MainLoop_46830:load scr passed");
			LevelInitGame_56A30(setLevel, customLevelPath);
			Logger->debug("MainLoop_46830:init game level passed");
			if (CommandLineParams.DoAutoChangeRes()) {
				resindex_begin = 0;
			}

			//!!!!!!!!!!! debug fix it
			//mouseturnoff = true;
			/*uint16_t testa1[3];
			testa1[0] = 0x4a80;
			testa1[1] = 0xd480;
			testa1[2] = 0x1320;
			sub_50A90((int)testa1, 17, 17);*/
			//UpdateSpellPickupFloating_692A0(Entities_EA3E4[0xc4e0 / 168]);
			//!!!!!!!!!!! debug fix it

			LoadSpr_47160();
			// debug !!!
			//UpdateSpellPickupFloating_692A0(testarraymain);
			// debug !!!
			//test_x_D41A0_BYTEARRAY_0();

			//fix
			//write_pngs2();//only for export
			//fix

			while (!world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234)
			{
				if (musicAble_E37FC && musicActive_E37FD && m_iNumberOfTracks)
				{
					switch (world_D41A0.terrain_2FECE.MapType) {
					case MapType_t::Day:
						world_D41A0.maptypeMusic_0x235 = 2;
						break;
					case MapType_t::Night:
						world_D41A0.maptypeMusic_0x235 = 1;
						break;
					case MapType_t::Cave:
						world_D41A0.maptypeMusic_0x235 = 3;
						break;
					}
				}
				SetCenterScreenForFlyAssistant_6EDB0();
				if (m_ptrGameRender == nullptr)
				{
					if (!strcmp(m_render.c_str(), "HW"))
						m_ptrGameRender = (GameRenderInterface*)new GameRenderHW(pdwScreenBuffer_351628, *xadatapald0dat2.colorPalette_var28, viewDistanceScale);
					else if (!strcmp(m_render.c_str(), "Original"))
						m_ptrGameRender = (GameRenderInterface*)new GameRenderOriginal();
					else if (!strcmp(m_render.c_str(), "HD"))
						m_ptrGameRender = (GameRenderInterface*)new GameRenderHD(pdwScreenBuffer_351628, *xadatapald0dat2.colorPalette_var28, (multiThreadedRender ? numberOfRenderThreads : 0), assignToSpecificCores, sizePercentToThreadRender, sizePercentToThreadRenderSprites, viewDistanceScale);
					else
					{
						if ((gameResWidth <= 640) && (gameResHeight <= 480)) {
							m_ptrGameRender = (GameRenderInterface*)new GameRenderOriginal();
						}
						else {
							m_ptrGameRender = (GameRenderInterface*)new GameRenderNG(viewDistanceScale, fogScale, multiThreadedRender ? numberOfRenderThreads : 0, assignToSpecificCores, sizePercentToThreadRender);
						}
					}
				}
				InGameLoop_47320();
				if (m_ptrGameRender != nullptr)
				{
					delete m_ptrGameRender;
					m_ptrGameRender = nullptr;
				}
				CloseMovie_53CC0();
				EndSample_8D8F0();
				StopMusic_8E020();
				StopCdPlayback_86860(cdDrive_1803EC);
				RestoreSoundVolume_59BF0();
				VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
				if (videoMode_180660 & 1)
				{
					ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0));
					ClearScreenAlphaBuffer(320, 200, 255);
				}
				else
				{
					ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, GetPaletteIndex_5BE80((TColor*)*xadatapald0dat2.colorPalette_var28, 0, 0, 0));
					ClearScreenAlphaBuffer(640, 480, 255);
				}
				if (videoMode_180660 & 1)
					VgaBlit320_90478(maxGameFps);
				else
					VgaBlit640_75200(480, maxGameFps);
				if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2
					&& !(gameState_D41A4.setting_38545 & 4))
				{
					GrantSpellToPlayer_6DB50(1, 0);
				}
				FreeTmapsAndAnimations_713A0();
				if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 4)
				{
					LoadLevel_56D60(actLevel, 0);
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] = 4;
				}
				else if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2)
				{
					CollectLevelStats_5C530();
					if (gameState_D41A4.setting_38545 & 0x20)
						EnableAllSpells_6E0D0();
				}
				else
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] |= 8;
				if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 0x10)
				{
					actLevel = gameState_D41A4.levelnumber_43w;
					if (actLevel >= 0x18u)
					{
						if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2)
							CollectLevelStats_5C530();
						break;
					}
					if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 2)
					{
						Type_SecretMapScreenPortals_E2970* secretsPortals = GetSecretAndActivedPortal_824B0(actLevel);
						if (secretsPortals)
						{
							count_begin++;//for debug

							gameState_D41A4.levelnumber_43w = secretsPortals->levelNumber_6;
							
							//fix level number-neoriginal code
							actLevel = gameState_D41A4.levelnumber_43w;
							//fix level number-neoriginal code

							//fix res before secret level-neoriginal code
							VGA_Resize(320, 200);
							screenWidth_18062C = 320;
							screenHeight_180624 = 200;
							SetViewport_A0D50(0, 0, 320, 200);
							videoMode_180660 = 1;
							//fix res before secret level-neoriginal code

							LoadScreen_47FC0(true);
							LevelInitGame_56A30(actLevel);
							LoadSpr_47160();
						}
					}
				}
				else if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 0xA)
				{
					break;//must be here
				}
			}
			// The level is over and we are on our way back to the menu.  Give the network
			// session back here, or the next network game is refused before it sends a
			// single packet - see NetworkLeaveSession().
			if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
				NetworkLeaveSession();

			if (CommandLineParams.DoNetworkDebug())
				debug_net_printf("MATCHEND: level torn down, heading for the menu\n");

			// The scoreboard belongs to the match that has just ended, so it is worked out here,
			// while its numbers still stand.  The menu turns it into a popup when it opens its own
			// message list; one posted from inside the game would go with the list the menu
			// deletes on its way out.
			if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
			{
				g_matchResultPending = MatchScoreResultLine();
				if (CommandLineParams.DoNetworkDebug())
					debug_net_printf("MATCHEND: result %s\n", g_matchResultPending.c_str());
			}

			nextMenu_E29D8 = MenuItem::MainMenu;
			skipMenus = false;
			setLevel = -1;
			customLevelPath = "";
		}

		if (CommandLineParams.DoTestRenderers()) {
			if (renderer_tests_quit) {
				nextMenu_E29D8 = MenuItem::Exit; // exit menu loop	
				break;
			}
		}
	}
}

int debug_first_run = 0;
int debugcounter_228320 = 0;

//----- (00047320) --------------------------------------------------------
void InGameLoop_47320()//228320
{
	/*
	debugnextlevel++;


	uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;

	comp20 = compare_with_sequence((char*)"00228323-002DC4E0", (uint8_t*)x_BYTE_10B4E0_terraintype, 0x2dc4e0, debugcounter_228320, 0x70000, 0x10000, &origbyte20, &remakebyte20);
	comp20 = compare_with_sequence((char*)"00228323-002DC4E0", (uint8_t*)x_BYTE_11B4E0_height, 0x2dc4e0, debugcounter_228320, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x10000);
	comp20 = compare_with_sequence((char*)"00228323-002DC4E0", (uint8_t*)x_BYTE_12B4E0_shading, 0x2dc4e0, debugcounter_228320, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x20000);
	comp20 = compare_with_sequence((char*)"00228323-002DC4E0", (uint8_t*)x_BYTE_13B4E0_angle, 0x2dc4e0, debugcounter_228320, 0x70000, 0x10000, &origbyte20, &remakebyte20, 0x30000);
	//comp20 = compare_with_sequence((char*)"00228320", (uint8_t*)x_BYTE_14B4E0, 0x2dc4e0, debugcounter11, 0x70000,0x10000, &origbyte20, &remakebyte20, 0x40000);
	comp20 = compare_with_sequence((char*)"00228323-002DC4E0", (uint8_t*)x_WORD_15B4E0_source, 0x2dc4e0, debugcounter_228320, 0x70000, 0x20000, &origbyte20, &remakebyte20, 0x50000);

	//uint8_t origbyte20 = 0;
	//uint8_t remakebyte20 = 0;
	comp20 = compare_with_sequence_D41A0((char*)"00228323-00356038", (uint8_t*)&D41A0_BYTESTR_0, 0x356038, debugcounter_228320, 224790, &origbyte20, &remakebyte20);

	debugcounter_228320++;
	*/
	gameState_D41A4.paletteMod_51 = 0;
	uint32_t gameTurn = 0;
	world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.word[1] = 0;

	//fix res on begin level for hidden levels-neoriginal code
	lowResScaledUi = UseLowResScaledUi();
	if (lowResScaledUi)
	{
		// Narrower than 640: the level stays with the low res UI, laid out to the screen at the game resolution
		SetLowResScaledUiScreen();
		resindex_begin = 0;
		EventDispatcher::I->DispatchEvent(EventType::E_RESOLUTION_CHANGE, screenWidth_18062C, screenHeight_180624);
		viewPort.ResizeViewPort_2CA90(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);
	}
	else if (!IsDefaultResolution320(gameResWidth, gameResHeight))
	{
		VGA_Resize(320, 200);
		screenWidth_18062C = 320;
		screenHeight_180624 = 200;
		SetViewport_A0D50(0, 0, 320, 200);
		videoMode_180660 = 1;
		resindex_begin = 0;
	}
	//fix res on begin level for hidden levels-neoriginal code

	EventDispatcher::I->DispatchEvent(EventType::E_GAME_STATE_CHANGE, GameState::STARTED);
	EventDispatcher::I->DispatchEvent<ResourceType, uint8_t>(EventType::E_RESOURCE_CHANGE, ResourceType::CLEAR_COLOURIDX_SET, m_BackgroundColour_D4B7C);

	g_inGameLoop = true;

	while (1)
	{
		g_state_monitor.Update();

		//debug
		if (CommandLineParams.DoRightButton()) {
			if (CommandLineParams.DoDebugafterload())
			{
				if (debug_first_run % 10 == 8)
				{
					if (!mouseRightButton2_18074A && !mouseRightButton_180744)//first cycle after press and ...
					{
						//mouseRightButton_180744 = 1;
						mouseLeftButton_180746 = 1;
						//mousePosX_E375C = temp_mouse_x;
						//mouse_state = temp_mouse_y;
						//mousePosY_E375E = temp_mouse_y;
					}
					//mouseRightButton2_18074A = 1;
					mouseLeftButton2_18074C = 1;
				}
				debug_first_run++;
			}
		}

		//debug

		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234 || world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 8)
			break;//end level
		/*
		//savetext
		FILE* filesavetext;
		filesavetext=fopen("testsavetext.data", "wb");
		fwrite(textureAddresses_DDF50[0],2621440,1, filesavetext);
		fclose(filesavetext);
		//textureAddresses_DDF50
		//savetext
		*/
		DrawAndEventsInGame_47560(GameTimerTurn_17DB54);
		if (gameTurn < 2)
		{
			StopMusic_8E020();
			if (gameTurn == 1)
			{
				StartMusic_8E160(world_D41A0.maptypeMusic_0x235, 0x7Fu);
				SOUND_ShowMusicWarning();
			}
			gameTurn++;
		}

		// force special settings for renderer tests			
		if (CommandLineParams.DoTestRenderers()) {
			// force player turn
			SetMousePositionInMemory_5BDC0(
				renderer_tests[CommandLineParams.GetSetLevel()].set_mouse_x,
				renderer_tests[CommandLineParams.GetSetLevel()].set_mouse_y
			);

			if (renderer_tests[CommandLineParams.GetSetLevel()].set_flatshader) {
				world_D41A0.m_GameSettings.display_0x2196.flat_0x2199 = 1;
			}

			// force up key pressed
			LastPressedKey_1806E4 = 0x48;
			pressedKeys_180664[keys_EB39E[0]] = 1;

			if (stop_renderer_tests()) {
				break;
			}
		}
	}
	g_inGameLoop = false;
	//Clear pause status
	gameState_D41A4.OptionsSettingFlag_24 &= ~GAME_PAUSED;

	EventDispatcher::I->DispatchEvent(EventType::E_GAME_STATE_CHANGE, GameState::GAMEPLAY_ENDED);

	if(IsRecording())
		m_InputRecorder->SaveRecording();

	lowResScaledUi = false;
	VgaSetVideoMode640x480AndPalette_90E07((TColor*)*xadatapald0dat2.colorPalette_var28);
}

type_entity_0x6E8E OldTestEntity[256];

void analyzeEntites() {
	int lastTempEntity = 0;
	for (type_entity_0x6E8E* mx = Entities_EA3E4[1]; mx < Entities_EA3E4[1000]; mx++)
	{
		if ((mx->class_0x3F_63 == 0xB) && (mx->model_0x40_64 == 0x20))
		{
			if (memcmp(&OldTestEntity[lastTempEntity], mx, sizeof(type_entity_0x6E8E)) != 0)
			{
				OldTestEntity[lastTempEntity] = *mx;
			}
			lastTempEntity++;
		}
	}
};

// cheat "access all spells" (Alt+F1), also --all_spells
void GiveAllSpells_Cheat(type_entity_0x6E8E* actEvent)
{
	for (int k = 0; k < 26; k++)
	{
		if (!actEvent->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[k])
		{
			//adress 23354c
			type_entity_0x6E8E* locEv = CallCreateFunction_4A190(0x2321a0 + 0x20 * k, &actEvent->position_0x4C_76);
			if (locEv)
			{
				locEv->flags_0xc_12_15.byte[0] |= 1u;
				locEv->manaRegen_0x88_136 = 0;
				locEv->parentId_0x28_40 = actEvent - world_D41A0.entities_0x6E8E;
				actEvent->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[k] = locEv - world_D41A0.entities_0x6E8E;
				for (int n = 0; n < 10; n++)
				{
					if (actEvent->playerData_0xA4_164x->str_611.SpellIndexes_0x39B_923x.SpellIndex[n] == -1)
					{
						actEvent->playerData_0xA4_164x->str_611.SpellIndexes_0x39B_923x.SpellIndex[n] = k;
						break;
					}
				}
			}
		}
	}
	for (type_entity_0x6E8E* evIndex = Entities_EA3E4[1]; evIndex < Entities_EA3E4[1000]; evIndex++)
	{
		if (evIndex->class_0x3F_63 == 11)
			evIndex->flags_0xc_12_15.byte[0] &= 0xFEu;
	}
}

// level start: save of a recording - stored when recording, loaded when playing
void RecordingLevelSave()
{
	if (CommandLineParams.DoAllSpells())//before the save of a recording
		GiveAllSpells_Cheat(Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]);
	if (m_InputRecorder == nullptr)
		return;
	uint16_t level = gameState_D41A4.levelnumber_43w;
	m_InputRecorder->LevelStarted(level);//a restart of the level takes the save of its own start
	const bool resave = m_InputRecorder->m_IsPlaying && !m_InputRecorder->m_ResavePath.empty();//a save of its own
	const std::vector<uint8_t>* save = m_InputRecorder->GetLevelSave(level);
	if (m_InputRecorder->m_IsPlaying && save != nullptr && !resave)
		LoadLevelFromBuffer(*save);
	else if (m_InputRecorder->m_IsRecording || resave)
		m_InputRecorder->RecordLevelSave(level, SaveLevelToBuffer());
}

void intervalsave(int index) {
	char outname[512];
	sprintf(outname, "-%d", index % 5000);
	type_D41A0_BYTESTR_0 temp0x39 = world_D41A0;
	SaveLevel_55080(0, gameState_D41A4.levelnumber_43w, outname);
	world_D41A0 = temp0x39;
};

//long debugcounter_47560_2=0;
//----- (00047560) --------------------------------------------------------
void DrawAndEventsInGame_47560(int16_t turn)//228560
{
	SetFrameStart(std::chrono::system_clock::now());
	if ((CommandLineParams.ModeRegressionsTestType() != -1) && (count_begin == 1))
		debugcounter_47560++;
	PaletteChanges_47760();
	if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
	{
		UpdateSpriteAnimations_715B0();//nothing draw //animate sprites
	}
	ReadGameUserInputs_89D10();//get keys

	if (CommandLineParams.DoAutoChangeRes()) {
		if ((gameResWidth >= 640) && (gameResHeight >= 480))
		{
			if (resindex_begin == 1)
			{
				//Auto press "r" key
				LastPressedKey_1806E4 = 0x13;
				resindex_begin++;
			}
			else
			{
				if (resindex_begin == 2)
				{
					LastPressedKey_1806E4 = 0;
					resindex_begin++;
				}
				else
				{
					if (resindex_begin < 1)
					{
						resindex_begin++;
					}
				}
			}
		}
	}
	MouseAndKeysEvents_17A00(turn);
	//debug
	if (CommandLineParams.DoDebugafterload())
	{
		if (debug_first_run == 5)
		{
			//loadD41A0();
			//gameState_D41A4.langIndex_4 = 1;
			//InitLanguage_76A40_mod_only_language();
			if (LoadLevel_555D0(0u, gameState_D41A4.levelnumber_43w, true))
				sprintf(printbuffer, "%s:%s.", langStrings_E9C4C[423], "OK");//Load Level
			else
				sprintf(printbuffer, "%s:%s.", langStrings_E9C4C[423], langStrings_E9C4C[429]);//429 - Failed
			ShowMessage_52D70(0, printbuffer);
			gameState_D41A4.OptionsSettingFlag_24 ^= GAME_PAUSED;
		}
		if (debug_first_run == 6)
			gameState_D41A4.OptionsSettingFlag_24 ^= GAME_PAUSED;
		if (!CommandLineParams.DoRightButton())
			debug_first_run++;
	}
	//debug
	if (CommandLineParams.DoIntervalSave()) {
		//save in interval
		int interval = 1;
		if (save_debugcounter % interval == 0)
			intervalsave(save_debugcounter / interval);
		//save in interval
		save_debugcounter++;
	}
	//adress 228583
	if (CommandLineParams.DoDebugSequences2()) {
		//add_compare(0x228583, CommandLineParams.DoDebugafterload());
	}
	PlayerEvents_51BB0();//nothing draw
	//adress 228588
	UpdateLightSourcesOld_848A0();//nothing draw
	//adress 22858d
	uint8_t speed = gameState_D41A4.speedIndex;
	if (speed == 0)
	{
		if (!speed)
			UpdateEntities_57730();
	}
	else if (speed == 1)
	{
		for (int i = 0; i < 4; i++)
			UpdateEntities_57730();
	}
	else if (speed == 2)
	{
		for (int j = 0; j < 8; j++)
			UpdateEntities_57730();
	}
	UpdateLightSources_84B80();//prepare lightting
	GameObjectives_58F00();//nothing draw
	PresentObjective_59820();//nothing draw
	if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
		CheckLevelEndReached_57570();//nothing draw
	CheckLevelEndFlags_575C0();//nothing draw
	PlayEntitySounds_6E150();//nothing draw
	DrawGameFrame_2BE30();
	//adress 2285ff
	//add_compare(0x002285FF, CommandLineParams.DoDebugafterload());
	if (CommandLineParams.DoTestRegression()) {
		add_compare(0x002285FF, IsAfterLoad, -1, false, CommandLineParams.GetMaxRegressionsSteps());
		//add_compare(0x002285FF, CommandLineParams.DoDebugafterload(), 6);
	}
	if (CommandLineParams.ModeRegressionsTestType()>0) {
		add_compare(0x002285FF, IsAfterLoad, -1, false, CommandLineParams.GetMaxRegressionsSteps());
		//add_compare(0x002285FF, IsAfterLoad || (CommandLineParams.GetPlaybackPath().length() > 0), -1, false, CommandLineParams.GetMaxRegressionsSteps(),2140);
		//add_compare(0x002285FF, CommandLineParams.DoDebugafterload(), 6);
	}

	if (CommandLineParams.DoDebugSequences2()) {
		//add_compare(0x002285FF, CommandLineParams.DoDebugafterload());
	}
	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x002285FF, CommandLineParams.DoDebugafterload());
	}
	/*if (debugcounter_47560_2>=0x8f)
	{
		debugcounter_47560_2++;
		debugcounter_47560_2--;
	}
	debugcounter_47560_2++;*/
	if (CommandLineParams.DoAnalyzeEntity()) {
		analyzeEntites();
	}
	if (gameState_D41A4.showHelp_10)
		DrawHelpPopUps_871F0();
	//adress 22860f
	if (CommandLineParams.DoDetectDwordA()) {
		for (int iii = 1; iii < 0x3E9; iii++)
			if (Entities_EA3E4[iii]->oldMapEntity_0x16_22 == iii)
				allert_error();
		for (int jx = 1; jx < 50; jx++)
		{
			if (world_D41A0.dynamicLights_0x3664C[jx].active_0)
				if (world_D41A0.dynamicLights_0x3664C[jx].event_A.pointer_0x6E8E < world_D41A0.entities_0x6E8E || world_D41A0.dynamicLights_0x3664C[jx].event_A.pointer_0x6E8E >= &world_D41A0.entities_0x6E8E[1000])
					allert_error();
		}
	}
	gameState_D41A4.lastTurnTime_196 = GameTimerTurn_17DB54 - gameState_D41A4.lastTurnTime_196;
	DrawGameDebugText_6FEC0();
	gameState_D41A4.lastTurnTime_196 = GameTimerTurn_17DB54;
	if (gameState_D41A4.paletteMod_51 >= 3u)
		BlendAndBlit_40F80();
}

//----- (00048350) --------------------------------------------------------
void ResetGameSettingsFlags_48350()//229350
{
	//int8_t* result; // eax

	gameState_D41A4.setting_38545 &= 0x43u;
	//result = (int8_t*)x_D41A0_BYTEARRAY_0;
	//x_D41A0_BYTEARRAY_0[0x36E0B] &= 0xFEu;
	world_D41A0.gameFlags_0x36E0B &= 0xFEu;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;

int debugcounter_234160 = 0;
//----- (00053160) --------------------------------------------------------
void UpdateGameFrame_53160()//234160
{
	//uint8_t* v0; // ebx
	//int v1; // esi
	//int v2; // eax
	__int16 v3; // ax
	__int16 v4; // dx
	//int v5; // eax
	char* v6; // edi
	//char v8; // al
	//char v9; // al
	//int result; // eax
	//x_BYTE *v11; // [esp+4h] [ebp-Ch]
	unsigned __int16 v12; // [esp+8h] [ebp-8h]
	char v13; // [esp+Ch] [ebp-4h]
	/*
	uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20 = compare_with_sequence_D41A0_4((char*)"00234160-0038CF50", (uint8_t*)& gameState_D41A4, 0x38cf50, debugcounter_234160, 0x94c, &origbyte20, &remakebyte20);
	comp20 = compare_with_sequence_D41A0((char*)"00234160-00356038", (uint8_t*)& D41A0_BYTESTR_0, 0x356038, debugcounter_234160, 224790, &origbyte20, &remakebyte20);
	debugcounter_234160++;
	*/
	int v0index;
	int v11index;

	v13 = 1;
	v12 = 0;
	//v11 = (x_BYTE *)(x_D41A0_BYTEARRAY_0 + 28222);
	//v0 = x_D41A0_BYTEARRAY_0 + 11230;//0x2bde
	v0index = 0;
	v11index = 0;
	do
	{
		//memset((x_BYTE *)(x_D41A0_BYTEARRAY_0 + 28222+ v11index*10), 0, 10);
		//for (int kk = 0; kk < 10; kk++)
		world_D41A0.playerInputs_0x6E3E[v11index].PlayerAction_byte0 = 0;
		world_D41A0.playerInputs_0x6E3E[v11index].param1_0x6E3E_byte1 = 0;
		world_D41A0.playerInputs_0x6E3E[v11index].param2_0x6E3E_byte2 = 0;
		world_D41A0.playerInputs_0x6E3E[v11index].roll = 0;
		world_D41A0.playerInputs_0x6E3E[v11index].pitch = 0;
		world_D41A0.playerInputs_0x6E3E[v11index].entityIndex_0x6E3E_byte5 = 0;
		world_D41A0.playerInputs_0x6E3E[v11index].nextEntity_0x6E3E_word6 = 0;
		world_D41A0.playerInputs_0x6E3E[v11index].entityIndex2_0x6E3E_word8 = 0;

		//qmemcpy(gameState_D41A4.byteindex_256ar, (void *)v0, 2124);
		//gameState_D41A4.byteindex_256ar = D41A0_BYTESTR_0.array_0x2BDE[v0index];
		//qmemcpy(gameState_D41A4.byteindex_256ar, x_D41A0_BYTEARRAY_0 + 11230+2124* v0index, 2124);
		gameState_D41A4.savedPlayer_256ar = world_D41A0.players_0x2BDE[v0index];
		//memset((void*)v0, 0, 2124);
		clean_x_D41A0_BYTEARRAY_0_0x2BDE(v0index);
		//v1 = &x_D41A0_BYTEARRAY_4[0x8CF];
		//qmemcpy((void *)(v0 + 1999), (void *)(&gameState_D41A4.byteindex_2255ar[0]), 24);
		//copyto_x_D41A0_BYTEARRAY_0_0x2BDE_0x7CF(v0index,&gameState_D41A4.byteindex_2255ar[0]);
		//qmemcpy((void *)(v0 + 2023), (void *)(&gameState_D41A4.byteindex_2255ar[24]), 2);
		//copyto_x_D41A0_BYTEARRAY_0_0x2BDE_0x7E7(v0index, &gameState_D41A4.byteindex_2255ar[24]);
		world_D41A0.players_0x2BDE[v0index].playerData_0x3E6_2BE4_12228.str_611.availableSpells_0x3E9_1001x = gameState_D41A4.savedPlayer_256ar.playerData_0x3E6_2BE4_12228.str_611.availableSpells_0x3E9_1001x;
		if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
			v13 = 0;
		if (v13)
			//CopyPlayerSpellData_549A0(v0 + 1609, &gameState_D41A4.byteindex_256ar[1609]);
			CopyPlayerSpellData_549A0(&world_D41A0.players_0x2BDE[v0index].playerData_0x3E6_2BE4_12228.str_611, &gameState_D41A4.savedPlayer_256ar.playerData_0x3E6_2BE4_12228.str_611);
		//v2 = (int)x_D41A0_BYTEARRAY_4;
		//*(x_DWORD *)(v0 + 24) = gameState_D41A4.byteindex_256ar[24];
		world_D41A0.players_0x2BDE[v0index].dword_0x018_2BDE_11254 = gameState_D41A4.savedPlayer_256ar.dword_0x018_2BDE_11254;
		//*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 28222 + v11index * 10) = 1;
		world_D41A0.playerInputs_0x6E3E[v0index].PlayerAction_byte0 = 1;
		//*(x_WORD *)(v0 + 7) = v12;
		world_D41A0.players_0x2BDE[v0index].playerSlot_0x007_2BE4_11237 = v12;
		if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && v12 != world_D41A0.LevelIndex_0xc)
			//*(x_BYTE *)(v0 + 9) = 1;
			world_D41A0.players_0x2BDE[v0index].IsAiPlayer_0x009_2BE4_11239 = 1;
		//*(x_WORD *)(v0 + 16) = 32;
		world_D41A0.players_0x2BDE[v0index].positionCount_0x010_2BDE_11246 = 32;
		//*(x_WORD *)(v0 + 477) = 128;
		world_D41A0.players_0x2BDE[v0index].positions_0x1d1_2BDE_11695[0].rotation__2BDE_11701.fov = 128;
		//v3 = *(x_WORD *)(v0 + 16);
		v3 = world_D41A0.players_0x2BDE[v0index].positionCount_0x010_2BDE_11246;
		//*(x_BYTE *)(v0 + 993) = 2;
		world_D41A0.players_0x2BDE[v0index].chatState_0x3E1_2BE4_12223 = 2;
		v4 = 0;
		//*(x_WORD *)(v0 + 14) = v3 - 1;
		world_D41A0.players_0x2BDE[v0index].ActPlayerIndex_0x00e_2BDE_11244 = v3 - 1;
		while (v4 < world_D41A0.players_0x2BDE[v0index].positionCount_0x010_2BDE_11246)
		{
			//v5 = 7 * v4++;
			v4++;
			//*(x_DWORD *)(v0 + 2 * v5 + 479) = *(x_DWORD *)(v0 + 465);
			//set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d1_2BDE_11707(v0index, v4/* + 1*/, get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d1_2BDE_11707(v0index, 0));
			world_D41A0.players_0x2BDE[v0index].positions_0x1d1_2BDE_11695[v4].axis_2BDE_11695 = world_D41A0.players_0x2BDE[v0index].positions_0x1d1_2BDE_11695[0].axis_2BDE_11695;
			//*(x_DWORD *)(v0 + 2 * v5 + 483) = *(x_DWORD *)(v0 + 469);
			//set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d5_2BDE_11707(v0index, v4/* + 1*/, get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d5_2BDE_11707(v0index, 0));
			//D41A0_BYTESTR_0.array_0x2BDE[v0index].struct_0x1d1_2BDE_11695[v4].axis_2BDE_11695.x = D41A0_BYTESTR_0.array_0x2BDE[v0index].struct_0x1d1_2BDE_11695[0].axis_2BDE_11695.dd;
			//*(x_DWORD *)(v0 + 2 * v5 + 487) = *(x_DWORD *)(v0 + 473);
			//set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d9_2BDE_11707(v0index, v4/* + 1*/, get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1d9_2BDE_11707(v0index, 0));
			//D41A0_BYTESTR_0.array_0x2BDE[v0index].struct_0x1d1_2BDE_11695[v4].axis_2BDE_11695.z = D41A0_BYTESTR_0.array_0x2BDE[v0index].struct_0x1d1_2BDE_11695[0].axis_2BDE_11695.dd;
			//*(x_WORD *)(v0 + 2 * v5 + 491) = *(x_WORD *)(v0 + 477);
			//set_x_D41A0_BYTEARRAY_0_0x2BDE_0x1dd_2BDE_11707(v0index, v4/* + 1*/, get_x_D41A0_BYTEARRAY_0_0x2BDE_0x1dd_2BDE_11707(v0index, 0));
			world_D41A0.players_0x2BDE[v0index].positions_0x1d1_2BDE_11695[v4].rotation__2BDE_11701.fov = world_D41A0.players_0x2BDE[v0index].positions_0x1d1_2BDE_11695[0].rotation__2BDE_11701.fov;
		}
		if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
			//*(x_BYTE *)(v0 + 1446) = netWizardTypes_E131C[v12];
			//set_x_D41A0_BYTEARRAY_0_0x2BDE_0x5a6(netWizardTypes_E131C[v12], netWizardTypes_E131C[v12]);
			world_D41A0.players_0x2BDE[v0index].playerData_0x3E6_2BE4_12228.wizardType_0x1C0_448 = netWizardTypes_E131C[v12];
		//v6 = (char *)(v0 + 927);//wizard name
		v6 = (char*)world_D41A0.players_0x2BDE[v0index].WizardName_0x39f_2BFA_12157;
		//adress 2342e4
		auto wizard_name = std::string(WizardsNames_D93A0[GetTrueWizardNumber_61790(v12)]);
		strcpy(v6, wizard_name.c_str());
		strcpy(world_D41A0.players_0x2BDE[v0index].WizardName_0x39f_2BFA_12157, wizard_name.c_str());
		/*do
		{
			v8 = *v7;
			*v6 = *v7;
			if (!v8)
				break;
			v9 = v7[1];
			v7 += 2;
			v6[1] = v9;
			v6 += 2;
		} while (v9);*/
		///*result = */InitialiseSpells_54A50(v12, x_D41A0_BYTEARRAY_0 + 11230 + 2124 * v0index);
		InitialiseSpells_54A50(v12, v0index);
		//v0 += 2124;
		v0index++;
		//v11 += 10;
		v11index++;
		v12++;
	} while (v12 < 8u);
	//return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// D93A0: using guessed type const char *off_D93A0_wizards_names2;

//----- (00055EB0) --------------------------------------------------------
void InitPlayerSlots_55EB0(__int16 a1)//236eb0
{
	unsigned int index; // eax
	type_entity_0x6E8E* ix; // edx

	index = 0;
	do
	{
		if (index < 0xC)
		{
			if (index == 1)
				goto LABEL_13;
		}
		else if (index <= 0xF || index >= 0x16 && index <= 0x17)
		{
			goto LABEL_13;
		}
		for (ix = gameState_D41A4.creatureLists_38403x[index]; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (ix->targetId_0x96_150 == a1)
				ix->targetId_0x96_150 = 0;
		}
	LABEL_13:
		index++;
	} while ((signed int)index < 29);
	//return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

void (*null_vector)() {
};

void write_pngs()
{
	uint8_t buffer[10000];
	char outname[512];

	for (int k = 0; k < 100; k++)
	{
		bitmap_pos_struct_t actposistruct = menuSprites_17DED4[k];
		if (actposistruct.width_4 > 0)
		{
			for (int i = 0; i < 10000; i++) {
				buffer[i] = 0xff;
			}

			//bitmap_pos_struct_t actposistruct = *filearray_2aa18c[0].posistruct[1];

			uint8_t* temp_screen = pdwScreenBuffer_351628;
			int temp_screen_x = screenWidth_18062C;

			screenWidth_18062C = actposistruct.width_4;
			pdwScreenBuffer_351628 = buffer;

			DrawBitmapFinal_8F935(actposistruct.width_4, actposistruct.height_5, 0, 0, actposistruct.data, 0, 0, 1);

			pdwScreenBuffer_351628 = temp_screen;
			screenWidth_18062C = temp_screen_x;

			sprintf(outname, "c:\\prenos\\remc2\\outimg\\test-%d.png", k);
			write_bitmap_pos_struct_to_png(buffer, actposistruct.width_4, actposistruct.height_5, outname);//test write
		}
	}
}

//----- (00056210) --------------------------------------------------------
static int ClampPort(int port)
{
	if (port < 0) return 0;
	if (port > 99999) return 99999;
	return port;
}

void ProcessCommandLine_56210(int argc, char** argv)//237210
{
	int32_t x_DWORD_355208;//3551CE+3A DWORD
	x_BYTE x_BYTE_355234_hardisknumber;//harddrive//3551CE+66 BYTE
	x_BYTE x_BYTE_35520C;//3551CE+3e BYTE
	x_BYTE x_BYTE_355210_level;//3551CE+42 BYTE
	x_BYTE x_BYTE_355230;//3551CE+62 BYTE
	x_BYTE x_BYTE_355218;//3551CE+4a BYTE
	x_BYTE x_BYTE_355244_spellsedit;//aSpellsedit//3551CE+76 BYTE
	x_BYTE x_BYTE_355240_load_set_level;//3551CE+72 BYTE
	x_BYTE x_BYTE_35522C_nocd;//nocd//3551CE+5e BYTE
	x_BYTE x_BYTE_355224_showversion;//Showversion//3551CE+56 BYTE
	x_BYTE x_BYTE_355228_showversion2;//Showversion2//3551CE+5a BYTE
	x_BYTE x_BYTE_355214;//3551CE+46 BYTE
	x_BYTE x_BYTE_35521C_cc;//3551CE+4e BYTE
	x_BYTE x_BYTE_35523C_detectoff;//Detectoff//3551CE+6e BYTE
	x_BYTE x_BYTE_355220_langcheck;//Langcheck//3551CE+52 BYTE
	x_BYTE x_BYTE_355238_music2;//aMusic2//3551CE+6a BYTE

	uint8_t argnumber; // ebx
	char arg0; // dl
	char* v4; // edi
	char* v5; // esi
	//char v6; // al
	//char v7; // al
	//unsigned __int8 v8; // al
	//int v9; // ebx
	//char* v10; // esi
	//int result; // eax
	char actarg[512] = "\0"; // [esp+0h] [ebp-46h]
	//int v13; // [esp+80h] [ebp+3Ah]
	//char v14; // [esp+84h] [ebp+3Eh]
	//unsigned __int8 v15; // [esp+88h] [ebp+42h]
	//char v16; // [esp+8Ch] [ebp+46h]
	//char v17; // [esp+90h] [ebp+4Ah]
	//char v18; // [esp+94h] [ebp+4Eh]
	//char v19; // [esp+98h] [ebp+52h]
	//char v20; // [esp+9Ch] [ebp+56h]
	//char v21; // [esp+A0h] [ebp+5Ah]
	//char v22; // [esp+A4h] [ebp+5Eh]
	//char v23; // [esp+A8h] [ebp+62h]
	//unsigned __int8 v24; // [esp+ACh] [ebp+66h]
	//char v25; // [esp+B0h] [ebp+6Ah]
	//char v26; // [esp+B4h] [ebp+6Eh]
	//char v27; // [esp+B8h] [ebp+72h]
	//char v28; // [esp+BCh] [ebp+76h]
	//fix it:
	argnumber = 1;
	x_DWORD_355208/* v13*/ = -1;//3551CE+3A DWORD //355208
	x_BYTE_355234_hardisknumber/*v24*/ = 'C';//harddrive//3551CE+66 BYTE
	x_BYTE_35520C/*v14*/ = 0;//3551CE+3e BYTE
	x_BYTE_355210_level/*v15*/ = 0;//3551CE+42 BYTE
	x_BYTE_355230/*v23*/ = 0;//3551CE+62 BYTE
	x_BYTE_355218/*v17*/ = 0;//3551CE+4a BYTE
	x_BYTE_355244_spellsedit/*v28*/ = 0;//aSpellsedit//3551CE+76 BYTE
	x_BYTE_355240_load_set_level/*v27*/ = 0;//3551CE+72 BYTE
	x_BYTE_35522C_nocd/*v22*/ = 0;//nocd//3551CE+5e BYTE
	x_BYTE_355224_showversion/*v20*/ = 0;//Showversion//3551CE+56 BYTE
	x_BYTE_355228_showversion2/*v21*/ = 0;//Showversion2//3551CE+5a BYTE
	x_BYTE_355214/*v16*/ = 0;//3551CE+46 BYTE
	x_BYTE_35521C_cc/*v18*/ = 0;//3551CE+4e BYTE
	x_BYTE_35523C_detectoff/*v26*/ = 0;//Detectoff//3551CE+6e BYTE
	x_BYTE_355220_langcheck/*v19*/ = 0;//Langcheck//3551CE+52 BYTE
	x_BYTE_355238_music2/*v25*/ = 0;//aMusic2//3551CE+6a BYTE
	ApplySetting_89B60(7u);
	while (argnumber < argc)
	{
		strcpy(actarg, argv[argnumber]);
		arg0 = argv[argnumber][0];// **(x_BYTE **)((int)argv + 4 * v2);
		if (arg0 == '-' || arg0 == '/')
		{
			v4 = actarg;
			v5 = argv[argnumber] + 1;//(char *)(*(x_DWORD *)((int)argv + 4 * v2) + 1);
			strcpy(v4, v5);
		}
		if (_stricmp("network", (char*)actarg))
		{
			if (!_stricmp("editor", (char*)actarg))
			{
				config_EDITOR = true;
			}
			else if (!_stricmp("reglevel", (char*)actarg))
			{
				test_regression_level = atoi(argv[++argnumber]);
			}
			else if (!_stricmp("level", (char*)actarg))
			{
				x_BYTE_355210_level = atoi(argv[++argnumber]);//2372C4 - 279F5D
				LoadLevelNumber_D419C = x_BYTE_355210_level;
				x_BYTE_355240_load_set_level = 1;
			}
			else if (!_stricmp("harddrive", (char*)actarg))
			{
				x_BYTE_355234_hardisknumber = x_toupper(argv[++argnumber][0]);//**(x_BYTE **)((int)argv + 4 * ++v2);
				//x_BYTE_355234_hardisknumber = x_toupper(x_BYTE_355234_hardisknumber);
				//x_BYTE_355234_hardisknumber = v8;
				if (x_BYTE_355234_hardisknumber < 'A' || x_BYTE_355234_hardisknumber > 'Z')
					x_BYTE_355234_hardisknumber = 'C';
			}
			else if (!_stricmp("skipscreens", (char*)actarg))
			{
				skipScreen_D41AD = 1;
			}
			else if (!_stricmp("nocd", (char*)actarg))
			{
				x_BYTE_35522C_nocd = 1;
			}
			else if (!_stricmp("showversion", (char*)actarg))
			{
				x_BYTE_355224_showversion = 1;
			}
			else if (!_stricmp("showversion2", (char*)actarg))
			{
				x_BYTE_355228_showversion2 = 1;
			}
			else if (!_stricmp("detectoff", (char*)actarg))
			{
				x_BYTE_35523C_detectoff = 1;
			}
			else if (!_stricmp("langcheck", (char*)actarg))
			{
				x_BYTE_355220_langcheck = 1;
			}
			else if (!_stricmp("vio", (char*)actarg))
			{
				//v9 = argnumber + 1;
				//v10 = argv[argnumber + 1];//*(x_DWORD *)((int)argv + 4 * v9);

				inputState_18058C.vio1AnalogJoy_1805C4 = atoi(argv[argnumber + 1]);
				argnumber += 2;
				inputState_18058C.vio2AnalogJoy_1805C6 = atoi(argv[argnumber]/* *(char**)((int)argv + 4 * v2)*/);
				ApplySetting_89B60(1u);
			}
			else if (!_stricmp("extern", (char*)actarg))
			{
				*(uint32_t*)&externControl_E36C4[0] = atoi(argv[++argnumber]/* *(char **)((int)argv + 4 * ++v2)*/);
				myprintf("External ctrl at %ul\n");
				ApplySetting_89B60(6u);
			}
			else if (!_stricmp("VFX1", (char*)actarg))
			{
				ApplySetting_89B60(2u);
			}
			else if (!_stricmp("cc", (char*)actarg))
			{
				x_BYTE_35521C_cc = atoi(argv[++argnumber]/* *(char**)((int)argv + 4 * ++v2)*/);
			}
			else if (!_stricmp("spellsedit", (char*)actarg))
			{
				x_BYTE_355244_spellsedit = 1;
			}
			else if (!_stricmp("music2", (char*)actarg))
			{
				x_BYTE_355238_music2 = 1;
			}
			// client <server ip> <server port> <own port>
			else if (!_stricmp("client", (char*)actarg))
			{
				Iam_client = true;
				strcpy(serverIP, (char*)argv[++argnumber]);
				ServerPort = ClampPort(atoi(argv[++argnumber]));
				NetworkPort = ClampPort(atoi(argv[++argnumber]));
			}
			// server <own port> - and nothing else.  The host holds one port, which serves
			// both the control traffic it answers and the game data it exchanges, so there is
			// nothing left to say: its address is loopback and its data port is that same
			// port.  It used to need a "client 127.0.0.1 <same port> <another port>" after
			// this, which named the host twice and gave it a second port to keep track of.
			else if (!_stricmp("server", (char*)actarg))
			{
				Iam_server = true;
				Iam_client = true;              // the host plays too, through its own server
				ServerPort = ClampPort(atoi(argv[++argnumber]));
				NetworkPort = ServerPort;
				strcpy(serverIP, "127.0.0.1");
			}
		}
		argnumber++;
	}
	if (!x_BYTE_35522C_nocd)//if cd
		InitialiseCdAudio_86A00();

	//x_D41A0_BYTEARRAY_0 = (uint8_t*)sub_83CD0_malloc2(0x36e17);//2A51A0
	/*if (!x_D41A0_BYTEARRAY_0)
	{
		myprintf("ERROR: CANT ALLOC CARPET MEMORY.");
		exit(-1);
	}*/

	/*x_D41A0_BYTEARRAY_4 = (uint8_t*)sub_83CD0_malloc2(0x9693);
	if (!x_D41A0_BYTEARRAY_4)
	{
		sub_83E80_freemem4(x_D41A0_BYTEARRAY_0);
		myprintf("ERROR: CANT ALLOC TAPE MEMORY.");
		exit(-1);
	}*/

	/*memset((void*)x_D41A0_BYTEARRAY_0[0], 0, 224791);
	memset((void*)x_DWORD_D41A4, 0, 38547);*/
	memset((void*)&world_D41A0, 0, sizeof(world_D41A0));
	//clean_x_D41A0_BYTEARRAY_0();

	//memset((void*)x_D41A0_BYTEARRAY_4, 0, 0x9693);

	if (x_BYTE_35520C)
	{
		gameState_D41A4.setting_byte1_22 |= 0x8;
		gameState_D41A4.OptionsSettingFlag_24 |= 0x8;
	}
	if (x_BYTE_355230)
		gameState_D41A4.OptionsSettingFlag_24 |= 2u;
	if (x_BYTE_355218)
		gameState_D41A4.OptionsSettingFlag_24 |= 8u;
	if (x_BYTE_355244_spellsedit)
		gameState_D41A4.OptionsSettingFlag_24 |= 0x10u;
	if (!x_BYTE_35522C_nocd && cdSpeechEnabled_E2A28)
		gameState_D41A4.OptionsSettingFlag_24 |= SPEECH_ENABLED;
	if (x_BYTE_355240_load_set_level)
		gameState_D41A4.OptionsSettingFlag_24 |= LEVEL_LOADED_FROM_ARG;
	if (x_BYTE_355214)
		gameState_D41A4.setting_byte2_23 |= 2u;
	if (x_BYTE_355224_showversion)
		gameState_D41A4.setting_byte4_25 |= 8u;
	if (x_BYTE_355220_langcheck)
		gameState_D41A4.setting_byte4_25 |= 0x10u;
	if (x_BYTE_355238_music2)
		gameState_D41A4.setting_byte4_25 |= 0x40u;
	if (x_BYTE_35523C_detectoff)
		autoScanForSoundHardware_E379B = true;
	//result = x_D41A0_BYTEARRAY_4;
	gameState_D41A4.levelnumber_43w = x_BYTE_355210_level;
	gameState_D41A4.setting_45w = x_DWORD_355208;
	if (x_BYTE_35521C_cc)
		gameState_D41A4.setting_byte4_25 = (gameState_D41A4.setting_byte4_25 | 4) & 0xFD;
	if (x_BYTE_355228_showversion2)
	{
		PrintHeader_70890();
		ShutdownGame_5BC20();
		exit(1);
	}
	//return (int)x_D41A0_BYTEARRAY_4;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 98805: using guessed type x_DWORD x_toupper(x_DWORD);
// 98F5D: using guessed type x_DWORD stricmp(x_DWORD, x_DWORD);
// 98F9E: using guessed type x_DWORD unknown_libname_1_atoi(x_DWORD);
// D419C: using guessed type char LoadLevelNumber_D419C;
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// D41AD: using guessed type char skipScreen_D41AD;
// E2A28: using guessed type char x_BYTE_E2A28;
// E36C4: using guessed type int x_DWORD_E36C4;
// 1805C4: using guessed type __int16 x_WORD_1805C4_vio1_analog_joy;
// 1805C6: using guessed type __int16 x_WORD_1805C6_vio2_analog_joy;

//----- (000567C0) --------------------------------------------------------
void ClearSettings_567C0()//2377c0 // clean level
{
	//int result; // eax
	//int v1; // ebx

	//memset((void*)(&x_D41A0_BYTEARRAY_0[16]), 0, 29);
	world_D41A0.modelCounters_0x10.fill(0);
	//memset((void*)(&x_D41A0_BYTEARRAY_0[45]), 0, 4);
	world_D41A0.dword_0x2d = 0;
	//memset((void*)(&x_D41A0_BYTEARRAY_0[49]), 0, 2);
	world_D41A0.volcanoEntity_0x31 = 0;
	//memset((void*)(&x_D41A0_BYTEARRAY_0[51]), 0, 2);
	world_D41A0.volcanoFireEntity_0x33 = 0;
	//memset((void*)(&x_D41A0_BYTEARRAY_0[53]), 0, 4);
	world_D41A0.freeEntityCount_0x35 = 0;
	//memset((void*)(&x_D41A0_BYTEARRAY_0[569]), 0, 1);
	world_D41A0.byte_0x239 = 0;
	memset(&world_D41A0.creatureStats_0x364D2, 0, sizeof(world_D41A0.creatureStats_0x364D2));
	//memset((void*)(&x_D41A0_BYTEARRAY_0[9058]), 0, 48);
	memset(&world_D41A0.playerStarts_0x2362, 0, sizeof(world_D41A0.playerStarts_0x2362));
	//clean_x_D41A0_BYTEARRAY_0_0x2362();
	//memset((void*)(&x_D41A0_BYTEARRAY_0[28302]), 0, 168000);
	memset(world_D41A0.entities_0x6E8E, 0, sizeof(type_entity_0x6E8E) * 0x3e8);
	memset(spellTableOverrun, 0, sizeof(spellTableOverrun));

	memset((void*)(&gameState_D41A4.paletteMod_51), 0, 1);
	memset((void*)(gameState_D41A4.colorIndex_121), 0, 16);
	memset((void*)(&gameState_D41A4.speedIndex), 0, 1);
	memset((void*)(&gameState_D41A4.paletteSubMod_180), 0, 1);
	memset((void*)(&gameState_D41A4.manaStats_242ar), 0, 14);
	memset((void*)(gameState_D41A4.creatureLists_38403x), 0, sizeof(type_entity_0x6E8E*) * 29);
	memset((void*)(&gameState_D41A4.class3List_38519), 0, sizeof(type_entity_0x6E8E*));
	memset((void*)(&gameState_D41A4.manaSphereList_38523), 0, sizeof(type_entity_0x6E8E*));
	memset((void*)(&gameState_D41A4.buildingList_38527), 0, sizeof(type_entity_0x6E8E*));
	memset((void*)(&gameState_D41A4.projectileList_38531), 0, sizeof(type_entity_0x6E8E*));
	memset((void*)(&gameState_D41A4.miscList_38535), 0, sizeof(type_entity_0x6E8E*));
	memset((void*)mapEntityIndex_15B4E0, 0, 0x20000);
	memset((void*)mapTerrainType_10B4E0, 0, 0x10000);
	memset((void*)mapHeightmap_11B4E0, 0, 0x10000);
	memset((void*)mapShading_12B4E0, 0, 0x10000);
	memset((void*)mapAngle_13B4E0, 0, 0x10000);
	memset((void*)smalltitBuffer_E9C38, 0, 0xfa00);
	//result = (int)x_D41A0_BYTEARRAY_4;
	gameState_D41A4.setting_30 = 0;
	//v1 = *(x_DWORD *)(x_D41A0_BYTEARRAY_4 + 22);
	gameState_D41A4.FrameTimingIndex_26 = 0;
	gameState_D41A4.setting_byte1_22 &= 0xFFFE3FFF;
	//return result;
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// E9C38: using guessed type int smalltitBuffer_E9C38;

//----- (00056A30) --------------------------------------------------------
void LevelInitGame_56A30(int16_t level, std::string customLevelPath)//237a30
{
	// The game that is starting owns the scoreboard from here, and the result of the previous
	// one has been read by now - this is what "hangs there until the next game" means.
	//
	// Not at NetworkInitConnection_7308F, which is where it first went: that runs when the
	// player enters the multiplayer session menu, 300 ms after the level was torn down, so the
	// result was wiped before the menu ever drew it.
	MatchScoreReset();

	if (CommandLineParams.DoMouseOff()) { mouseturnoff = true; }
	if (level > -1) {
		gameState_D41A4.levelnumber_43w = (uint16_t)level;
	}
	Logger->debug("LevelInitGame_56A30:before sub_6EB90");
	//fixing
	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_BUILD00DATTAB]);//24fb90 adress 0x23ca2e
	//fixing
	Logger->debug("LevelInitGame_56A30:sub_6EB90 passed");

	char temp_x_BYTE_E3799_sound_card = soundActive_E3799;
	soundActive_E3799 = false;
	ClearSettings_567C0();
	if (!(gameState_D41A4.setting_byte1_22 & 8))
	{
		PrintTextMessage_70910((char*)"Load Level\0");

		Logger->debug("LevelInitGame_56A30:before LevelDecompress_533B0");

		LevelDecompress_533B0(gameState_D41A4.levelnumber_43w, &world_D41A0.terrain_2FECE, customLevelPath);

		Logger->debug("LevelInitGame_56A30:LevelDecompress_533B0 passed");

	}
	ReadAndDecompressSkyAndBlocks_54660(world_D41A0.terrain_2FECE.MapType, TileTextureSize_D41B5);//235660
	ReadAndDecompressTables_54800(world_D41A0.terrain_2FECE.MapType);//235800
	//237ab3
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
		world_D41A0.NumberOfPlayers_0xe = world_D41A0.terrain_2FECE.numberOfPlayers_0x2FED7;
	PrintTextMessage_70910((char*)"Generate map\0");
	if (!(gameState_D41A4.setting_byte1_22 & 4))
		GenerateLevelMap_43830(&world_D41A0.terrain_2FECE);
	ResetEntityLists_49F30();//prepare events pointers
	//237B05
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x237B05, CommandLineParams.DoDebugafterload());
	}
	PrintTextMessage_70910((char*)"Generate features\0");
	if (!(gameState_D41A4.setting_byte1_22 & 4))
		GenerateLevelFeatures_49270(&world_D41A0.terrain_2FECE);
	PrintTextMessage_70910((char*)"Initialise Models\0");
	memset(&predictedAxis_EB398ar, 0, 6);
	RebuildEntityLists_49F90();
	//adress 237B55
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x237B55, CommandLineParams.DoDebugafterload());
	}
	world_D41A0.freeEntityCount2_0x11e6 = -1;
	SetTmapsFile_71A70(world_D41A0.terrain_2FECE.MapType);
	//adress 237b75
	if (!(gameState_D41A4.setting_byte1_22 & 4))
	{
		InitStages_58940();
		InitStageVars_11EE0();
		ClearDynamicLights_84790();
	}
	//adress 237BB0
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x237BB0, CommandLineParams.DoDebugafterload());
	}
	if (CommandLineParams.DoSetObjective()) {
		// FIXME: D41A0_BYTESTR_0 undeclared
		// D41A0_BYTESTR_0.struct_0x3659C[0].substr_3659C.stage_0x3659F[0] = 2;
		// D41A0_BYTESTR_0.struct_0x3659C[0].substr_3659C.stage_0x3659F[1] = 2;
		// D41A0_BYTESTR_0.struct_0x3659C[0].substr_3659C.stage_0x3659F[2] = 2;
		// D41A0_BYTESTR_0.struct_0x3659C[0].substr_3659C.stage_0x3659F[3] = 2;
		// //D41A0_BYTESTR_0.struct_0x3659C[0].substr_3659C.stage_0x3659F[4] = 2;
		// D41A0_BYTESTR_0.struct_0x3659C[0].substr_3659C.array_0x3659C_byte[1] = 4;
		// //D41A0_BYTESTR_0.struct_0x3654C[0].str_3654E_word2 = 40;
		// //D41A0_BYTESTR_0.struct_0x3654C[0].str_36550_word4 = 40;
	}
	ResetLevelState_4A1E0(0, 1);
	//adress 237bb9
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x237bb9, CommandLineParams.DoDebugafterload());
	}
	soundActive_E3799 = temp_x_BYTE_E3799_sound_card;
	UpdateGameFrame_53160();
	//adress 237bc7
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x237BC7, CommandLineParams.DoDebugafterload());
	}
	//adress 237beb
	UpdateCastles_60F00();
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x237BF0, CommandLineParams.DoDebugafterload());
	}
	RecordingLevelSave();
}

//----- (0005B7A0) --------------------------------------------------------
void PrepareTextures_5B7A0()//23C7A0
{
	if (bigTextures)
	{
		TileTextureSize_D41B5 = texturepixels;
	}

	uint8_t* oldbuf = BLOCK32DAT_BEGIN_BUFFER;
	if (TileTextureSize_D41B5 == 128)
		BLOCK32DAT_BEGIN_BUFFER = BigTextureBuffer;

	//TileTextureSize_D41B5 = 16;

	//v0 = x_BYTE_D41B5;
	FillD4350Table_3B4D0(TileTextureSize_D41B5);//21C4D0
	//v1 = 256 % (256 / TileTextureSize_D41B5);
	int texture_addresses_index = 0;
	for (int ypos = 0; ypos < (signed int)(256 / (256 / TileTextureSize_D41B5)); ypos++)
	{
		for (int xpos = 0; xpos < (256 / TileTextureSize_D41B5); xpos++)
		{
			uint8_t* ptrTexture = (uint8_t*)((ypos * TileTextureSize_D41B5 << 8) + (xpos * TileTextureSize_D41B5) + BLOCK32DAT_BEGIN_BUFFER);
			textureAddresses_DDF50.at(texture_addresses_index++) = ptrTexture;
		}
	}
	LoadPaletteAndHelpPalette_5B840();//23C840
	BLOCK32DAT_BEGIN_BUFFER = oldbuf;
}
// D41B5: using guessed type char x_BYTE_D41B5;
// DDF50: using guessed type int x_DWORD_DDF50[];
// E9C2C: using guessed type int x_DWORD_E9C2C;

//----- (0005B840) --------------------------------------------------------
void LoadPaletteAndHelpPalette_5B840()//23C840
{
	char dataPath[MAX_PATH];

	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALD-0.DAT");
	DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
	sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/CLRD-0.DAT");
	DataFileIO::ReadFileAndDecompress(dataPath, xadataclrd0dat.colorPalette_var28);
}
// EA3D8: using guessed type int *xadatapald0dat2.colorPalette_var28;

//----- (0005B870) --------------------------------------------------------
void CopySentence_5B870(char* langbuffer, char** langindexbuffer, uint32_t count)//23c870
{
	//CopySentence_5B870((x_BYTE *)langBuffer_D41BC, 0xE9C4C, 471);
	//edx 87ab14 ecx 1d7
  //x_BYTE *v3; // edx
  //int v4; // ecx
  //int result; // eax
  //x_DWORD *v6; // ebx
  //x_BYTE *v7; // ebx

	uint32_t langindex = 0;
	uint32_t indexindex = 0;
	while (indexindex < count)//1d7
	{
		langindexbuffer[indexindex] = &langbuffer[langindex];
		while (langbuffer[langindex++]);
		indexindex++;
	}
}

//----- (0005B8D0) --------------------------------------------------------
void Initialize()//23c8d0
{
	PrintHeader_70890();//23C8D6 - 251890
	if ((gameState_D41A4.setting_byte4_25) & 8)
		TestersInfo_5C490();//23C8E6 - 23D490

	if (useMagicCarpet1Music)
		gameState_D41A4.setting_byte4_25 |= 0x40u;

	Logger->debug("Init:Begin of creating dirs\n");

	std::string exepath = get_exe_path();

	if (CreateNetherSubdir_53D10(gameDataPath, "SAVE") != 3//23C906 - 234D10
		|| CreateNetherSubdir_53D10(gameDataPath, "CDATA") != 3//23C931 - 234D10
		|| CreateNetherSubdir_53D10(gameDataPath, "CLEVELS") != 3//23C95C - 234D10
		|| CreateNetherSubdir_53D10(gameDataPath, "SOUND") != 3//23C987 - 234D10
		|| CreateNetherSubdir_53D10(gameDataPath, "LANGUAGE") != 3//23C9B2 - 234D10
		|| CreateNetherSubdir_53D10(gameDataPath, "SHOTS") != 3)//23C9DD - 234D10
	{
		myprintf("Error creating setup directories.\n");
		exit(-1);
	}

	DataFileIO::SetCDFilePaths(cdDataPath.c_str(), pstr);

	Logger->debug("Init:End of creating dirs\n");

	CreateSoundDir_560D0();//23C9ED - 2370D0
	SetDefaultKeys_5BCC0();//23C9F2 - 23CCC0
	if (!LoadPsxData_5BF50())//23C9F7 - 23CF50 //something with files about their loading, or just a set of Palettes
		exit(-1);
	SetDefaultSettings_5C1B0();//23CA05 - 23D1B0
	if (!CreateUserDirectories_54200())//23CA0A - 235200 //the percentages go here too
	{
		myprintf("ERROR COPYING DATA FILES TO LOCAL DRIVE \n");
		exit(-1);
	}
	ProcessTmaps_71410();//252410

	//sub_6EB90((unsigned int **)*filearray_2aa18c);//24FB90
	//2AA18C
	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);//24fb90
	//sub_6EB90((unsigned int **)&*filearray_2aa18c[0x60]);//24FB90
	//2AA1EC
	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_BUILD00DATTAB]);//24fb90 adress 0x23ca2e

	LoadSearchData_101C0();//23CA36 - 1f11c0 //sem se to vubec nedostane
	//zkontrolovat inicializaci

	//*xadataspellsdat.colorPalette_var28 = (uint8_t*)malloc(10000);//fix it

	/*if ( !(gameState_D41A4.OptionsSettingFlag_24 & 8) )
	//fix it //gameState_D41A4.OptionsSettingFlag_24=50CF38 =0
	*/
	//*xadataspellsdat.colorPalette_var28 = (uint8_t*)malloc(50000);
	if (!(gameState_D41A4.OptionsSettingFlag_24 & 8))
	{
		char spellDataPath[MAX_PATH];
		sprintf(spellDataPath, "%s/%s", cdDataPath.c_str(), "DATA/SPELLS.DAT");
		DataFileIO::ReadFileAndDecompress(spellDataPath, xadataspellsdatx.colorPalette_var28);//234e60 buffer - 2ab818
	}

	//v0 = SetDefaultSpells_5C0A0();//23d0a0g
	SetDefaultSpells_5C0A0();
	if (gameInitialized_D4B80)
		exit(-1);
	gameInitialized_D4B80 = 1;
	videoMode_180660 = 1;//fix it
	if (videoMode_180660 & 1)
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 320, 200, 0);//253883
		ClearScreenAlphaBuffer(320, 200, 255);
	}
	else
	{
		ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);//2538a9
		ClearScreenAlphaBuffer(640, 480, 255);
	}

	//VGA_Write_basic_Palette(basic_pal_buffer);

  //VgaPalFadeInFadeOut_90B27(*xadatapald0dat2.colorPalette_var28, 0x20u, 0);

	VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);//271B27 tady se zapina vga
	//fixed
	ClearGraphicsBuffer_72883((void*)pdwScreenBuffer_351628, 640, 480, 0);//fixed
	ClearScreenAlphaBuffer(640, 480, 255);
	//VgaBlit640_75200(480);//debug
	//VgaBlit640_75200(480);//debug
	//fixed
  //*xadatapald0dat2.colorPalette_var28 = (uint8_t*)malloc(10000);//fix it
	memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
	VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);//222a90
	gameState_D41A4.setting_byte1_22 = 0;//fix it
	if ((gameState_D41A4.setting_byte1_22) & 0x110)//2a51a4 // 38cf50+16 //00 druh grafiky/inicializace
	{
		if (videoMode_180660 & 1)
			VgaSetVideoMode320x200AndPalette_90D6E((TColor*)*xadatapald0dat2.colorPalette_var28);
		else
			VgaSetVideoMode640x480AndPalette_90E07((TColor*)*xadatapald0dat2.colorPalette_var28);
		memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 768);
		VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);//222a90
	}
	else
	{
		VgaPalFadeInFadeOut_90B27(0, 0x10u, 0);
		videoMode_180660 = 1;//fix it
		if (videoMode_180660 & 1)
			VgaSetVideoMode320x200AndPalette_90D6E((TColor*)pdwScreenBuffer_351628);//3aa0a4
		else
			VgaSetVideoMode640x480AndPalette_90E07((TColor*)pdwScreenBuffer_351628);
		memset((void*)*xadatapald0dat2.colorPalette_var28, 0, 0x300);
		VgaPaletteInstall_41A90((TColor*)*xadatapald0dat2.colorPalette_var28);//222a90
	}
	if (gameState_D41A4.setting_byte1_22 & 8)
		NoOp_8C2CD();
	else
		ClearPressedKeys_753D0();
	// ?keyboard init

	soundActive_E3799 = soundAble_E3798;
	musicActive_E37FD = musicAble_E37FC;
	InstallMouse_8CEDF();
	if (!mouseInstalled_E3768)
	{
		ShutdownGame_5BC20();
		myprintf("ERROR : MOUSE DRIVER NOT INSTALLED\n");
		exit(-1);
	}
	//mouse init

	//pointersdat_buffer = xadatapointersdat.colorPalette_var28;//eb394 - 2bc394 -446f1{set in 23cf50}

	//nema to byt buffer obrazovky?! ---

	SetCursor_8CD27((*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct)[0]);//anything with vga, maybe mouse cursor//26dd27 //xadatapointersdat asi 1a6f44
	//1a6f44->1a6578 00 00 pointer->0b0005a1a1a1a1a10005a1b413aaa1
	//nastavi se zde:sub_6EB90(&filearray_2aa18c[filearrayindex_POINTERSDATTAB]);//24fb90

	//pak je to v 809380

	//VGA_Set_Cursor(*(*filearray_2aa18c[filearrayindex_POINTERSDATTAB].posistruct),0);

	NetworkAllocation2_5C430(); //23d430
	InitSoundAndMusic_46DD0(/*v2, v3, (char*)filearray_2aa18c[filearrayindex_POINTERSDATTAB].begin_buffer*/);//init sound and music//227DD0
	//x_WORD_E2A14_sound_activel = 0;x_BYTE_E3798_sound_active2 = 0;//debug tittes

	//test_x_D41A0_BYTEARRAY_0();
}

static void (*slow_tick_cb)(void) = nullptr;     // náhrada _chain_intr -> starý handler

static std::thread       gameTimerThread;
static std::atomic<bool> gameTimerRunning{ false };

static void GameTimerThread_6FD30()
{
	// 119 Hz ~ 8.4 ms perioda; sleep_for(1ms) jako v netThread, tick řešíme akumulátorem
	auto nextTick = std::chrono::steady_clock::now();
	const auto interval = std::chrono::microseconds(1000000 / 119);

	while (gameTimerRunning.load())
	{
		auto now = std::chrono::steady_clock::now();
		if (now >= nextTick)
		{
			nextTick += interval;
			GameTimerTurn_17DB54++;
			gameState_D41A4.timerAccumulator_2392 += gameState_D41A4.timerStep_2388;
			if (gameState_D41A4.timerAccumulator_2392 >= 0x10000)
			{
				gameState_D41A4.timerAccumulator_2392 -= 0x10000;

				if (slow_tick_cb)
					slow_tick_cb();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void StopProgrammableIntervalTimer()
{
	if (!gameTimerRunning.load()) return;
	gameTimerRunning.store(false);
	if (gameTimerThread.joinable())
		gameTimerThread.join();
}

struct GameTimerGuard {
	~GameTimerGuard() { StopProgrammableIntervalTimer(); }
};
static GameTimerGuard g_gameTimerGuard;

//----- (0006FDA0) --------------------------------------------------------
void SetProgrammableIntervalTimer_6FDA0()//fix//250da0
{
	//int v0; // eax
	//void(*v1)(); // eax
	//__int16 v2; // dx
	//int result; // eax

	//fix it
	//v2 = 0;
	//fix it

	//v0 = (int)x_D41A0_BYTEARRAY_4;
	gameState_D41A4.timerStep_2388 = 10022;
	gameState_D41A4.timerAccumulator_2392 = 0;
	//v1 = dos_getvect(8);
	//x_WORD_F5334 = v2;
	//x_DWORD_F5330 = v1;
	/* __outx_BYTE(0x43u, 0x36u);
	 __outx_BYTE(0x40u, x_D41A0_BYTEARRAY_4[0x954]);
	 __outx_BYTE(0x40u, x_D41A0_BYTEARRAY_4[0x954] >> 8);*/
	 //Set vector method to call
	 //result = dos_setvect(8, (DWORD)GameTimerThread_6FD30, (unsigned __int16)__CS__);
	 //BYTE1(result) = 1;
	timerInstalled_DB734 = 1;
	if (gameTimerRunning.load()) return;
	gameTimerRunning.store(true);
	gameTimerThread = std::thread(GameTimerThread_6FD30);
	//return result;
}

//----- (0005BC20) --------------------------------------------------------
void ShutdownGame_5BC20()//23cc20
{
	if (gameInitialized_D4B80 == 1)
	{
		NetworkDisallocation2_5C450();
		if (mouseInstalled_E3768)
			MouseReset_54600();//mouse reset
		/*if (gameState_D41A4.setting_byte1_22 & 8)
			sub_8C2DE();
		else
			sub_75420();*/
		ReleaseSoundTimer_46F50();
		//removed sub_8C21F_any_graphics_command(); // end of graphics?
		NetworkDisallocation_72D04();
		ClearProgrammableIntervalTimer_6FE20();
		UnloadPsxData_5C060();
	}
	//removed sub_83E80_freemem4(x_DWORD_D4198);
		//sub_83E80_freemem4(x_D41A0_BYTEARRAY_0);
		//sub_83E80_freemem4(x_D41A0_BYTEARRAY_4);
	StopCdPlayback_86860(cdDrive_1803EC);
	CloseCdDriver_85F00();
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// D4B80: using guessed type char gameInitialized_D4B80;
// E3768: using guessed type int mouseInstalled_E3768;
// 1803EC: using guessed type __int16 cdDrive_1803EC;

/*
char __cdecl GetPaletteIndexOrig_5BE80(unsigned __int8 *a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4)
{
  unsigned __int8 *v4; // eax
  signed int v5; // edx
  signed int v6; // ecx
  unsigned int v7; // edx
  int v9; // esi
  int v10; // esi
  int v11; // edi
  unsigned int v12; // [esp+0h] [ebp-8h]
  char v13; // [esp+4h] [ebp-4h]

  v4 = a1;//eax=[ebp+14]
  if ( word_180660 & 6 )//[351660]&6
	v5 = 16;//edx
  else
	v5 = 256;//edx
  v6 = 9999;//ecx
  v12 = v5;[ebp-8]=edx
  v7 = 0;//edx
  if ( v12 <= 0 )[ebp-8]<0
	return v13;
  do
  {
	v9 = a3 - v4[1];//[ebp+1c]-a1[1]
	v10 = (a2 - *v4) * (a2 - *v4) + v9 * v9;
	v11 = a4 - v4[2];//[ebp+20]-[eax+2]
	if ( v10 + v11 * v11 < v6 )
	{
	  v6 = v10 + v11 * v11;
	  v13 = v7;
	}
	++v7;
	v4 += 3;
  }
  while ( v7 < v12 );
  return v13;
}
*/

//----- (0005BF50) --------------------------------------------------------
signed int LoadPsxData_5BF50()//23cf50 //find 2bc394
{
	signed int result; // eax

	vesaInfoBuffer_181C40 = (uint8_t*)Malloc_83D70(0x100); //fix it 264D70
	if (vesaInfoBuffer_181C40)
	{
		if (DataFileIO::LoadFileArray_84250(psxasearchd_2bac30))//psxasearchd_2bac30~=2a9a54   set 2bc394(after 2A9A54) - set 2bab20
		{
			myprintf("NOT ENOUGH MEMORY\n");
			DataFileIO::UnloadFileArray_90D3F(psxasearchd_2bac30);
			result = 0;
		}
		else
		{
			if (DataFileIO::LoadFileArray_84250(psxadatabuild00dat))
			{
				myprintf("NOT ENOUGH MEMORY\n");
				DataFileIO::UnloadFileArray_90D3F(psxasearchd_2bac30);
				DataFileIO::UnloadFileArray_90D3F(psxadatabuild00dat);
				result = 0;
			}
			else
			{
				LoadBldgPrm_539A0();
				if (DataFileIO::LoadFileArray_84250(psxawscreen_351628))
				{
					DataFileIO::UnloadFileArray_90D3F(psxawscreen_351628);
					myprintf("NOT ENOUGH MEMORY\n");
					result = 0;
				}
				else
				{
					videoMode_180660 = 1;
					InitMemoryAndTextureSize_83B50();//264B50
					result = 1;
				}
			}
		}
	}
	else
	{
		myprintf("NO VESA DRIVER DETECTED\n");
		result = 0;
	}
	return result;
}
// 180660: using guessed type __int16 videoMode_180660;
// 181C40: using guessed type int vesaInfoBuffer_181C40;

//----- (0005C060) --------------------------------------------------------
signed int UnloadPsxData_5C060()//23d060
{
	DataFileIO::UnloadFileArray_90D3F(psxasearchd_2bac30);
	DataFileIO::UnloadFileArray_90D3F(psxadatabuild00dat);
	DataFileIO::UnloadFileArray_90D3F(psxawscreen_351628);
	return 1;
}

//----- (0005C1B0) --------------------------------------------------------
signed int SetDefaultSettings_5C1B0()//23A05 - 23D1B0
{
	//x_BYTE *v0; // eax
	//int v1; // eax
	//int v2; // eax
	//int v3; // eax
	//int v4; // eax
	//char v5; // dl
	unsigned __int16 v6; // dx
	unsigned __int16 v7; // bx

	InitEntityPointers_5C330();//23D330
	//x_D41A0_BYTEARRAY_0[8618] = 1;//0x21aa
	world_D41A0.visualSettings_0x21AA.creflections_0x21AA = 1;
	//x_D41A0_BYTEARRAY_0[8619] = 1;//0x21ab
	world_D41A0.visualSettings_0x21AA.cshadows_0x21AB = 1;
	//x_D41A0_BYTEARRAY_0[8620] = 1;//0x21ac
	world_D41A0.visualSettings_0x21AA.csky_0x21AC = 1;
	//x_D41A0_BYTEARRAY_0[8621] = 1;//0x21ad
	world_D41A0.visualSettings_0x21AA.calter_0x21AD = 1;
	//x_D41A0_BYTEARRAY_0[8622] = 1;//0x21ae
	world_D41A0.miscSettings_0x21AE.xxxx_0x21AE = 1;
	//x_D41A0_BYTEARRAY_0[8623] = 1;//0x21af
	world_D41A0.miscSettings_0x21AE.xxxx_0x21AF = 1;
	//x_D41A0_BYTEARRAY_0[8624] = 1;//0x21b0
	world_D41A0.miscSettings_0x21AE.xxxx_0x21B0 = 1;
	//x_D41A0_BYTEARRAY_0[8625] = 0;//0x21b1
	world_D41A0.miscSettings_0x21AE.xxxx_0x21B1 = 0;
	//x_D41A0_BYTEARRAY_0[8626] = 1;//0x21b2
	world_D41A0.screenSettings_0x21B2.xxxx_0x21B2 = 1;
	//x_D41A0_BYTEARRAY_0[8627] = 1;//0x21b3
	world_D41A0.screenSettings_0x21B2.xxxx_0x21B3 = 1;
	//x_D41A0_BYTEARRAY_0[8628] = 1;//0x21b4
	world_D41A0.screenSettings_0x21B2.cresolution_0x21B4 = 1;
	//x_D41A0_BYTEARRAY_0[8630] = 1;//0x21b6
	world_D41A0.detailSettings_0x21B6.clights_0x21B6 = 1;
	//x_D41A0_BYTEARRAY_0[8631] = 1;//0x21b7
	world_D41A0.detailSettings_0x21B6.csetting_0x21B7 = 1;
	//x_D41A0_BYTEARRAY_0[8632] = 1;//0x21b8
	world_D41A0.detailSettings_0x21B6.ctransparency_0x21B8 = 1;
	//x_D41A0_BYTEARRAY_0[8633] = 1;//0x21b9
	world_D41A0.detailSettings_0x21B6.cflat_0x21B9 = 1;

	GetSystemSpeed();//265050 //get computer speed

	//if (x_D41A0_BYTEARRAY_0[34])//0x22
	if (gameState_D41A4.m_wHighSpeedSystem)//0x22
	{
		//x_D41A0_BYTEARRAY_0[8586] = 1;//0x218a
		world_D41A0.m_GameSettings.m_Graphics.m_wReflections = (reflections) ? 1 : 0;
		//x_D41A0_BYTEARRAY_0[8588] = 1;//0x218c
		world_D41A0.m_GameSettings.m_Graphics.m_wSky = (sky) ? 1 : 0;
		//x_D41A0_BYTEARRAY_0[8598] = 1;//0x2196
		world_D41A0.m_GameSettings.display_0x2196.m_wDynamicLighting = (dynamicLighting) ? 1 : 0;
	}
	else
	{
		//x_D41A0_BYTEARRAY_0[8586] = 0;//0x218a
		world_D41A0.m_GameSettings.m_Graphics.m_wReflections = 0;
		//x_D41A0_BYTEARRAY_0[8588] = 0;//0x218c
		world_D41A0.m_GameSettings.m_Graphics.m_wSky = 0;
		//x_D41A0_BYTEARRAY_0[8598] = 0;//0x2196
		world_D41A0.m_GameSettings.display_0x2196.m_wDynamicLighting = 0;

		//Disable options
		world_D41A0.visualSettings_0x21AA.creflections_0x21AA = 0;
		world_D41A0.visualSettings_0x21AA.csky_0x21AC = 0;
		world_D41A0.detailSettings_0x21B6.clights_0x21B6 = 0;
	}
	//x_D41A0_BYTEARRAY_0[8587] = 1;//0x218b
	world_D41A0.m_GameSettings.m_Graphics.m_wShadows = 1;
	//x_D41A0_BYTEARRAY_0[8591] = 1;//0x218f
	world_D41A0.m_GameSettings.m_Display.m_wTopBar = 1;
	//x_D41A0_BYTEARRAY_0[8590] = 1;//0x218e
	world_D41A0.m_GameSettings.m_Display.m_wMiniMap = 1;
	//x_D41A0_BYTEARRAY_0[8589] = 40;//0x218d
	world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize = 40;
	//x_D41A0_BYTEARRAY_0[8599] = 0;//0x2197
	world_D41A0.m_GameSettings.display_0x2196.setting_0x2197 = 0;
	//x_D41A0_BYTEARRAY_0[8600] = 0;//0x2198
	world_D41A0.m_GameSettings.display_0x2196.transparency_0x2198 = 0;
	//x_D41A0_BYTEARRAY_0[8601] = 0;//0x2199
	world_D41A0.m_GameSettings.display_0x2196.flat_0x2199 = 0;
	//x_D41A0_BYTEARRAY_0[14] = 2;
	world_D41A0.NumberOfPlayers_0xe = 2;;
	strcpy(gameState_D41A4.netGameName_145ar, "NETHERW");
	gameState_D41A4.paletteCount_184w = 256;
	//v5 = '\0';
	gameState_D41A4.byteindex_218w = 25;
	gameState_D41A4.player_name_57ar[0] = '\0';
	v6 = gameState_D41A4.levelnumber_43w;
	v7 = gameState_D41A4.byteindex_218w;
	gameState_D41A4.setting_30 = 0;
	if (v6 >= v7)
		gameState_D41A4.levelnumber_43w = v7 - 1;
	SetTextBoxMinMaxSizes_87090();
	if (helpPopup_1804B0.initialized_0xa2)
		gameState_D41A4.showHelp_10 = 1;
	//sub_2CA90(x_D41A0_BYTEARRAY_0[8589]);//268090//0x218d
	viewPort.ResizeViewPort_2CA90(world_D41A0.m_GameSettings.m_Graphics.m_wViewPortSize);//268090//0x218d
	return 1;
}
// D0C18: using guessed type char x_BYTE_D0C18;
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// 180552: using guessed type char x_BYTE_180552;

uint32_t readdword(int8_t* a, uint32_t b)
{
	return(*(uint32_t*)&a[b]);
}

//----- (0005C3D0) --------------------------------------------------------
signed int FileDecompress_5C3D0(uint8_t* input, uint8_t* output)//23d3d0
{
	//char v3; // [esp+0h] [ebp-8h]
	//char v4; // [esp+1h] [ebp-7h]
	//char v5; // [esp+2h] [ebp-6h]
	//char v6; // [esp+3h] [ebp-5h]
	//char v7; // [esp+4h] [ebp-4h]

	char RNSSING[5] = "RNC\x1";
	//v3 = 82;
	//v4 = 78;
	//v6 = 1;
	//v5 = 67;
	//v7 = 0;
	if (strncmp((const char*)input, RNSSING, 4))
		return 0;
	DataFileRNC::Decompress(input, output);
	return 1;
}
// 99682: using guessed type x_DWORD strncmp(x_DWORD, x_DWORD, x_DWORD);

//----- (0005C490) --------------------------------------------------------
void TestersInfo_5C490()//23d490
{
	//int result; // eax
	int v1; // edx

	myprintf("\n");
	myprintf("***********************************************************\n");
	myprintf("*   TESTERS: WRITE DOWN THE ABOVE VERSION DATE AND TIME.  *\n");
	myprintf("*   Press RETURN to continue...                           *\n");
	myprintf("***********************************************************\n");
	do
	{
		if (testersInfoSize_E3BB0 <= 0 || testersInfoFlags_E3BB8 & 4 || *testersInfoPtr_E3BAC == 13 || *testersInfoPtr_E3BAC == 26)
		{
			v1 = fgetc((FILE*)&testersInfoPtr_E3BAC);
		}
		else
		{
			v1 = *testersInfoPtr_E3BAC;
			testersInfoPtr_E3BAC++;
			testersInfoSize_E3BB0--;
		}
	} while (v1 != 10);
}

//----- (0005C8D0) --------------------------------------------------------
void SetMaxDistance_5C8D0(type_entity_0x6E8E* event)//23d8d0
{
	uint32_t distance; // eax
	type_entity_0x6E8E* event2 = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	distance = Maths::Distance3D_583F0(&event->position_0x4C_76, &event2->position_0x4C_76);
	if (distance <= event2->playerData_0xA4_164x->maxDistance_0x19E_414)
		event2->playerData_0xA4_164x->maxDistance_0x19E_414 = distance;
}

//----- (0006EAB0) --------------------------------------------------------
void EndLoop_6EAB0(int  /*a1*/, __int16 a2, __int16 index)//24fab0
{
	if (!soundActive_E3799 || !soundAble_E3798)
		return;
	if ((unsigned __int16)index < 31)
	{
		if ((unsigned __int16)index < 1u || (unsigned __int16)index > 2u && index != 5)
			return;
	}
	else if ((unsigned __int16)index > 31)
	{
		if ((unsigned __int16)index >= 47 && ((unsigned __int16)index <= 0x2Fu || index == 49))
			UpdatePlayingSampleStatus_8F710(0, index, 0, 4u, 1);
		return;
	}
	if (a2 == world_D41A0.LevelIndex_0xc)
		UpdatePlayingSampleStatus_8F710(0, index, 0, 2u, 1);
}

//----- (0006EBF0) --------------------------------------------------------
void CreateFileArrayIndexes_6EBF0(filearray_struct* a1)//24FBF0
{
	for (uint32_t i = 0; (a1[i].begin_buffer) != &ZERO_BUFFER_PST2; i++)
	{
		if (videoMode_180660 & 1)
			CreateIndexDatTabHalved_99A77((uint8_t*)*a1[i].begin_buffer, (uint8_t*)*a1[i].end_buffer, *a1[i].dat_buffer, *a1[i].posistruct);
		else
			CreateIndexDatTabRelative_99AEB((uint8_t*)*a1[i].begin_buffer, (uint8_t*)*a1[i].end_buffer, *a1[i].dat_buffer, *a1[i].posistruct);
	}
	/*
  //unsigned int* *i; // ebx
  unsigned int result; // eax

  //i = (unsigned int**)malloc(30);//fix it

  for ( ; a1[0]; a1 += 3 )
  {
	if ( videoMode_180660 & 1 )
	  result = CreateIndexDatTabHalved_99A77((uint8_t*)*a1[0], *a1[1], *a1[2]);
	else
	  result = (unsigned int)CreateIndexDatTabRelative_99AEB((unsigned int *)*a1[0], *a1[1], *a1[2]);
  }
  return result;*/
}

//----- (0006EDB0) --------------------------------------------------------
void SetCenterScreenForFlyAssistant_6EDB0()//24FDB0
{
	uint32_t display_w = screenWidth_18062C, display_h = screenHeight_180624;
	uint32_t new_x, new_y;

	if (videoMode_180660 & 1) {
		// 640x480 virtual screen
		display_w = 640;
		display_h = 480;
	}

	new_x = display_w >> 1;
	new_y = display_h >> 1;

	VGA_Set_mouse(new_x, new_y);
}

//----- (0006FE20) --------------------------------------------------------
void ClearProgrammableIntervalTimer_6FE20()//fix//250e20
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	//int v0; // et1
	//int v1; // eax

	if (timerInstalled_DB734 == 1)
	{
		//v0 = gameState_D41A4.dwordindex_2380;
		/*__outx_BYTE(0x43u, 0x36u);
		__outx_BYTE(0x40u, 0);
		__outx_BYTE(0x40u, 0);
		v1 = dos_setvect(8, x_DWORD_F5330, (unsigned __int16)x_WORD_F5334);*/
	}
	timerInstalled_DB734 = 0;
}
// 99BDB: using guessed type x_DWORD dos_setvect(x_DWORD, x_DWORD, x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// DB734: using guessed type char timerInstalled_DB734;
// F5330: using guessed type int x_DWORD_F5330;
// F5334: using guessed type __int16 x_WORD_F5334;


void WriteBufferToBMP(uint16_t width, uint16_t height, uint8_t* ptrPalette, uint8_t* ptrBuffer, const char* filename)
{
	std::string path = GetSubDirectoryPath("BufferOut");
	if (myaccess(path.c_str(), 0) < 0)
	{
		std::string exepath = get_exe_path();
		mymkdir((exepath + "/" + "BufferOut").c_str());
	}

	path = GetSubDirectoryFilePath("BufferOut", "PaletteOut.bmp");
	BitmapIO::WritePaletteAsImageBMP(path.c_str(), 256, ptrPalette);
	path = GetSubDirectoryFilePath("BufferOut", filename);
	BitmapIO::WriteImageBufferAsImageBMP(path.c_str(), width, height, ptrPalette, ptrBuffer);
}

void WriteMenuGraphicToBMP(uint16_t width, uint16_t height, uint8_t scale, uint8_t* ptrPalette, uint8_t* ptrBuffer)
{
	width = width * scale;
	height = height * scale;

	std::string path = GetSubDirectoryPath("BufferOut");
	if (myaccess(path.c_str(), 0) < 0)
	{
		std::string exepath = get_exe_path();
		mymkdir((exepath + "/" + "BufferOut").c_str());
	}

	path = GetSubDirectoryFilePath("BufferOut", "PaletteOut.bmp");
	BitmapIO::WritePaletteAsImageBMP(path.c_str(), 256, ptrPalette);
	path = GetSubDirectoryFilePath("BufferOut", "MenuGraphic.bmp");

	uint8_t* ptrImage = new uint8_t[width * height];
	GameBitmap::DrawMenuGraphic(width, height, scale, ptrBuffer, ptrImage);

	BitmapIO::WriteImageBufferAsImageBMP(path.c_str(), width, height, ptrPalette, ptrImage);
	delete[] ptrImage;
}

//----- (00075AE0) --------------------------------------------------------
/*void sub_75AE0(int a1)
{
	int v2; // [esp+0h] [ebp-28h]
	int v3; // [esp+4h] [ebp-24h]
	int v4; // [esp+8h] [ebp-20h]
	int v5; // [esp+14h] [ebp-14h]
	//__int16 v6; // [esp+1Ch] [ebp-Ch]

	memset(&v6, 0, 12);
	memset(&v2, 0, 28);
	v4 = 0;
	v5 = a1;
	v2 = 0x300;
	v3 = 0x33;
	//v6 = __DS__;
	//return int386x(0x31, (REGS*)&v2, (REGS*)&v2, (SREGS*)&v6);//mozna neco s mysi
}*/
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 99E10: using guessed type x_DWORD int386x(x_DWORD, x_DWORD, x_DWORD, x_DWORD);

//----- (00075B50) --------------------------------------------------------
// NOTE: void sub_75B50(__int16 a1) removed as it did DOS memory handling

//----- (00075B80) --------------------------------------------------------
int AllocMemBlock_75B80(int a1, x_WORD* a2, x_WORD* a3)//see: https://github.com/videogamepreservation/descent2/blob/master/SOURCE/BIOS/DPMI.C
{
	int v3; // ebx
	int v5; // [esp+0h] [ebp-28h]
	//unsigned int v6; // [esp+4h] [ebp-24h]
	int v7; // [esp+Ch] [ebp-1Ch]
	int v8; // [esp+18h] [ebp-10h]
	//char v9; // [esp+1Ch] [ebp-Ch]

	//fix it
	v7 = 0;
	v8 = 0;
	//fix it

	*a2 = 0;
	*a3 = 0;
	//segread((SREGS*)&v9);
	v3 = 0;
	//v6 = (unsigned int)(a1 + 15) >> 4;
	v5 = 0x100;
	//removed int386(0x31, (REGS*)&v5, (REGS*)&v5);//allocate memory block //dpmi_real_malloc
	if (!v8)
	{
		*a2 = v5;
		*a3 = v7;
		v3 = 16 * (signed __int16)*a2;
	}
	return v3;
}
// 99DE6: using guessed type x_DWORD segread(x_DWORD);

//----- (00083B50) --------------------------------------------------------
void InitMemoryAndTextureSize_83B50()//264B50
{
	//int result; // eax
	//int v1; // edx
	//int v2; // edx
	//int v3; // eax
	//int v4; // ST04_4

	//result = x_D41A0_BYTEARRAY_4;//2A51A4d == ff //fix
	if (!(gameState_D41A4.pointer_0xE2_heapbuffer_226))//2A51B2F = ff
	{
		TileTextureSize_D41B5 = 32;
		/*if ( (unsigned int)x_DWORD_17FEAC <= 0x800000 )//fix
		{
		  if ( (unsigned int)x_DWORD_17FEAC <= 0x700000 )
		  {
			v1 = x_DWORD_17FEAC - 2398576;
			if ( (unsigned int)x_DWORD_17FEAC <= 0x600000 )
			{
			  if ( (unsigned int)x_DWORD_17FEAC <= 0x500000 )
			  {
				if ( (unsigned int)x_DWORD_17FEAC <= 0x400000 )
				{
				  if ( (unsigned int)x_DWORD_17FEAC <= 0x300000 )
				  {
					gameState_D41A4.dword_0xE6_heapsize_230 = 0x200000;
					x_BYTE_D41B5 = 16;
				  }
				  else
				  {
					gameState_D41A4.dword_0xE6_heapsize_230 = x_DWORD_17FEAC - 1398576;
				  }
				  sub_9A230_set_x_WORD_E37B4(811);
				}
				else
				{
				  gameState_D41A4.dword_0xE6_heapsize_230 = v1;
				  sub_9A230_set_x_WORD_E37B4(811);
				}
			  }
			  else
			  {
				gameState_D41A4.dword_0xE6_heapsize_230 = v1;
				sub_9A230_set_x_WORD_E37B4(822);
			  }
			}
			else
			{
			  gameState_D41A4.dword_0xE6_heapsize_230 = v1;
			  sub_9A230_set_x_WORD_E37B4(822);
			}
			*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8628) = 0;
		  }
		  else
		  {
			gameState_D41A4.dword_0xE6_heapsize_230 = x_DWORD_17FEAC - 3398576;
			sub_9A230_set_x_WORD_E37B4(822);
		  }
		}
		else*/
		{
			gameState_D41A4.dword_0xE6_heapsize_230 = 0x400000;
			//fix
			gameState_D41A4.dword_0xE6_heapsize_230 += 0x1000000;
			//fix
			SetSoundFreq_9A230(822);
		}
		//v2 = (int)sub_83CD0_malloc2(gameState_D41A4.dword_0xE6_heapsize_230);
		//v3 = x_D41A0_BYTEARRAY_4;
		gameState_D41A4.pointer_0xE2_heapbuffer_226 = (uint8_t*)Malloc_83CD0(gameState_D41A4.dword_0xE6_heapsize_230);
		if (!gameState_D41A4.pointer_0xE2_heapbuffer_226)
		{
			//v4 = x_D41A0_BYTEARRAY_4[230]; //fix
			myprintf("ERROR 1: NOT ENOUGH MEMORY. (%ld / %ld)\n");
			exit(1);
		}
		LoadPsxBlock_54630(TileTextureSize_D41B5);
		PrepareTextures_5B7A0();//23C7A0
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// D41B5: using guessed type char x_BYTE_D41B5;
// 17FEAC: using guessed type int x_DWORD_17FEAC;

void GetSystemSpeed()
{
	//Just assume High spec for now
	gameState_D41A4.m_wHighSpeedSystem = 5;
}

//----- (00085060) --------------------------------------------------------
int ReadDwordAtOffset4_85060(int a1)//266060
{
	return *(x_DWORD*)(a1 + 4);
}

//----- (00085EB0) --------------------------------------------------------
void* AllocMemory_85EB0(int32 a1)//266eb0 //malloc
{
	/*
#ifdef TEST_x64
	allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
  return 0;
#else
	return (int)malloc(a1 * 16 * sizeof(uint8_t));
#endif
	//return (uint8_t*)malloc(a1*sizeof(uint8_t));
	*/
	return malloc(a1 * 16 * sizeof(uint8_t));
}
// 17FF0C: using guessed type int Int386Request_17FF0C;
// 17FF10: using guessed type int x_DWORD_17FF10;
// 17FF24: using guessed type int cdBuffer_17FF24;

//----- (00085F00) --------------------------------------------------------
__int16 FreeMemory_85F00(__int16  /*a1*/)//266f00 //https://github.com/videogamepreservation/descent2/blob/master/SOURCE/BIOS/DPMI.C
{
	/*__int16 result; // ax

	x_WORD_17FF18 = a1;
	LOWORD(Int386Request_17FF0C) = 0x100;
//removed int386(49, (REGS*)&Int386Request_17FF0C, (REGS*)&Int386Request_17FF0C);
	if ( cdBuffer_17FF24 )
	  myprintf("fdm:error freeing %lx\n");
	LOBYTE(result) = cdBuffer_17FF24 == 0;
	HIBYTE(result) = 0;
	return result;*/

	//__int16 result; // ax
	free((void*)cdBuffer_17FF24);
	if (cdBuffer_17FF24)
	{
		myprintf("fdm:error freeing %lx\n");
		return 1;
	}
	return 0;
}
// 17FF0C: using guessed type int Int386Request_17FF0C;
// 17FF18: using guessed type __int16 x_WORD_17FF18;
// 17FF24: using guessed type int cdBuffer_17FF24;

//----- (00089B60) --------------------------------------------------------
int16_t ApplySetting_89B60(uint8_t a1)//26ab60
{
	int16_t v1; // esi
	__int16 v2; // ax
	bool v3; // zf

	v1 = -1;
	inputState_18058C.joyCenterZ_1805AC = 0;
	inputState_18058C.joyCenterY_1805A8 = 0;
	inputState_18058C.joyCenterX_1805A4 = 0;
	inputState_18058C.joyAxisZ_180598 = 0;
	inputState_18058C.joyAxisY_180594 = 0;
	inputState_18058C.joyAxisX_180590 = 0;
	inputState_18058C.x_DWORD_1805A0 = 0;
	inputState_18058C.MouseButtonState_18059C = 0;
	inputState_18058C.x_WORD_1805C8 = 0;
	switch (a1)
	{
	case 1u:
		// i_Glasses (Virtual I-O) joystick
		v1 = InitJoystickType3_8B600(inputState_18058C);//fix it
		if ((signed __int16)v1 != -1)
			goto LABEL_3;
		break;
	case 2u:
		// VFX1 CyberPuck joystick
		if (DetectVrDevice_75650())//fix it
		{
			v1 = 1;
			myprintf("VFX INITIALISED\n");
			inputState_18058C.x_WORD_1805C8 = 1;
			v2 = a1;
			inputState_18058C.x_DWORD_1805A0 = 0;
			inputState_18058C.MouseButtonState_18059C = 0;
			goto LABEL_12;
		}
		break;
	case 4u:
		memset(unk_180560x, 0, 44);//fix it
		InitJoystickState_8BB40(unk_180560x, 8);
		v3 = unk_180560x[24] == 0;
		goto LABEL_10;
	case 6u:
		memset(unk_180560x, 0, 44);//fix it
		v1 = 1;
		v2 = a1;
		goto LABEL_12;
	case 7u:
		v2 = a1;
		v1 = 1;
		goto LABEL_12;
	case 8u:
	case 9u:
	LABEL_3:
		inputState_18058C.x_WORD_1805C8 = 1;
		break;
	case 0xBu:
		memset(unk_180560x, 0, 44);//fix it
		InitJoystickState_8BB40(unk_180560x, 4);
		v3 = unk_180560x[24] == 0;
	LABEL_10:
		if (!v3)//fix it
		{
			ReadJoystick_8BBE0(unk_180560x);
			v1 = 1;
			v2 = a1;
		LABEL_12:
			inputState_18058C.joystickMode_1805C2 = v2;
		}
		break;
	default:
		break;
	}
	inputState_18058C.joystickMode_1805C2 = a1;
	if ((signed __int16)v1 > 0)
		joystickEnabled_E36D1 = a1;
	return v1;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// E36D1: using guessed type char joystickEnabled_E36D1;
// 180584: using guessed type char x_BYTE_180584;
// 180590: using guessed type int x_DWORD_180590;
// 180594: using guessed type int x_DWORD_180594;
// 180598: using guessed type int x_DWORD_180598;
// 18059C: using guessed type int MouseButtonState_18059C;
// 1805A0: using guessed type int x_DWORD_1805A0;
// 1805A4: using guessed type int x_DWORD_1805A4;
// 1805A8: using guessed type int x_DWORD_1805A8;
// 1805AC: using guessed type int x_DWORD_1805AC;
// 1805C2: using guessed type __int16 x_WORD_1805C2_joystick;
// 1805C8: using guessed type __int16 x_WORD_1805C8;

//----- (00090EA0) --------------------------------------------------------
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// E39A0: using guessed type char x_BYTE_E39A0;
// 181BE4: using guessed type int x_DWORD_181BE4_number_of_disc;

//----- (00090EE0) --------------------------------------------------------
// 8C1CA: using guessed type x_DWORD dos_setdrive(x_DWORD, x_DWORD);
// A0DA5: using guessed type x_DWORD chdir(x_DWORD);
// 181BE4: using guessed type int x_DWORD_181BE4_number_of_disc;

//----- (00090F20) --------------------------------------------------------
// 8C1CA: using guessed type x_DWORD dos_setdrive(x_DWORD, x_DWORD);
// A0DA5: using guessed type x_DWORD chdir(x_DWORD);
// 181BE0: using guessed type int x_DWORD_181BE0;

// 8C1B2: using guessed type x_DWORD dos_getdrive(x_DWORD);
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// A0DC7: using guessed type x_DWORD getcwd(x_DWORD, x_DWORD);
// A0E6B: using guessed type x_DWORD free(x_DWORD);
// E39A0: using guessed type char x_BYTE_E39A0;
// 181BE0: using guessed type int x_DWORD_181BE0;
// 181BE4: using guessed type int x_DWORD_181BE4_number_of_disc;


//----- (00091F50) --------------------------------------------------------
void RestoreInterruptFlags_91F50(unsigned int a1)
{
	SetEflags_A1524(a1);
}

//----- (00099FF0) --------------------------------------------------------
signed int StringToLong2_99FF0(char* a1, char** a2, signed int a3)//27aff0
{
	return StringToLong_99E8E(a1, a2, a3, 1);
}

//----- (0009AD16) --------------------------------------------------------
int FreeMemPool_9AD16(void* a1)//27Bd16
{
	free(a1);
	return 1;
}

//----- (000AB610) --------------------------------------------------------
/*int sub_AB610(int a1, x_DWORD *a2)//28c610
{
	x_DWORD *v2; // ebx
	int v3; // ST00_4
	int result; // eax
	int v5; // ecx
	int v6; // edx
	int v7; // esi
	int v8; // edi
	int v9; // ebp
	int v10; // ebx

	v2 = a2;
	v3 = a2[7];
	result = *a2;
	v5 = a2[2];
	v6 = a2[3];
	v7 = v2[5];
	v8 = v2[6];
	v9 = v2[4];
	__ES__ = *((x_WORD *)v2 + 15);
	__FS__ = *((x_WORD *)v2 + 16);
	__GS__ = *((x_WORD *)v2 + 17);
	v10 = v2[1];
	return result;
}*/

//----- (000AC24B) --------------------------------------------------------
int ReturnZero_AC24B()//28d24b
{
	return 0;
}

//----- (000AD09E) --------------------------------------------------------
x_BYTE* ScanfParseCharset_AD09E(x_BYTE* a1, int a2)//28e09e
{
	int v2; // eax
	x_BYTE* v3; // ebx

	memset((void*)a2, 0, 32);
	v2 = (unsigned __int8)*a1;
	v3 = a1 + 1;
	if (*a1)
	{
		do
		{
			*(x_BYTE*)((v2 >> 3) + a2) |= bitMasks_D3EA0[v2 & 7];
			v2 = (unsigned __int8)*v3;
			if (!*v3)
				break;
			++v3;
		} while (v2 != 93);
	}
	return v3;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);

//----- (000AD754) --------------------------------------------------------
int HexCharToDigit_AD754(signed int a1)//28e754
{
	int result; // eax
	signed int v2; // eax

	if (a1 >= 48 && a1 <= 57)
		return a1 - 48;
	v2 = x_tolower(a1);
	if (v2 < 97 || v2 > 102)
		result = 16;
	else
		result = v2 - 87;
	return result;
}
// AB952: using guessed type x_DWORD x_tolower(x_DWORD);

#pragma endregion
