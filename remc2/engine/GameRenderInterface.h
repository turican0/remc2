#pragma once

#ifndef GAME_RENDER_INTERFACE
#define GAME_RENDER_INTERFACE

#include <cstdint>
#include "engine_support.h"
#include "ViewPort.h"
//#include "Type_E9C38_Smalltit.h"
//#include "Type_F2C20ar.h"

class GameRenderInterface 
{
public:
	// Empty virtual destructor for proper cleanup
	//virtual GameRenderInterface(uint8_t renderThreads, bool assignToSpecificCores, int rendererType);
	//GameRenderInterface() {}
	virtual ~GameRenderInterface() {}
	//virtual void Init(uint8_t renderThreads, bool assignToSpecificCores) = 0;
	//virtual void SetRenderViewPortSize_BCD45(ViewPort viewPort, uint16_t screenWidth, uint16_t screenHeight) = 0;
	//virtual void SetRenderViewPortSize_40C50(uint8_t viewPortSizeSetting, uint16_t screenWidth, uint16_t screenHeight) = 0;
	//virtual void DrawWorld(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov, uint8_t heightmap[], type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], uint8_t unk_F0A20x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int16_t x_WORD_180660_VGA_type_resolution, int16_t x_WORD_D4B7C, char isCaveLevel) = 0;

	//virtual void SetRenderViewPortSize_BCD45(uint8_t* ptrScreenBufferStart, uint16_t screenWidth, uint16_t viewPortWidth, uint16_t viewPortHeight) = 0;
	//virtual void SetRenderViewPortSize_40C50(uint8_t viewPortSizeSetting) = 0;
	//virtual void DrawWorld(uint8_t* pdwScreenBuffer, int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov) = 0;
	virtual void DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov) = 0;

	//virtual void SetRenderThreads(uint8_t renderThreads) = 0;
	//virtual uint8_t GetRenderThreads() = 0;

	//type_E9C38_smalltit Str_E9C38_smalltit[21 * 40];
	//type_F2C20ar str_F2C20ar;
};

#endif //GAME_RENDER_INTERFACE