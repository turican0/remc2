#pragma once

#ifndef GAME_RENDER_GL
#define GAME_RENDER_GL

#include "GameRenderInterface.h"

class GameRenderGL: public GameRenderInterface
{
public:
	~GameRenderGL();
	void DrawWorld(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov, uint8_t heightmap[], type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], uint8_t unk_F0A20x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int16_t x_WORD_180660_VGA_type_resolution, int16_t x_WORD_D4B7C, char isCaveLevel);
};

#endif //GAME_RENDER_GL
