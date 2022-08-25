#pragma once

#include <cstdint>

#include "../sub_main.h"

// from sub_main
extern char x_BYTE_D419C_level_num;
extern char x_BYTE_D41AD_skip_screen;
extern const char* LevelsNames_D9204[61];
extern char* off_DB558[];
extern __int16 x_WORD_E131A;
extern char x_BYTE_E1324;
extern type_WORD_E1F84 str_E1BAC[10];
extern long lastTime_E208C;
extern __int16 timeCounter_E2094;
extern char minTimeCounter_E2096;
extern char maxTimeCounter_E2097;
extern type_x_BYTE_E25ED_db_str x_BYTE_E25ED_db_str[16];
extern int16_t x_WORD_E29D6_not_movex;
extern int16_t x_WORD_E29D8;
extern int16_t x_WORD_E29DC;
extern char x_BYTE_E29DF_skip_screen;
extern char x_BYTE_E29E0;
extern uint8_t* pre_x_DWORD_E9C3C;
extern uint8_t x_BYTE_EB39E_keys[10];
extern uint8_t uiBackGroundColorIdx_EB3A8;

extern type_x_DWORD_17DB70str x_DWORD_17DB70str;
extern int32_t x_DWORD_17DBB8[4];
extern char x_BYTE_17DBC6;
extern uint32_t x_DWORD_17DBC8x[125];
extern uint32_t x_DWORD_17DDBCx[26];
extern char x_BYTE_17E09D;
extern int16_t x_WORD_1803EC;
extern uint8_t unk_180560x[0x2c];
extern type_unk_18058Cstr unk_18058Cstr;

extern int test_regression_level;

// functions
void sub_76930_menus_and_intros(int a2, uint16_t a3);
void sub_76A40_lang_setting();
void sub_76D10_intros(char a1);
void sub_76CF0();
void sub_76FA0_main_menu(/*int a1,*/ int a2, uint16_t a3);
bool sub_77350_new_game_dialog(type_WORD_E1F84* a1x);
char sub_779E0_lang_setting_dialog(type_WORD_E1F84* a1x);
char sub_780F0_load_game_dialog(type_WORD_E1F84* a1x);
char sub_78730_save_game_dialog(type_WORD_E1F84* a1x);
char sub_79610_set_keys_dialog();
signed int sub_79E10(char* a1, char a2);
void sub_7AB00_draw_menu_animations();
void sub_7AC00_load_and_set_graphics_and_Palette();
int sub_7ADE0(char a1);
void sub_7BEC0();
void sub_7DA70(__int16 a1, __int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6);
void sub_7DD70();
int16_t sub_7E1F0_test_mouse_regions();
signed int sub_7E620(type_WORD_E1F84* a1x);
char sub_7E800(type_WORD_E1F84* a1x);
char sub_7E820(type_WORD_E1F84* a1x);
int sub_7EAE0_new_game_draw(__int16* a1, __int16* a2, __int16* a3, __int16* a4, int8_t* a5, x_WORD* a6);
int LoadLanguageFile(posistruct2_t** a1x, posistruct2_t** a2x, uint8_t* a3, char* langfilename, posistruct_t* a3dattabindex);
int sub_7F960(posistruct2_t* a1x, posistruct2_t* a2x, uint8_t* a3, char* langcountstring, posistruct_t* a3dattabindex);
void DrawText_80C30(__int16 posX, __int16 posY, __int16 a3);
void sub_81760_new_game_subdraw(/*type_unk_E17CC_str_0x194* a1*/);
void sub_81DB0_read_config();
void sub_81EE0_draw_and_sound_dragon_and_fire(__int16 a5, __int16 a6);
void sub_82510(/*__int16 a1*//*, int *a2*/);
void sub_82670();
void sub_82C20_drawEndGameTable(__int16 a1);
void sub_83250_play_intros(char a1);
void sub_833C0();
void sub_83850_show_welcome_screen();
void sub_85C8B_draw_new_game_map_background(uint8_t* a1, uint8_t* a2, int a3, int a4, int a5, int a6);
bool pre_sub_7B250_draw_and_serve(uint32_t var, type_WORD_E1F84* var2x);
char sub_77980_exit_dialog(type_WORD_E1F84* a1x);
char sub_78E00_set_player_name_dialog(/*int a1, int a2, */type_WORD_E1F84* a3x);
int sub_7F6A0(int a1, int a2, __int16 a3, __int16 a4, char* a5, int8_t* a6, unsigned __int8 a7);
char sub_79160_set_joystick_dialog(type_WORD_E1F84* a1x);
int sub_7CCF0();
int sub_7CD30();
int sub_7CCA0();
int sub_7CDA0();
char sub_7CBF0();
char sub_7CC40();
char MultiplayerMenu_7DE80(type_WORD_E1F84* a2x);
int sub_7BF20_draw_scroll_dialog(type_str_word_26* a1x);
void sub_8C0E0(unsigned __int8(/*__fastcall*/ *a1)(signed int));
int sub_7B660_draw_scroll_dialog2(int a1, int a2, __int16 a3, type_str_word_26* a4x, char* a5, char a6);
char /*__fastcall*/ sub_77680();
char sub_7B250_draw_and_serve(/*int a1, int a2*//*, __int16 a3*/);
signed int sub_7C390();
void sub_7C710();
bool SetMultiplayerColors_7CE50();
void sub_7C720(unsigned __int8 a1, uint8_t* a2);
int sub_7CB10();
bool sub_7B200_in_region(type_WORD_E1F84* a1x, int16_t testx, int16_t testy);
void PaletteMulti_7C9D0(signed __int16 a1);
void CleanRectByColor_7C7C0(uint8_t* a2);
void CleanRecByColor_85C42(uint8_t* a1, int a2, int a3, int a4, int a5, unsigned __int8 a6);
void DrawNetworkLevelName_7D380();
void PaletteCopy_7C800(signed __int16 a1);
void sub_85BF5(uint8_t* a1, uint8_t* a2, int a3, int a4, int a5, int a6);
