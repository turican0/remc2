#pragma once

#ifndef MAIN_SOUND
#define MAIN_SOUND

#include "ail_sound.h"

#include "Basic.h"

#pragma pack (1)
typedef struct {//lenght 12
	int16_t word_0;
	int16_t word_1;
	int16_t word_2;
	int16_t word_3;
	int16_t word_4;
	int16_t word_5;
}type_F4FE0;
#pragma pack (16)

extern char x_BYTE_E3799_sound_card;
extern char x_BYTE_E37FD;
extern char x_BYTE_E3798_sound_active2;
extern char x_BYTE_E37FC_music;
extern char x_BYTE_E37FD;
extern int x_DWORD_E3844;
extern int x_DWORD_E387C;
extern FILE* x_FILE_E3840;
extern __int16 m_iNumberOfTracks;
extern int x_DWORD_E387C;
//extern int x_DWORD_E3840;
extern int8_t x_WORD_E2A14_sound_activel;
extern int8_t x_WORD_E2A14_sound_activeh;
extern __int16 x_WORD_E37B6_sound_number;
extern char x_BYTE_E379B; // weak
extern __int16 x_WORD_E3828; // weak
extern int x_DWORD_E3824;
extern __int16 x_WORD_E381C;
extern __int16 x_WORD_E381E; // weak
extern __int16 x_WORD_E3820; // weak
extern __int16 x_WORD_E3822; // weak
extern __int16 x_WORD_E387A; // weak
extern __int16 x_WORD_E387A; // weak
extern int x_DWORD_E383C; // weak
extern __int16 x_WORD_E3834; // weak
extern type_E3808_music_header* str_E3808_music_header;
//extern uint8_t* x_DWORD_E3808_music_header; // weak
//extern int8_t* x_DWORD_E380C; // weak
//extern int index_E380C_MaxMusic;
//extern uint8_t* x_DWORD_E3810_music_data; // weak
//extern type_E3810_music_data* str_E3810_music_data; // weak
extern char x_BYTE_180C84_drivertype; // weak
extern int16_t x_WORD_181B44; // weak

extern signed int x_DWORD_182188[]; // idb
extern signed int x_DWORD_1821A0[]; // idb
extern signed int x_DWORD_1821B8[]; // idb
extern signed int x_DWORD_1821D0[]; // idb
extern signed int x_DWORD_1821E8[]; // idb

void /*__fastcall*/ sub_8D290_init_sound(/*char* a1*//*, int a2, int a3*/);
void sub_8D800_sound_proc2();
void sub_8D8F0_sound_proc3_endsample();
void /*__fastcall*/ sub_8D970_init_music(/*char* a1*//*int a1, int a2, char* a3*/);
void sub_8E020_sound_proc14_stopsequence();
int sub_8E0D0(); // weak
void sub_8E160_sound_proc15_startsequence(__int16 track, unsigned __int8 volume);
// x_DWORD sprintf(x_DWORD, const char *, ...); weak
void sub_8E410_sound_proc16_xmidivolume(int32_t master_volume);
int sub_8E470_sound_proc17_volume(int a1);
signed int LoadMusic(int channel);
void GetMusicSequenceCount();
char LoadMusicTrack(FILE* file, unsigned __int8 drivernumber);
int sub_8F0AB(FILE* a1, /*int a2,*/ int a3);
void sub_8F100_sound_proc19(uint32_t a1, __int16 a2, int a3, int a4, unsigned __int16 a5, char a6, unsigned __int8 a7);
void sub_8F420_sound_proc20(int a1, __int16 a2);
void sub_8F710_sound_proc21(int a1, __int16 a2, signed int a3, unsigned __int8 a4, char a5);
void sub_8F850_sound_proc22();


void sub_90FD0();
void sub_91010();
int sub_91420();
signed int sub_916F0_sound_proc24();
signed int sub_919C0();
int sub_91A80_AIL_set_preference(uint32_t number, int32_t value);
void sub_91BD0_s_plus();
void sub_91BF0_s_minus();
uint16_t sub_A121D_AIL_API_get_real_vect(uint32_t vectnum);
// int sub_91C10_AIL_get_real_vect(__int16 a1, int a2, int a3, int a4);
void sub_91D50_AIL_set_real_vect(uint32_t vectnum, uint16_t real_ptr);
void sub_91E90_AIL_restore_USE16_ISR(int32_t a1);

void sub_99C10_sound_proc4();
void sub_99C90();

HDIGDRIVER sub_93330_AIL_install_DIG_driver_file(char* filename, IO_PARMS* IO);
char sub_9AE90(int eax0, int edx0, int ebx0, int* a1, int8_t* a2, int a3, int a4);
int sub_931F0_AIL_install_DIG_INI(HDIGDRIVER* a2, char* digPath);
HMDIDRIVER sub_95850_AIL_install_MDI_driver_file(char* filename, IO_PARMS* IO);
int32_t sub_95710_AIL_install_MDI_INI(HMDIDRIVER* mdi, char* fileName);

void sub_920D0_AIL_delay(int a1);
int sub_92160();
int LoadAILFromFile(AIL_INI* INI, char* filename);
//HTIMER sub_92600_AIL_register_timer(AILTIMERCB callback_fn);
//signed __int32 sub_92740_AIL_set_timer_user(int a1, signed __int32 a2);
//void sub_92890_AIL_set_timer_period(int a1, int a2);
//void sub_92890_AIL_set_timer_period(HTIMER timer, uint32_t microseconds);
//void sub_92930_AIL_set_timer_frequency(int a1, unsigned int a2);
//void sub_92930_AIL_set_timer_frequency(HTIMER timer, uint32_t hertz);
//void sub_92BA0_AIL_start_timer(HTIMER a1);
//void sub_92CB0_AIL_stop_timer(HTIMER a1);
//void sub_92DC0_AIL_release_timer_handle(int32_t a1);
//void sub_92E50_AIL_release_all_timers();
IO_PARMS* sub_92ED0_AIL_get_IO_environment(AIL_DRIVER* drvr);
// x_DWORD *sub_93010_AIL_install_driver(int a1, int a2, int a3);
AIL_DRIVER* sub_93010_AIL_install_driver(/*int a1, */uint8_t* driver_image, int32_t n_bytes);//AIL_install_driver //274010
int sub_93160_AIL_uninstall_driver(AIL_DRIVER* a1);
// int sub_931F0_AIL_install_DIG_INI(int a1, int **a2);
// int *sub_93330_AIL_install_DIG_driver_file_AIL_install_DIG_driver_file(int a1, int a2, const void *a3);
int sub_93480_AIL_uninstall_DIG_driver(HDIGDRIVER a1);
HSAMPLE sub_93510_AIL_allocate_sample_handle(HDIGDRIVER a1);
int sub_937A0_AIL_release_sample_handle(int a1);
void sub_93830_AIL_init_sample(HSAMPLE a1);
//signed int sub_938C0_AIL_set_sample_file(x_DWORD *a1, int a2, int a3);
int32_t sub_938C0_AIL_set_sample_file(HSAMPLE S, uint8_t* file_image, int32_t block);
void sub_93A10_AIL_set_sample_address(HSAMPLE S, uint8_t* start, uint32_t len);
void sub_93AB0_AIL_set_sample_type(x_DWORD* a1, int a2, int a3);
void sub_93B50_AIL_start_sample(HSAMPLE S);
void sub_93D00_AIL_end_sample(HSAMPLE S);
void sub_93D90_AIL_set_sample_playback_rate(HSAMPLE S, int32_t playback_rate);
void sub_93E30_AIL_set_sample_volume(HSAMPLE S, int32_t volume);
void sub_93ED0_AIL_set_sample_volume_pan(HSAMPLE S, int32_t volume);
void sub_93F70_AIL_set_sample_loop_count(HSAMPLE S, int loop_count);
uint32_t sub_94010_AIL_sample_status(HSAMPLE S);
void sub_94650_AIL_set_digital_master_volume(x_DWORD* a1, int a2);
EOS_callback_type sub_95140_AIL_register_EOS_callback(HSAMPLE a1, EOS_callback_type a2);
void sub_953E0_AIL_set_sample_user_data(HSAMPLE S, int a2, int a3);
int sub_95480_AIL_sample_user_data(HSAMPLE S, int a2);
// int sub_95710_AIL_install_MDI_INI(int a1, int **a2);
// int *sub_95850_AIL_install_MDI_driver_file(int a1, int a2, const void *a3);
int sub_959A0_AIL_uninstall_MDI_driver(HMDIDRIVER  mdi);
HSEQUENCE sub_95A30_AIL_allocate_sequence_handle(HMDIDRIVER mdi);
int32_t sub_95C00_AIL_init_sequence(HSEQUENCE hSequence, uint8_t* start, int32_t sequence_num, uint32_t track);
void sub_95D50_AIL_start_sequence(HSEQUENCE hSequence, uint32_t track);
void sub_95DE0_AIL_stop_sequence(HSEQUENCE hSequence);
void sub_95E70_AIL_resume_sequence(HSEQUENCE hSequence);
void sub_95F00_AIL_end_sequence(HSEQUENCE hSequence);
void SetAilSequenceVolume(HSEQUENCE hSequence, int32_t volume, int32_t milliseconds);
int sub_96170_AIL_sequence_status(HSEQUENCE hSequence);
void sub_96670_AIL_set_XMIDI_master_volume(HMDIDRIVER mdi, int32_t master_volume);
int32_t sub_969A0_AIL_MDI_driver_type(HMDIDRIVER mdi);
void sub_96AE0_AIL_set_GTL_filename_prefix(uint8_t* a1);
int sub_97330_AIL_sequence_position(x_DWORD* a1, x_DWORD* a2, x_DWORD* a3);
x_DWORD* sub_97480_AIL_branch_index(HSEQUENCE hSequence, int a2);
xCALLBACK sub_97670_AIL_register_trigger_callback(HSEQUENCE hSequence, xCALLBACK a2);
int sub_97A60_AIL_register_event_callback(int a1, int a2);
int sub_97BB0_AIL_register_timbre_callback(int a1, int a2);
int sub_97F90_AIL_lock_channel(MDI_DRIVER* a1);
void sub_980D0_AIL_release_channel(x_DWORD* a1, int a2);
int sub_98170_AIL_map_sequence_channel(HSEQUENCE a1, int a2, int a3);
void sub_98360_AIL_send_channel_voice_message(HMDIDRIVER mdi, HSEQUENCE hSequence, int32_t status, int32_t data_1, int32_t data_2);

int32_t sub_84300_load_sound(uint8_t a1);
void sub_844A0_sound_proc5();
char ReadAndDecompressSound(FILE* a1, unsigned __int8 a2);

//int sub_9E3A0_AIL_API_read_INI(uint8_t* a1, char* a2);
int sub_9E3A0_AIL_API_read_INI(AIL_INI* INI, char* filename);
int sub_9E6E0_set_preference(uint32_t number, int32_t value);
// x_DWORD *sub_9E720_AIL_API_install_driver(int a1, int a2, int a3);
AIL_DRIVER* sub_9E720_AIL_API_install_driver(uint8_t* driver_image, int32_t n_bytes);//27f720
int sub_9EA60_AIL_API_uninstall_driver(AIL_DRIVER* a1);
IO_PARMS* sub_9EB60_AIL_API_get_IO_environment(AIL_DRIVER* drvr);
void sub_9EC30();
signed int sub_9ED70();
int sub_9EE70(); // weak
void sub_9F040();

void sub_9FA80();

void initAWE32_9F740(char* a1);

int sub_9F280(int* a1);
VDI_CALL sub_9F2E0(int* a1, int a2, unsigned __int16 a3, unsigned __int16 a4);
__int64 sub_9F3D0(int* a1, int a2, unsigned __int16 a3, unsigned __int16 a4);
VDI_CALL sub_9F4F0(int* a1, int a2, unsigned __int16 a3, unsigned __int16 a4);
VDI_CALL sub_9F5E0(int* a1, int a2, unsigned __int16 a3, unsigned __int16 a4);
VDI_CALL sub_9F6D0(int* a1, __int16 a2);

void sub_A0EEC_s_plus2();
void sub_A0EF9_s_minus2();
char sub_A102C(int a1);
char sub_A105C(unsigned int a1);
void sub_A108F();
void sub_A10F4_sound_proc_irq();
signed int sub_A11E2();
void sub_A1249_AIL_API_set_real_vect(uint32_t vectnum, uint16_t real_ptr);
//signed int sub_A12C5_sound_proc_irq(int a1, int a2, __int16 a3);
void sub_A14DB_AIL_API_restore_USE16_ISR(int a1);
unsigned int sub_A1520();
void sub_A1524(unsigned int a1);
//int sub_A158B_AIL_API_call_driver(int32_t* drvr, __int16 a2, x_WORD *a3, x_WORD *a4);
int32_t sub_A158B_AIL_API_call_driver(AIL_DRIVER* drvr, int32_t fn, VDI_CALL* in, VDI_CALL* out);
void sub_A1665_AIL_API_delay(int a1);
int sub_A16A2();
//unsigned int sub_A16AE_register_timer(int a1);
//HTIMER sub_A16AE_register_timer(AILTIMERCB callback_fn);
//signed __int32 sub_A16F5_AIL_API_set_timer_user(int a1, signed __int32 a2);
void sub_A171D_release_timer_handle(int32_t a1);
void sub_A1744_AIL_API_release_all_timers();
//void sub_A1768_start_timer(HTIMER timer);
void sub_A1798();
void sub_A17BC_stop_timer(HTIMER timer);
void sub_A17EC();
//void sub_A1810_set_timer_period(int a1, int a2);
void sub_A1810_set_timer_period(HTIMER timer, uint32_t microseconds);
//void sub_A1840_AIL_API_set_timer_frequency(int a1, unsigned int a2);
void sub_A1840_AIL_API_set_timer_frequency(HTIMER timer, uint32_t hertz);
void sub_A1870(int a1, unsigned int a2);
int sub_A18B2();
// x_DWORD fprintf(x_DWORD, const char *, ...); weak
// x_DWORD fopen(x_DWORD, x_DWORD); weak
// x_DWORD fclose(x_DWORD); weak
void sub_A2070(HDIGDRIVER a1);
void sub_A20D0(HDIGDRIVER a1);
void sub_A2110(HSAMPLE S);
void sub_A2450(uint32_t user); // weak
int sub_A2650(HDIGDRIVER a1);
int sub_A2C80(HDIGDRIVER a1, IO_PARMS* a2);
int sub_A2DE0(); // weak
HDIGDRIVER sub_A2EA0(AIL_DRIVER* a1, IO_PARMS IO);
// int *sub_A3600_AIL_API_install_DIG_driver_file(int a1, int a2, const void *a3);
// int *sub_A36B0(int a1, int a2, int a3, const void *a4);
// int sub_A3720_AIL_API_install_DIG_INI(int a1, int **a2);
int sub_A37A0(HDIGDRIVER a1);
int sub_A37C0(int a1);
HSAMPLE sub_A3820_allocate_sample_handle(HDIGDRIVER dig);
int sub_A38C0_AIL_API_release_sample_handle(int a1);
void sub_A38E0_init_sample(HSAMPLE S);
uint32_t sub_A3A00_sample_status(HSAMPLE S);
void sub_A3A30_set_sample_address(HSAMPLE S, uint8_t* start, uint32_t len);
void sub_A3A70_set_sample_type(HSAMPLE S, int32_t a2, uint32_t a3);
int sub_A3AC0(int a1);
void sub_A3AF0_set_sample_playback_rate(HSAMPLE S, int32_t playback_rate);
int32_t sub_A3B10_get_sample_volume(HSAMPLE S);
void sub_A3B40_set_sample_volume(HSAMPLE S, int32_t volume);
int32_t sub_A3B80_get_sample_volume_pan(HSAMPLE S);
void sub_A3BB0_set_sample_volume_pan(HSAMPLE S, int32_t volume);
int sub_A3BF0(HSAMPLE S);
int sub_A3C30(HSAMPLE S, int a2);
signed int sub_A3C60_get_sample_loop_count(HSAMPLE S);
void sub_A3C90_set_sample_loop_count(HSAMPLE S, int32_t loop_count);
void sub_A3CB0_API_AIL_start_sample(HSAMPLE S);
void sub_A3D30(HSAMPLE S);
void sub_A3D60(HSAMPLE S);
void sub_A3DA0__API_AIL_end_sample(HSAMPLE S);
int sub_A3E10(HSAMPLE S, int a2);
int sub_A3E60(HSAMPLE S, int a2);
EOS_callback_type sub_A3EB0_register_EOS_callback(HSAMPLE S, EOS_callback_type EOS);
void sub_A3F00_set_sample_user_data(HSAMPLE S, int a2, int a3);
int sub_A3F30_sample_user_data(HSAMPLE S, int index);
void sub_A3F70_set_digital_master_volume(x_DWORD* a1, int a2);
int sub_A3FE0(int a1);
char sub_A4000(x_DWORD* a1, int a2, unsigned int a3);
void sub_A4100(int* a1, int a2, int a3, int a4);
signed int sub_A41B0(x_DWORD* a1);
int sub_A4370(x_DWORD* a1);
int sub_A4390(int a1);
void sub_A43E0(int a1);
int sub_A47A0(); // weak
void sub_A47C0_sub_set_sample_file(uint8_t* file_image, HSAMPLE S);
int sub_A4920(); // weak
HSAMPLE sub_A4970(int a1, uint8_t* a2, int a3);
int32_t sub_A4B20_set_sample_file(HSAMPLE S, uint8_t* file_image, int32_t block);
int sub_A4C60(int a1, int a2);
signed int sub_A4E10(char a1);
void sub_A4EB0(HMDIDRIVER a1);
void sub_A4F10(HMDIDRIVER a1, char a2, char a3, char a4);
void sub_A4FD0(HMDIDRIVER a1, int a2, unsigned int a3);
int sub_A5040(x_DWORD* a1);
unsigned int sub_A50A0(int a1);
uint8_t* sub_A50F0(uint8_t* a1, int a2);
int sub_A5210(int a1, char a2, unsigned int a3);
void sub_A5530(CTRL_LOG* a1, char a2, unsigned int a3, unsigned __int8 a4);
void sub_A5850(HSEQUENCE hSequence, char a2, unsigned int a3, signed int a4, int a5);
void sub_A5E50(HSEQUENCE hSequence);
void sub_A5F30(int* a1, int a2);
void sub_A5FD0(HSEQUENCE a1, int a2);
void sub_A6370(HSEQUENCE hSequence);
void sub_A6490(HSEQUENCE hSequence);
void sub_A64C0(HSEQUENCE hSequence);
//void sub_A6530(uint32_t user); // weak
int sub_A6E00_sound_proc25(HMDIDRIVER a1, IO_PARMS* a2);
int sub_A6F30(); // weak
HMDIDRIVER sub_A6FB0_sound_proc26(AIL_DRIVER* a1, IO_PARMS* a2);
// int *sub_A77D0_AIL_API_install_MDI_INI(int a1, int a2, const void *a3);
// int *sub_A7880_sound_proc27(int a1, int a2, int a3, const void *a4);
// int sub_A78F0_AIL_API_install_MDI_INI(int a1, int **a2);
int sub_A7970_AIL_API_uninstall_MDI_driver(HMDIDRIVER mdi);
int32_t sub_A7990_AIL_API_MDI_driver_type(HMDIDRIVER mdi);
void sub_A7AA0_AIL_API_set_GTL_filename_prefix(uint8_t* a1);
HSEQUENCE sub_A7B30_AIL_API_allocate_sequence_handle(HMDIDRIVER mdi);
void sub_A7BF0_sound_proc33(HSEQUENCE hSequence);
//int sub_A7C20_AIL_API_init_sequence(HSEQUENCE hSequence, int a2, int a3);
int32_t sub_A7C20_AIL_API_init_sequence(HSEQUENCE hSequence, void* start, int32_t sequence_num, uint32_t track);
void sub_A8010_AIL_API_start_sequence(HSEQUENCE hSequence, uint32_t track);
void sub_A8050_AIL_API_stop_sequence(HSEQUENCE hSequence);
void sub_A8180_AIL_API_resume_sequence(HSEQUENCE hSequence);
void sub_A8250_AIL_API_end_sequence(HSEQUENCE hSequence);
int sub_A82A0(int a1, int a2);
void sub_A82C0(x_DWORD* a1, int a2, int a3);
void sub_A8360_AIL_API_set_sequence_volume(HSEQUENCE hSequence, int32_t volume, int32_t milliseconds);
int sub_A8410_AIL_API_sequence_status(HSEQUENCE hSequence);
signed int sub_A8440(int a1);
int sub_A8470(int a1);
int sub_A84A0(int a1);
void sub_A84D0_AIL_API_set_XMIDI_master_volume(HMDIDRIVER mdi, int32_t master_volume);
int sub_A8550(int a1);
int sub_A8570_sound_proc39(int* a1, __int16 a2, __int16 a3);
VDI_CALL sub_A85B0_sound_proc40(int* a1, int a2, int a3);
int sub_A8690_sound_proc41(int* a1, __int16 a2, __int16 a3);
int sub_A86D0_sound_proc42(int* a1, __int16 a2, __int16 a3);
int sub_A8710(int a1);
int sub_A8770(int a1, char a2, unsigned int a3);
int sub_A87C0(int a1, int a2);
int sub_A8830(x_DWORD* a1, x_DWORD* a2, x_DWORD* a3);
x_DWORD* sub_A8900(HSEQUENCE hSequence, int a2);
int sub_A89E0(int a1, int a2);
xCALLBACK sub_A8A20_AIL_API_trigger_callback(HSEQUENCE hSequence, xCALLBACK a2);
int sub_A8A60(int a1, int a2);
int sub_A8AA0(int a1, int a2);
int sub_A8AE0(int a1, int a2);
int sub_A8B20(int a1, int a2);
int sub_A8B60(int a1, int a2, int a3);
int sub_A8B90(int a1, int a2);
int sub_A8BD0(int a1, int a2);
int sub_A8BF0(MDI_DRIVER* a1);
void sub_A8EA0(x_DWORD* a1, int a2);
int sub_A9080(HSEQUENCE a1, int a2, int a3);
int sub_A9100(int a1, int a2);
void sub_A9140_AIL_API_send_channel_voice_message(HMDIDRIVER mdi, HSEQUENCE hSequence, int32_t status, int32_t data_1, int32_t data_2);
int sub_A9230(int a1, int a2);
x_DWORD* sub_A95D0(int a1, char a2, unsigned int a3, unsigned __int8 a4);
x_DWORD* sub_A98D0(x_DWORD* a1, int a2, int a3);
void sub_A9950(int a1, int a2);
void sub_A9A30(int a1, int a2);
int sub_A9C00(); // weak
int sub_A9C50(); // weak
x_DWORD* sub_AA310(int a1, int a2, int a3, signed int a4);
int sub_AA590(int* a1);

int sub_A3720_AIL_API_install_DIG_INI(HDIGDRIVER* dig, char* digPath);
HDIGDRIVER sub_A3600_AIL_API_install_DIG_driver_file(char* filename, IO_PARMS* IO);
int32_t sub_A78F0_AIL_API_install_MDI_INI(HMDIDRIVER* mdi, char* fileName);
HMDIDRIVER sub_A77D0_AIL_API_install_MDI_INI(char* filename, IO_PARMS* IO);

__int16 sub_98B2C(unsigned __int8 a1, int a2);

int sub_9E2B0(char* a1, int a2/*, x_DWORD* a3*/);

signed int sub_9D4D0(int a1, uint8_t** a2, uint32_t* a3, uint32_t* a4);//27e4d0

void sub_9D560(uint8_t* a1);

unsigned int sub_91F20();

signed __int64 sub_9F110(int a1);
int sub_9F170(int a1, unsigned __int16 a2);
int sub_9F1D0(int a1);
int sub_9F220(int a1);

int sub_B0B87(HDIGDRIVER a1, int a2);
void sub_B0C1A(HDIGDRIVER a1);
char* sub_B0CC8();
void sub_99970(char a1, unsigned __int8 a2);

signed int sub_99E8E(unsigned __int8* a1, unsigned __int8** a2, signed int a3, int a4);

int sub_99FF5(unsigned __int8 a1);

int sub_9A230_set_x_WORD_E37B4(int a1);

signed int sub_ACB61(int a1, unsigned __int8* a2, int* a3);
unsigned __int8* sub_ACD79(unsigned __int8* a1, int a2);
int sub_ACE56(int a1);

int sub_AEAF5();


void AIL_fix();
char* mygetenv(const char* a1);

#endif //MAIN_SOUND