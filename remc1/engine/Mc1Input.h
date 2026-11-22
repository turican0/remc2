#pragma once

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// MC1 input of the player (17C20): the keys and the mouse to the command of the player in the frame (state + 7413h)

extern const uint8_t scanCodeChars_9AD0C_9AD0C[128];

// by the mode of the player: 0 and 4 the game, 1 the help, 2 the spells, 3 a message being typed
void ProcessInput_17C20_17C20();
// the command (31 kinds) with its parameter, mostly only into an empty slot or over the same command
void SetPlayerCommand_188A0_188A0(uint16_t command, uint8_t param);
// the mouse to the move of the carpet (-127..127 from the centre of the screen)
void MouseToMove_169E0_169E0();
// Alt: F1-F7, save / load; Shift: quit, restart, cheats; P the pause, F1-F10 the sound, the music, the speed and the
// options of the view (a message to the player)
void FunctionKeys_16B00_16B00();

extern uint8_t joyMenuMouse_90B20_90B20;
extern uint8_t joyLeftDown_90B21_90B21;
extern uint8_t joyRightDown_90B22_90B22;
extern int16_t joyMouseX_90B1C_90B1C;
extern int16_t joyMouseY_90B1E_90B1E;
extern int16_t vfxSamplesX_90B04_90B04[2];
extern int16_t vfxSamplesY_90B0C_90B0C[2];
extern int32_t vfxIndexX_90B18_90B18;
extern int32_t vfxIndexY_90B14_90B14;
extern uint8_t vfxTracking_9AD08_9AD08;
extern int16_t vfxTrackerY_12EE9E_12EE8E;
extern int16_t vfxTrackerX_12EEA0_12EE90;
int32_t InitVfx_5AF50_5B460();
int32_t ReadVfx_5B210_5B720();
void SetVfxMouse_5B2B0_5B7C0();
void InitDigitalJoystick_3F7D0_3FB10(bitmap_pos_struct_t* sprite);
void InitAnalogJoystick_3F820_3FB60(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
extern uint8_t secondStick_B7701_B76F1;
extern int16_t joyX_B7702_B76F2;
extern int16_t joyY_B7704_B76F4;
extern int16_t joyMaxX_B7706_B76F6;
extern int16_t joyMaxY_B7708_B76F8;
extern int16_t joyMinX_B770A_B76FA;
extern int16_t joyMinY_B770C_B76FC;
extern uint16_t joyScaleMaxX_B770E_B76FE;
extern uint16_t joyScaleMaxY_B7710_B7700;
extern int16_t joyScaleMinX_B7712_B7702;
extern int16_t joyScaleMinY_B7714_B7704;
extern int16_t joy2X_B7716_B7706;
extern int16_t joy2Y_B7718_B7708;
extern int16_t joy2MaxX_B771A_B770A;
extern int16_t joy2MaxY_B771C_B770C;
extern int16_t joy2MinX_B771E_B770E;
extern int16_t joy2MinY_B7720_B7710;
extern uint16_t joy2ScaleMaxX_B7722_B7712;
extern uint16_t joy2ScaleMaxY_B7724_B7714;
extern int16_t joy2ScaleMinX_B7726_B7716;
extern int16_t joy2ScaleMinY_B7728_B7718;
extern uint8_t joyButtons_B772A_B771A;

// the joystick (mode 0 the game with the VFX1 head, else a menu: the mouse moved by the steps in the area)
void JoystickInput_3FCA0_3FFE0(int32_t mode, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY, int32_t stepX, int32_t stepY);
void ReadDigitalJoystick_3F413_3F753();
void ReadAnalogJoystick_3F587_3F8C7();
