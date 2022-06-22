//this lib can be delete in future
#ifndef AIL_CONFIG
#define AIL_CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <direct.h>  
//#include <dir.h>
#include <io.h>  

#include "../portability/port_outputs.h"

//#include <sys/statvfs.h>
#include <windows.h>
#include "../portability/dirent.h"
#include "../portability/port_time.h"
#include "../portability/port_filesystem.h"
#include "../portability/port_sdl_sound.h"
#include "ail_sound.h"
#include "defs.h"
#define __CFSHL__(x, y) (x<<y)
//myprintf("invalid_operation:__CFSHL__(x, y)")
#define __RCR__(x, y) printf("invalid_operation:__RCR__(x, y)")
#define __RCL__(x, y) printf("invalid_operation:__RCL__(x, y)")
#define __ROL4__(x, y) __ROL__(x, y)       // Rotate left
#define __ROR4__(x, y) __ROR__(x, y)       // Rotate left

#include "sub_main_mouse.h"

#include "cd_utils.h"

int x_DWORD_E3E9C[] = { 0 }; // weak
int x_DWORD_E3ED8 = 0; // weak
int x_DWORD_E3EDC[] = { 0 }; // weak
int x_DWORD_E3F1C[] = { 0 }; // weak
int x_DWORD_E3F5C[] = { 0 }; // weak
int x_DWORD_E3F98 = 0; // weak
int x_DWORD_E3F9C[] = { 0 }; // weak
int x_DWORD_E3FDC = 0; // weak
__int16 x_WORD_E3FE0 = 0; // weak
int x_DWORD_E3FE2 = 0; // weak
int x_DWORD_E3FE6 = 0; // weak

int x_DWORD_E3FEA = 0; // weak
int x_DWORD_E3FEE = 0; // weak

int x_DWORD_E3FF2 = 0; // weak
__int16 x_WORD_E3FF6 = 0; // weak
int x_DWORD_E3FF8 = 0; // weak


int x_DWORD_181C04 = 0; // weak 352c04
int x_DWORD_181BF4 = 0; // weak//352bf4
int x_DWORD_181BF8; // weak


char sub_A102C(int a1);
char sub_A105C(unsigned int a1);
void sub_A108F();

int x_DWORD_A128C = 2576980377; // weak

_UNKNOWN unk_E4004; // weak

__int16 x_WORD_A1286 = 39321; // weak
__int16 x_WORD_A1277 = 39321; // weak

__int16 x_WORD_A12A2 = 39321; // weak
__int16 x_WORD_A12AC = 39321; // weak

__int16 x_WORD_A12B3 = 39321; // weak

x_DWORD loc_9B94B;
void *off_9B844 = &loc_9B94B; // weak
// extern _UNKNOWN loc_A0000_vga_buffer; weak
int x_DWORD_A1270 = 1397753374; // weak

int x_DWORD_E3FFC = 0; // weak
int x_DWORD_E4000 = 0; // weak

AILTIMERCB x_DWORD_E3E5C_timer[] = { 0 }; // weak

FILE* x_DWORD_181BF0_AIL_debugfile; // weak

int x_DWORD_E39BC = 0; // weak

int x_DWORD_181BFC; // weak
int x_DWORD_181C00; // weak


int x_DWORD_181C08; // weak
int x_DWORD_181C0C; // weak
int x_DWORD_181C10; // weak

char IsTable[256] =
{
  '\x00','\x01','\x01','\x01','\x01','\x01','\x01','\x01',
  '\x01','\x01','\x03','\x03','\x03','\x03','\x03','\x01',
  '\x01','\x01','\x01','\x01','\x01','\x01','\x01','\x01',
  '\x01','\x01','\x01','\x01','\x01','\x01','\x01','\x01',
  '\x01','\n','\f','\f','\f','\f','\f','\f',
  '\f','\f','\f','\f','\f','\f','\f','\f',
  '\f','8','8','8','8','8','8','8',
  '8','8','8','\f','\f','\f','\f','\f',
  '\f','\f','X','X','X','X','X','X',
  'H','H','H','H','H','H','H','H',
  'H','H','H','H','H','H','H','H',
  'H','H','H','H','\f','\f','\f',
  '\f','\f','\f','\x98','\x98','\x98',
  '\x98','\x98','\x98','\x88','\x88','\x88',
  '\x88','\x88','\x88','\x88','\x88',
  '\x88','\x88',
  '\x88',
  '\x88',
  '\x88',
  '\x88',
  '\x88',
  '\x88',
  '\x88',
  '\x88',
  '\x88',
  '\x88',
  '\f',
  '\f',
  '\f',
  '\f',
  '\x01',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0',
  '\0'
}; // weak
char x_BYTE_D3450[17] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',0 }; // weak
char x_BYTE_181C90[513]; // weak
int x_DWORD_181DAC[19]; // fix it -  weak

const char* dword_E4CC4[7] = { "PATH=Z:\\",
"COMSPEC=Z:\\COMMAND.COM",
"PROMPT=$P$G",
"BLASTER=A220 I7 D1 H5 T6",
"AIL_DEBUG=sound.dbg",//debug
"AIL_SYS_DEBUG=soundd.dbg",//debug
NULL };

IO_PARMS unk_181DF8; // weak
int x_DWORD_E3E3C = 0; // weak

int x_DWORD_E3E38 = 0; // weak
int x_DWORD_181DA8; // weak
IO_PARMS unk_181D90; // weak
const char* unk_D226C = " \0"; // weak
const char* unk_D2270 = "\0xfa\0"; // weak

int x_DWORD_E4A8C = 0; // weak
int x_DWORD_E4A90 = 0; // weak
int x_DWORD_E4A94 = 0; // weak
char aSample[7] = "SAMPLE"; // weak
int x_DWORD_E4B18 = 0; // weak
int x_DWORD_E4B1C = 0; // weak
char aTimb_0[5] = "TIMB"; // weak
char x_BYTE_E4B2A = '\xFF'; // weak
char x_BYTE_E4B2B = '\xFF'; // weak
int x_DWORD_E4B2C = 0; // weak
int x_DWORD_E4C90 = 0; // weak
int x_DWORD_E4C94 = 0; // weak
int x_DWORD_E4CA4 = 0; // weak

__int16 x_WORD_E4A04 = 0; // weak
int x_DWORD_E4A08 = 0; // weak
Bit8u x_BYTE_E4A0C[128] =
{
  0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,
  0x10,0x12,0x14,0x16,0x18,0x1A,0x1C,0x1E,
  0x20,0x22,0x24,0x26,0x28,0x2A,0x2C,0x2E,
  0x30,0x32,0x34,0x36,0x38,0x3A,0x3C,0x3E,
  0x40,0x42,0x44,0x46,0x48,0x4A,0x4C,0x4E,
  0x50,0x52,0x54,0x56,0x58,0x5A,0x5C,0x5E,
  0x60,0x62,0x64,0x66,0x68,0x6A,0x6C,0x6E,
  0x70,0x72,0x74,0x76,0x78,0x7A,0x7C,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80
}; // idb

HSAMPLE x_DWORD_181E10; // weak
FILE* x_DWORD_181E20; // weak
int x_DWORD_181E26; // weak
__int16 x_WORD_181E2A; // weak
int x_DWORD_181E2C; // weak
__int16 x_WORD_181E30; // weak
_UNKNOWN unk_181E80; // weak
int x_DWORD_181E8C; // weak
int x_DWORD_181E90; // weak
int x_DWORD_181E94; // weak
int x_DWORD_181E98; // weak
int x_DWORD_181E9C; // weak
int x_DWORD_181EA0; // weak
int x_DWORD_181EB0; // weak
int x_DWORD_181EB4; // weak
HSEQUENCE x_DWORD_181EB8; // weak
int x_DWORD_181EBC; // weak
int x_DWORD_181EC0; // weak
int x_DWORD_181EC4; // weak
int x_DWORD_181EC8; // weak
int x_DWORD_181ECC; // weak
int x_DWORD_181ED0; // weak
int x_DWORD_181ED4; // weak
int x_DWORD_181ED8; // weak
int x_DWORD_181EDC; // weak
_UNKNOWN unk_181EE0; // weak
signed int x_DWORD_182188[6]; // idb
signed int x_DWORD_1821A0[6]; // idb
signed int x_DWORD_1821B8[6]; // idb
signed int x_DWORD_1821D0[6]; // idb
signed int x_DWORD_1821E8[6]; // idb
_UNKNOWN unk_182200; // weak
_UNKNOWN unk_182204; // weak

void stub_fix_it();
int sub_AEF40();
int sub_AEF51();

int sub_AFB7E();
int sub_AFB9F();

int(*off_AEB40[2])() = { &sub_AEF40, &sub_AEF51 }; // weak
int(*off_AED40[2])() = { &sub_AFB7E, &sub_AFB9F }; // weak

int nullsub_3(x_DWORD number);
int _no_support_loaded(x_DWORD number1, x_DWORD number2);

char x_BYTE_E4CB4 = '\0'; // weak
int(/*__fastcall*/ *off_E4CBD)(x_DWORD) = &nullsub_3; // weak
int x_DWORD_E4CCC = 0; // weak
int x_DWORD_E4CD0 = 0; // weak
int x_DWORD_E4D10 = 0; // weak
__int16 x_WORD_E4D14 = 0; // weak
__int16 x_WORD_E4D16 = 0; // weak
char x_BYTE_E4D18 = '\0'; // weak
int(*off_E4D20)(x_DWORD, x_DWORD) = &_no_support_loaded; // weak
int x_DWORD_E4E1C = 0; // weak
int x_DWORD_E4E20 = 0; // weak
int x_DWORD_E4E24 = 0; // weak
int x_DWORD_E4E28 = 0; // weak
int x_DWORD_E4E2C = 0; // weak
__int64 qx_WORD_E4E30 = 0i64; // weak
int x_DWORD_E4E38 = 0; // weak
__int16 x_WORD_E4E3C = 0; // weak

Bit8u* x_DWORD_E3808_music_header = 0; // weak
Bit8u* x_DWORD_E380C = 0; // weak
Bit8u* x_DWORD_E3810_music_data = 0; // weak

char x_BYTE_E3E54[6] = { '\0', '\0', '\0', '\0', '\0', '\0' }; // weak


char* mygetenv(const char *a1);

Bit8u* sub_9D450_malloc4(int a1);
int sub_9D490_free4(void* a1, int a2);

char x_toupper(char inputchar);

void sub_9E1B0();
void sub_9E250(Bit32u user); // weak
int sub_9E2B0(char* a1, int a2, x_DWORD *a3);
//int sub_9E3A0_AIL_API_read_INI(Bit8u* a1, char* a2);
int sub_9E3A0_AIL_API_read_INI(AIL_INI* INI, char* filename/*, char* a8, IO_PARMS* IO*/);
int sub_9E6E0_set_preference(Bit32u number, Bit32s value);
// x_DWORD *sub_9E720_AIL_API_install_driver(int a1, int a2, int a3);
AIL_DRIVER* sub_9E720_AIL_API_install_driver(/*int a1, */Bit8u* driver_image, Bit32s n_bytes);//27f720
int sub_9EA60_AIL_API_uninstall_driver(AIL_DRIVER* a1);
IO_PARMS* sub_9EB60_AIL_API_get_IO_environment(AIL_DRIVER *drvr);
void sub_9EC30();
signed int sub_9ED70();

void sub_915A0_sound_proc23();
signed int sub_916F0_sound_proc24();
signed int sub_919C0();

void sub_9D590_lock_linear_region(Bit8u* a1, unsigned int a2);
bool sub_986A0_unlock_linear_region();

bool sub_9D650(unsigned int a1, unsigned int a2);
void sub_9D710(Bit8u* a1, int a2);
bool sub_9D740(unsigned int a1, int a2);

signed int sub_9D4D0(int a1, Bit8u* a2, Bit32u* a3, Bit32u* a4);
void* sub_83D70_malloc1(int a1);
void sub_9D560(Bit8u* a1);
void sub_83E80_freemem4(Bit8u* a1);
AIL_DRIVER* x_DWORD_181C50[16]; // idb

void sub_A10F4_sound_proc_irq();
signed int sub_A11E2();
void sub_A1249_AIL_API_set_real_vect(Bit32u vectnum, Bit16u real_ptr);
signed int sub_A12C5_sound_proc_irq(int a1, int a2, __int16 a3);
void sub_A14DB_AIL_API_restore_USE16_ISR(int a1);
unsigned int sub_A1520();
void sub_A1524(unsigned int a1);
//int sub_A158B_AIL_API_call_driver(Bit32s* drvr, __int16 a2, x_WORD *a3, x_WORD *a4);
Bit32s sub_A158B_AIL_API_call_driver(AIL_DRIVER* drvr, Bit32s fn, VDI_CALL* in, VDI_CALL* out);
void sub_A1665_AIL_API_delay(int a1);
int sub_A16A2();
//unsigned int sub_A16AE_register_timer(int a1);
HTIMER sub_A16AE_register_timer(AILTIMERCB callback_fn);
signed __int32 sub_A16F5_AIL_API_set_timer_user(int a1, signed __int32 a2);
void sub_A171D_release_timer_handle(Bit32s a1);
void sub_A1744_AIL_API_release_all_timers();
void sub_A1768_start_timer(HTIMER timer);
void sub_A1798();
void sub_A17BC_stop_timer(HTIMER timer);
void sub_A17EC();
//void sub_A1810_set_timer_period(int a1, int a2);
void sub_A1810_set_timer_period(HTIMER timer, Bit32u microseconds);
//void sub_A1840_AIL_API_set_timer_frequency(int a1, unsigned int a2);
void sub_A1840_AIL_API_set_timer_frequency(HTIMER timer, Bit32u hertz);


int sub_91A80_AIL_set_preference(Bit32u number, Bit32s value);
void sub_91BD0_s_plus();
void sub_91BF0_s_minus();
Bit16u sub_A121D_AIL_API_get_real_vect(Bit32u vectnum/*int a1, __int16 a2, int a3, int a4*/);
Bit16u sub_91C10_AIL_get_real_vect(/*VDI_HDR* a1, int a2, int a3,*/ Bit32u vectnum/*U32       vectnum*/);//AIL_get_real_vect
void sub_91D50_AIL_set_real_vect(Bit32u vectnum, Bit16u real_ptr);
void sub_91E90_AIL_restore_USE16_ISR(Bit32s a1);


//int sub_91F70_AIL_call_driver(Bit32s* a1, int a2, x_WORD *a3, x_WORD *a4);
void sub_920D0_AIL_delay(int a1);
int sub_92160();
int sub_92190_AIL_read_INI(AIL_INI* INI, char* filename/*, char* a8, IO_PARMS IO*/);
HTIMER sub_92600_AIL_register_timer(AILTIMERCB callback_fn);
signed __int32 sub_92740_AIL_set_timer_user(int a1, signed __int32 a2);
//void sub_92890_AIL_set_timer_period(int a1, int a2);
void sub_92890_AIL_set_timer_period(HTIMER timer, Bit32u microseconds);
//void sub_92930_AIL_set_timer_frequency(int a1, unsigned int a2);
void sub_92930_AIL_set_timer_frequency(HTIMER timer, Bit32u hertz);
void sub_92BA0_AIL_start_timer(HTIMER a1);
void sub_92CB0_AIL_stop_timer(HTIMER a1);
void sub_92DC0_AIL_release_timer_handle(Bit32s a1);
void sub_92E50_AIL_release_all_timers();
IO_PARMS* sub_92ED0_AIL_get_IO_environment(AIL_DRIVER* drvr);
// x_DWORD *sub_93010_AIL_install_driver(int a1, int a2, int a3);
int sub_93160_AIL_uninstall_driver(AIL_DRIVER* a1);
// int sub_931F0_AIL_install_DIG_INI(int a1, int **a2);
// int *sub_93330_AIL_install_DIG_driver_file_AIL_install_DIG_driver_file(int a1, int a2, const void *a3);
int sub_93480_AIL_uninstall_DIG_driver(unsigned int *a1);
HSAMPLE sub_93510_AIL_allocate_sample_handle(HDIGDRIVER a1);
int sub_937A0_AIL_release_sample_handle(int a1);
void sub_93830_AIL_init_sample(HSAMPLE a1);
//signed int sub_938C0_AIL_set_sample_file(x_DWORD *a1, int a2, int a3);
Bit32s sub_938C0_AIL_set_sample_file(HSAMPLE S, Bit8u* file_image, Bit32s block);
void sub_93A10_AIL_set_sample_address(HSAMPLE S, Bit8u* start, Bit32u len);
void sub_93AB0_AIL_set_sample_type(x_DWORD *a1, int a2, int a3);
void sub_93B50_AIL_start_sample(HSAMPLE S);
void sub_93D00_AIL_end_sample(HSAMPLE S);
void sub_93D90_AIL_set_sample_playback_rate(HSAMPLE S, Bit32s playback_rate);
void sub_93E30_AIL_set_sample_volume(HSAMPLE S, Bit32s volume);
void sub_93ED0_AIL_set_sample_volume_pan(HSAMPLE S, Bit32s volume);
void sub_93F70_AIL_set_sample_loop_count(HSAMPLE S, int loop_count);
Bit32u sub_94010_AIL_sample_status(HSAMPLE S);
void sub_94650_AIL_set_digital_master_volume(x_DWORD *a1, int a2);
EOS_callback_type sub_95140_AIL_register_EOS_callback(HSAMPLE a1, EOS_callback_type a2);
void sub_953E0_AIL_set_sample_user_data(HSAMPLE S, int a2, int a3);
int sub_95480_AIL_sample_user_data(HSAMPLE S, int a2);
// int sub_95710_AIL_install_MDI_INI(int a1, int **a2);
// int *sub_95850_AIL_install_MDI_driver_file(int a1, int a2, const void *a3);
int sub_959A0_AIL_uninstall_MDI_driver(HMDIDRIVER  mdi);
HSEQUENCE sub_95A30_AIL_allocate_sequence_handle(HMDIDRIVER mdi);
Bit32s sub_95C00_AIL_init_sequence(HSEQUENCE S, Bit8u* start, Bit32s sequence_num, Bit32u track);
void sub_95D50_AIL_start_sequence(HSEQUENCE S, Bit32u track);
void sub_95DE0_AIL_stop_sequence(HSEQUENCE S);
void sub_95E70_AIL_resume_sequence(HSEQUENCE S);
void sub_95F00_AIL_end_sequence(HSEQUENCE S);
void sub_96030_AIL_set_sequence_volume(HSEQUENCE S, Bit32s volume, Bit32s milliseconds);
int sub_96170_AIL_sequence_status(HSEQUENCE S);
void sub_96670_AIL_set_XMIDI_master_volume(HMDIDRIVER mdi, Bit32s master_volume);
Bit32s sub_969A0_AIL_MDI_driver_type(HMDIDRIVER mdi);
void sub_96AE0_AIL_set_GTL_filename_prefix(Bit8u* a1);
int sub_97330_AIL_sequence_position(x_DWORD *a1, x_DWORD *a2, x_DWORD *a3);
x_DWORD *sub_97480_AIL_branch_index(HSEQUENCE S, int a2);
xCALLBACK sub_97670_AIL_register_trigger_callback(HSEQUENCE S, xCALLBACK a2);
int sub_97A60_AIL_register_event_callback(int a1, int a2);
int sub_97BB0_AIL_register_timbre_callback(int a1, int a2);
int sub_97F90_AIL_lock_channel(x_DWORD *a1);
int sub_980D0_AIL_release_channel(x_DWORD *a1, int a2);
int sub_98170_AIL_map_sequence_channel(x_DWORD *a1, int a2, int a3);
void sub_98360_AIL_send_channel_voice_message(HMDIDRIVER mdi, HSEQUENCE S, Bit32s status, Bit32s data_1, Bit32s data_2);

void sub_A1870(int a1, unsigned int a2);
int sub_A18B2();
void sub_A18BE();
bool sub_9EDD0();

void sub_A0EEC_s_plus2();
void sub_A0EF9_s_minus2();

signed int sub_9DE20_get_file_lenght(char* a1);
Bit8u* sub_9DEA0_read_file(char* a1, Bit8u* a2);

HDIGDRIVER sub_A2EA0(AIL_DRIVER* a1, IO_PARMS IO);




// x_DWORD fprintf(x_DWORD, const char *, ...); weak
// x_DWORD fopen(x_DWORD, x_DWORD); weak
// x_DWORD fclose(x_DWORD); weak
void sub_A1F90();
void sub_A2070(HDIGDRIVER a1);
void sub_A20D0(HDIGDRIVER a1);
void sub_A2110(HSAMPLE S);
void sub_A2450(Bit32u user); // weak
int sub_A2650(HDIGDRIVER a1);
int sub_A2C80(HDIGDRIVER a1, IO_PARMS* a2);
int sub_A2DE0(); // weak

// int *sub_A3600_AIL_API_install_DIG_driver_file(int a1, int a2, const void *a3);
// int *sub_A36B0(int a1, int a2, int a3, const void *a4);
// int sub_A3720_AIL_API_install_DIG_INI(int a1, int **a2);
int sub_A37A0(unsigned int *a1);
int sub_A37C0(int a1);
HSAMPLE sub_A3820_allocate_sample_handle(HDIGDRIVER dig);
int sub_A38C0_AIL_API_release_sample_handle(int a1);
void sub_A38E0_init_sample(HSAMPLE S);
Bit32u sub_A3A00_sample_status(HSAMPLE S);
void sub_A3A30_set_sample_address(HSAMPLE S, Bit8u* start, Bit32u len);
void sub_A3A70_set_sample_type(HSAMPLE S, Bit32s a2, Bit32u a3);
int sub_A3AC0(int a1);
void sub_A3AF0_set_sample_playback_rate(HSAMPLE S, Bit32s playback_rate);
Bit32s sub_A3B10_get_sample_volume(HSAMPLE S);
void sub_A3B40_set_sample_volume(HSAMPLE S, Bit32s volume);
Bit32s sub_A3B80_get_sample_volume_pan(HSAMPLE S);
void sub_A3BB0_set_sample_volume_pan(HSAMPLE S, Bit32s volume);
int sub_A3BF0(HSAMPLE S);
int sub_A3C30(HSAMPLE S, int a2);
signed int sub_A3C60_get_sample_loop_count(HSAMPLE S);
void sub_A3C90_set_sample_loop_count(HSAMPLE S, Bit32s loop_count);
void sub_A3CB0_API_AIL_start_sample(HSAMPLE S);
void sub_A3D30(HSAMPLE S);
void sub_A3D60(HSAMPLE S);
void sub_A3DA0__API_AIL_end_sample(HSAMPLE S);
int sub_A3E10(HSAMPLE S, int a2);
int sub_A3E60(HSAMPLE S, int a2);
EOS_callback_type sub_A3EB0_register_EOS_callback(HSAMPLE S, EOS_callback_type EOS);
void sub_A3F00_set_sample_user_data(HSAMPLE S, int a2, int a3);
int sub_A3F30_sample_user_data(HSAMPLE S, int index);
void sub_A3F70_set_digital_master_volume(x_DWORD *a1, int a2);

int sub_A3FE0(int a1);
char sub_A4000(x_DWORD *a1, int a2, unsigned int a3);
void sub_A4100(int *a1, int a2, int a3, int a4);
signed int sub_A41B0(x_DWORD *a1);
bool sub_A4260();
void sub_A4330();
int sub_A4370(x_DWORD *a1);
int sub_A4390(int a1);
void sub_A43E0(int a1);
int sub_A47A0(); // weak
void sub_A47C0_sub_set_sample_file(Bit8u* file_image, HSAMPLE S);
int sub_A4920(); // weak
HSAMPLE sub_A4970(int a1, Bit8u* a2, int a3);
Bit32s sub_A4B20_set_sample_file(HSAMPLE S, Bit8u* file_image, Bit32s block);
int sub_A4C60(int a1, int a2);
bool sub_A4CB0();
void sub_A4CF0();
signed int sub_A4E10(char a1);
void sub_A4EB0(HMDIDRIVER a1);
void sub_A4F10(HMDIDRIVER a1, char a2, char a3, char a4);
void sub_A4FD0(int a1, int a2, unsigned int a3);
int sub_A5040(x_DWORD *a1);
unsigned int sub_A50A0(int a1);
Bit8u* sub_A50F0(Bit8u* a1, int a2);
int sub_A5210(int a1, char a2, unsigned int a3);
x_DWORD *sub_A5530(int a1, char a2, unsigned int a3, unsigned __int8 a4);
void sub_A5850(HSEQUENCE S, char a2, unsigned int a3, signed int a4, int a5);
void sub_A5E50(HSEQUENCE S);
void sub_A5F30(int *a1, int a2);
void sub_A5FD0(int a1, int a2);
void sub_A6370(HSEQUENCE S);
void sub_A6490(HSEQUENCE S);
void sub_A64C0(HSEQUENCE S);
//void sub_A6530(Bit32u user); // weak
int sub_A6E00_sound_proc25(HMDIDRIVER a1, IO_PARMS* a2);
int sub_A6F30(); // weak
HMDIDRIVER sub_A6FB0_sound_proc26(AIL_DRIVER* a1, IO_PARMS *a2);
HMDIDRIVER sub_A77D0_AIL_API_install_MDI_INI(char* filename, IO_PARMS *IO);//2887d0
// int *sub_A7880_sound_proc27(int a1, int a2, int a3, const void *a4);
Bit32s sub_A78F0_AIL_API_install_MDI_INI(/*int a1, */HMDIDRIVER *mdi); //2888f0
int sub_A7970_AIL_API_uninstall_MDI_driver(HMDIDRIVER mdi);
Bit32s sub_A7990_AIL_API_MDI_driver_type(HMDIDRIVER mdi);
void sub_A7AA0_AIL_API_set_GTL_filename_prefix(Bit8u* a1);
HSEQUENCE sub_A7B30_AIL_API_allocate_sequence_handle(HMDIDRIVER mdi);
void sub_A7BF0_sound_proc33(HSEQUENCE S);
//int sub_A7C20_AIL_API_init_sequence(HSEQUENCE S, int a2, int a3);
Bit32s sub_A7C20_AIL_API_init_sequence(HSEQUENCE S, void* start, Bit32s sequence_num, Bit32u track);
void sub_A8010_AIL_API_start_sequence(HSEQUENCE S, Bit32u track);
void sub_A8050_AIL_API_stop_sequence(HSEQUENCE S);
void sub_A8180_AIL_API_resume_sequence(HSEQUENCE S);
void sub_A8250_AIL_API_end_sequence(HSEQUENCE S);
int sub_A82A0(int a1, int a2);
void sub_A82C0(x_DWORD *a1, int a2, int a3);
void sub_A8360_AIL_API_set_sequence_volume(HSEQUENCE S, Bit32s volume, Bit32s milliseconds);
int sub_A8410_AIL_API_sequence_status(HSEQUENCE S);
signed int sub_A8440(int a1);
int sub_A8470(int a1);
int sub_A84A0(int a1);
void sub_A84D0_AIL_API_set_XMIDI_master_volume(HMDIDRIVER mdi, Bit32s master_volume);
int sub_A8550(int a1);
int sub_A8570_sound_proc39(int *a1, __int16 a2, __int16 a3);
VDI_CALL sub_A85B0_sound_proc40(int *a1, int a2, int a3);
int sub_A8690_sound_proc41(int *a1, __int16 a2, __int16 a3);
int sub_A86D0_sound_proc42(int *a1, __int16 a2, __int16 a3);
int sub_A8710(int a1);
int sub_A8770(int a1, char a2, unsigned int a3);
int sub_A87C0(int a1, int a2);
int sub_A8830(x_DWORD *a1, x_DWORD *a2, x_DWORD *a3);
x_DWORD *sub_A8900(HSEQUENCE S, int a2);
int sub_A89E0(int a1, int a2);
xCALLBACK sub_A8A20_AIL_API_trigger_callback(HSEQUENCE S, xCALLBACK a2);
int sub_A8A60(int a1, int a2);
int sub_A8AA0(int a1, int a2);
int sub_A8AE0(int a1, int a2);
int sub_A8B20(int a1, int a2);
int sub_A8B60(int a1, int a2, int a3);
int sub_A8B90(int a1, int a2);
int sub_A8BD0(int a1, int a2);
int sub_A8BF0(x_DWORD *a1);
int sub_A8EA0(x_DWORD *a1, int a2);
int sub_A9080(x_DWORD *a1, int a2, int a3);
int sub_A9100(int a1, int a2);
void sub_A9140_AIL_API_send_channel_voice_message(HMDIDRIVER mdi, HSEQUENCE S, Bit32s status, Bit32s data_1, Bit32s data_2);
int sub_B096C(int *a1);
int sub_B0B87(HDIGDRIVER a1, int a2);
void sub_B0C1A(HDIGDRIVER a1);

bool sub_A9280();

int sub_9F280(int *a1);
VDI_CALL sub_9F2E0(int *a1, int a2, unsigned __int16 a3, unsigned __int16 a4);
__int64 sub_9F3D0(int *a1, int a2, unsigned __int16 a3, unsigned __int16 a4);
VDI_CALL sub_9F4F0(int *a1, int a2, unsigned __int16 a3, unsigned __int16 a4);
VDI_CALL sub_9F5E0(int *a1, int a2, unsigned __int16 a3, unsigned __int16 a4);
VDI_CALL sub_9F6D0(int *a1, __int16 a2);
signed __int64 sub_9F110(int a1);

void __writegsx_WORD(unsigned long Offset, unsigned short Data);
int sub_9F220(int a1);
int sub_9F1D0(int a1);
int sub_9F170(int a1, unsigned __int16 a2);

void dbgfprintf(FILE* file, const char *format, ...);

int x_free(void* ptr);

#endif //READ_CONFIG