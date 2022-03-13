#pragma once

#ifndef GL_GAME_RENDER
#define GL_GAME_RENDER

#include "GameRenderInterface.h"

class GLGameRender: public GameRenderInterface
{
public:
	GLGameRender(uint16_t screenWidth, uint16_t screenHeight, uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight, uint8_t x_BYTE_F6EE0_tablesx[]);
	~GLGameRender();
	void SetRenderViewPortSize_BCD45(uint8_t viewPortSizeSetting, uint16_t screenWidth, uint16_t screenHeight);
	void SetRenderViewPortSize_BCD45(ViewPort viewPort, uint16_t screenWidth, uint16_t screenHeight);
	void DrawWorld(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov, uint8_t heightmap[], type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], uint8_t unk_F0A20x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int16_t x_WORD_180660_VGA_type_resolution, int16_t x_WORD_D4B7C, char isCaveLevel);
};

#endif //GL_GAME_RENDER
