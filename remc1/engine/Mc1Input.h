#pragma once

#include <cstdint>

// MC1 input of the player (17C20): the keys and the mouse to the command of the player in the frame (state + 7413h)

extern const uint8_t scanCodeChars_9AD0C_9AD0C[128];

// by the mode of the player: 0 and 4 the game, 1 the help, 2 the spells, 3 a message being typed
void ProcessInput_17C20_17C20();
// the command (31 kinds) with its parameter, mostly only into an empty slot or over the same command
void SetPlayerCommand_188A0_188A0(uint16_t command, uint8_t param);
// the mouse to the move of the carpet (-127..127 from the centre of the screen)
void MouseToMove_169E0_169E0();
