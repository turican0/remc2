#pragma once

#include "global_types.h"
#include "../portability/bitmap_pos_struct.h"

extern char CURSOR_SPRITE_INDEX_D419E; // weak//2a519e
extern char IsPauseMenuOpen_D47D9; // weak
extern int debugcounter_47560;
extern int x_DWORD_17FF14; // weak
extern int16_t x_WORD_17FF18; // weak
extern int x_DWORD_17FF20; // weak
extern int x_DWORD_17FF24; // weak
extern int x_DWORD_17FF28; // weak
extern int x_DWORD_17FF38; // weak
extern int x_DWORD_17FF40; // weak

void ProcessKeyboardPresses_17190();
void MouseAndKeysEvents_17A00(int16_t turn);
void PauseUnpauseGame_18BB0();
void HandleButtonClick_191B0(int16_t hiSetting, char loSetting);
void ChangeSettings_1A970(char a1, int a2, int a3);
void SetPausedMenuOpen_41AF0();
void SetCursor_8CD27(bitmap_pos_struct_t a2);
void sub_417D0_install_pal_and_mouse_minmax2();
void ReadPauseMenuEvents_197F0();
char sub_18DA0(type_entity_0x6E8E* a1, char a2, char a3);
void sub_18B30();
void sub_1A7A0_fly_asistant();
void HandleMouseButtons_18F80(type_entity_0x6E8E* a1);
void HandleArrowKeyPresses_1A8A0();
void ComputeMousePlayerMovement_17060(int16_t x, int16_t y);
int SelectSpellCategory_6D420(int16_t posX, int16_t posY);
char sub_6D4C0(type_str_611* a1);
void ReadOptionMenuEvents_19AB0();
void ReadOkayCancelButtonEvents_19E00();
void SetCurrentNotificationMessage_19760(const char* message, uint8_t a2, int16_t duration);
void sub_47650(int a1/*, int a2*/);
void GetPauseMenuCoordinates_2FFE0(int16_t* posX, int16_t* posY, int16_t* width, int16_t* height, uint8_t scale = 1);
signed int sub_10130(int a1, x_DWORD* a2, x_DWORD* a3);
void sub_19A50();
void sub_47130();
