#pragma once

#include "BasicMini.h"
#include "global_types.h"
#include "../portability/bitmap_pos_struct.h"

#pragma pack(push, 1)
typedef struct {
	int32_t x_DWORD_18058C;
	int32_t joyAxisX_180590; // [4]//inputState_18058C.x_DWORD_180590//1
	int32_t joyAxisY_180594; // [8]//inputState_18058C.x_DWORD_180594//2
	int32_t joyAxisZ_180598; // [12]//inputState_18058C.x_DWORD_180598//3
	int32_t MouseButtonState_18059C; // [16]//inputState_18058C.x_DWORD_18059C//4
	int32_t x_DWORD_1805A0; // weak//unk_18058Car[20]//5
	int32_t joyCenterX_1805A4; // weak//unk_18058Car[0x18]//6
	int32_t joyCenterY_1805A8; // weak//unk_18058Car[28]//7
	int32_t joyCenterZ_1805AC; // weak//unk_18058Car[0x20]//8
	axis_2d_32 mouse_1805B0;
	//int32_t x_DWORD_1805B0_mouse.x; // weak//unk_18058Car[36]
	//int32_t x_DWORD_1805B0_mouse.y; // weak//unk_18058Car[40]
	int32_t mouseX_1805B8; // weak//unk_18058Car[44]
	int32_t mouseY_1805BC; // weak//unk_18058Car[48]
	int16_t arrowKeys_1805C0; // weak//unk_18058Car[52] // keyboard buttons
	int16_t joystickMode_1805C2; // weak//unk_18058Car[54]
	int16_t vio1AnalogJoy_1805C4; // weak//unk_18058Car[56]
	int16_t vio2AnalogJoy_1805C6; // weak//unk_18058Car[58]
	int16_t x_WORD_1805C8; // weak//unk_18058Car[60]
	uint8_t serialBuffer_1805CE[84]; // weak//unk_18058Car[66]
	int16_t stub_180622;
} type_unk_18058Cstr;//end 180624
#pragma pack(pop)


extern char cursorSpriteIndex_D419E; // weak//2a519e
extern char IsPauseMenuOpen_D47D9; // weak
extern int debugcounter_47560;
extern int x_DWORD_17FF14; // weak
extern int16_t x_WORD_17FF18; // weak
extern int x_DWORD_17FF20; // weak
extern int cdBuffer_17FF24; // weak
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
void InstallPaletteAndMouseMinMax2_417D0();
void ReadPauseMenuEvents_197F0();
char CycleSelectedSpell_18DA0(type_entity_0x6E8E* a1, char a2, char a3);
void ToggleSpellMenu_18B30();
void FlyAssistant_1A7A0();
void HandleMouseButtons_18F80(type_entity_0x6E8E* a1);
void HandleArrowKeyPresses_1A8A0();
void ComputeMousePlayerMovement_17060(int16_t x, int16_t y);
int SelectSpellCategory_6D420(int16_t posX, int16_t posY);
char UpdateSubSpellIndex_6D4C0(type_str_611* a1);
void ReadOptionMenuEvents_19AB0();
void ReadOkayCancelButtonEvents_19E00();
void SetCurrentNotificationMessage_19760(const char* message, uint8_t a2, int16_t duration);
void ApplyBrightnessToPalette_47650(int a1/*, int a2*/);
void GetPauseMenuCoordinates_2FFE0(int16_t* posX, int16_t* posY, int16_t* width, int16_t* height, uint8_t scale = 1);
void GetChatMenuCoordinates(int16_t* posX, int16_t* posY, uint8_t scale = 1);
signed int GetNextSearchOffset_10130(int a1, x_DWORD* a2, x_DWORD* a3);
void ClearMouseButtonState_19A50();
void FreeWebSprites_47130();

extern uint8_t keys_EB39E[10];
extern char* vrRecord4_17D6C4;
extern uint8_t unk_180560x[44];
extern type_unk_18058Cstr inputState_18058C;
void MouseReset_54600();
void SetDefaultKeys_5BCC0();
void SetMousePosition_6EDE0(int16_t posX, int16_t posY);
void ClearPressedKeys_753D0();
void SetMouseMinMaxVert_759B0();
void GetMouseAndKeyboardEvents_7A060();
void ResetMouse_7B5A0();
int ReadGameUserInputs_89D10();
void InitJoystickState_8BB40(uint8_t* a1, char a2);
signed int ReadJoystick_8BBE0(uint8_t* a1);
char GetJoystickButtons_8C140(unsigned __int16 a1, uint8_t* a2);
int UpdateMouseCursorPosition_8C329();
int SaveCursorBackground_8C839();
int RestoreCursorBackground_8CA16();
void ClearCursorDrawnFlag_8CB1F();
void UpdateMouseEventData_8CB3A(uint32_t mouse_states, int32_t mouse_posx, int32_t mouse_posy);
signed int InstallMouse_8CEDF();
int SetMouseBoundsScaled_9025C(__int16 a1, __int16 a2, __int16 a3, __int16 a4, unsigned __int16 a5, __int16 a6);
signed int CheckKeyboard_9A10A();
__int16 ReadKeyboardShiftFlags_9D31C(__int16 result);
