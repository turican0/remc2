#pragma once

#ifndef GAME_RENDER_OLD
#define GAME_RENDER_OLD

#include "GameRenderInterface.h"

#include <array>
#include <thread>

#include "../utilities/Maths.h"
#include "../utilities/BitmapIO.h"
#include "../portability/mctypes.h"
#include "../utilities/SafeQueue.h"
#include "Terrain.h"
#include "Type_F2C20ar.h"
#include "Type_E9C38_Smalltit.h"
#include "Type_Unk_F0E20x.h"
#include "Type_WORD_D951C.h"
#include "XUnk_D4350.h"
#include "Type_D404C.h"
#include "Type_D94F0_Bldgprmbuffer.h"
#include "Type_D93C0_Bldgprmbuffer.h"
#include "TextureMaps.h"
#include "defs.h"
#include "RenderThread.h"
#include "ProjectionPolygon.h"

class GameRenderOriginal : public GameRenderInterface
{

private:

	uint8_t unk_D4328x[40] = {
		0xED,0x01,0x00,0x00,0x00,0xFF,0xD8,0xFF,0x01,0x00,0x00,0xED,0xFF,0x00,0x01,0x00,
		0x01,0xD8,0x00,0x01,0x13,0x00,0xFF,0xFF,0x00,0x01,0x28,0x01,0xFF,0x00,0x01,0x13,
		0x00,0xFF,0xFF,0x00,0xFF,0x28,0x00,0xFF
	};//2a5328

	uint8_t unk_DE56Cx[1][4194304]; //Number of possible render threads (8) //number of polygons (2048 * 2048)
	uint32_t offsets_B8845[16] = {
		0x00000000,0xFFFFFFF1,0xFFFFFFF2,0xFFFFFFF3,
		0xFFFFFFF4,0xFFFFFFF5,0xFFFFFFF6,0xFFFFFFF7,
		0xFFFFFFF8,0xFFFFFFF9,0xFFFFFFFA,0xFFFFFFFB,
		0xFFFFFFFC,0xFFFFFFFD,0xFFFFFFFE,0xFFFFFFFF
	};

	char x_BYTE_D4750[60] = {
		0x00,0x00,0x02,0x03,0x04,0x05,0x01,0x00,0x06,0x07,0x04,0x05,0x00,0x01,0x01,0x02,
		0x02,0x03,0x03,0x04,0x04,0x03,0x03,0x02,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x01,
		0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x03,0x03,0x03,0x03,
		0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0C,0x0C,0x0C
	};

	type_unk_F0E20x m_str_F0E20x[1920]; // Originally 640

	std::array<uint8_t*, 256> m_textureAddresses;
	
	int x_DWORD_D4794 = 0;
	int x_DWORD_D4798 = 0;
	char x_BYTE_E126D = 0;
	char x_BYTE_E126C = 112;
	int x_DWORD_D4790 = 20;
	int x_DWORD_D4324 = 0;
	char shadows_F2CC7 = 0;
	char notDay_D4320 = 0;
	char x_BYTE_F2CC6 = 0;
	int16_t x_WORD_F2CC0 = 0;
	int16_t x_WORD_F2CC2 = 0;
	int16_t x_WORD_F2CC4 = 0;
	uint8_t* x_DWORD_DE55C_ActTexture = 0;

	void DrawSky_40950(int16_t roll);
	void DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov);
	void DrawSprite_41BD3(uint32 a1);
	void DrawParticles_3E360(int a2x);
	void DrawTriangleInProjectionSpace_B6253(x_DWORD* a1, x_DWORD* a2, x_DWORD* a3);
	int32_t* x_DWORD_DB350_ret(uint32_t adress);
	uint16_t sub_3FD60(int a2x);
	void sub_88740(type_event_0x6E8E* a1x, int a2, int a3);
	void SetBillboards_3B560(int16_t roll);
	void DrawSorcererNameAndHealthBar_2CB30(type_event_0x6E8E* a1x, __int16 a2, int a3, __int16 a4);

public:
	~GameRenderOriginal();
	void DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov);
};

#endif //GAME_RENDER_OLD