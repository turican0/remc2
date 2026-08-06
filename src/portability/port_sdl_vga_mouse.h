#pragma once
#ifndef PORT_SDL_VGA_MOUSE_H
#define PORT_SDL_VGA_MOUSE_H

#include <cstdint>
#include <stdio.h>
#include <assert.h>
#include <sstream>
#include <stddef.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "SDL2/SDL.h"

#include "port_sdl_sound.h"
#include "port_filesystem.h"
#include "engine/Scene.h"
#include "engine/GameState.h"

typedef struct
{
	int32_t x;
	int32_t y;
} POSITION;



void VGA_Init(int windowWidth, int windowHeight, int gameResWidth, int gameResHeight, bool maintainAspectRatio, int displayIndex);
void VGA_Init(Uint32 flags, int windowWidth, int windowHeight, int gameResWidth, int gameResHeight, bool maintainAspectRatio, int displayIndex);
void CreateRenderSurfaces(int gameResWidth, int gameResHeight);
bool VGA_LoadFont();
void VGA_close();
void FreeRenderSurfaces();
void VGA_Resize(int width, int height);
void VGA_Blit(Uint8* srcBuffer);
void SubBlit(uint16_t originalResWidth, uint16_t originalResHeight);
void VGA_Debug_Blit(int width, int height, Uint8* srcBuffer);
void VGA_Set_Palette(Uint8* Palettebuffer);
void VGA_Set_Palette2(Uint8* Palettebuffer);
void SetPalette(SDL_Color* colors);
void VGA_Write_basic_Palette(Uint8* Palettebuffer);
Uint8* VGA_Get_Palette();
void VGA_Draw_string(char* wrstring);
void VGA_Draw_stringXYtoBuffer(const char* wrstring, int x, int y, uint8_t* buffer, char fontSize = 'M');
void Draw_letter(int letter_number, int pozx, int pozy);
void Draw_letterToBuffer(int letter_number, int pozx, int pozy, uint8_t* buffer, char fontSize = 'M');
//void VGA_Set_Cursor(posistruct posstrcursor,int count);
void VGA_GotoXY(int x, int y);
int VGA_WhereX(); //returns current text cursor (x) coordinate
int VGA_WhereY(); //returns current text cursor (y) coordinate
POSITION VGA_WhereXY();
void Set_basic_Palette0();
void Set_basic_Palette1();
void VGA_Set_file_Palette(char* filename);
int16_t VGA_get_shift_status();
bool VGA_check_standart_input_status();
uint16_t VGA_read_char_from_buffer();
void VGA_Set_mouse(int16_t a1, int16_t a2);
int PollSdlEvents();
uint16_t TranslateSdlKeysToGameKeys(uint16_t scancode);
uint32_t TranslateSdlMouseToGameMouse(SDL_MouseButtonEvent event);
uint32_t TranslateButtonState(SDL_MouseButtonEvent button);
void ScaleUpMouseCoordsToVga(int16_t& x, int16_t& y);
void ScaleDownMouseCoordsToVga(int16_t& x, int16_t& y);
void SetPress(bool pressed, uint16_t scancode);
void SetGameKeyPress_1806E4(bool pressed, uint16_t gameKeyChar);
void SetMouseEvents(uint32_t buttons, int16_t x, int16_t y);

void VGA_mouse_clear_keys();
void VGA_cleanKeyBuffer();
void Draw_debug_matrix1();
void Draw_black();
std::vector<SDL_Rect> GetDisplays();
SDL_Rect GetDisplayByIndex(uint8_t index);
SDL_Rect FindDisplayByResolution(uint32_t width, uint32_t height);
void ToggleFullscreen();
void ToggleFullscreen(bool fullScreen);
bool HandleSpecialKeys(const SDL_Event& event);
void SetMouseKeyboardScene(const Scene sceneId);
void SetMouseKeyboardGameState(const GameState state);
void OnMouseResolutionChanged(uint32_t width, uint32_t height);
void WriteSurfaceToFile(SDL_Surface* surface);
void WriteSurfaceToFile(SDL_Surface* surface, const char* filename);

extern uint8_t LastPressedKey_1806E4; // weak//3516e4
extern int8_t pressedKeys_180664[128]; // idb

#endif //PORT_SDL_VGA_MOUSE
