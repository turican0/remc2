#include <cstdint>
#include <gtest/gtest.h>
#include "../../remc2/engine/Basic.h"
#include "../../remc2/engine/GameRenderHD.h"

typedef struct
{
	int32_t X;
	int32_t Y;
	int32_t U;
	int32_t V;
} lineType;

TEST(GameRenderHD, CalculateRotationTranslationX)
{
	//Arrange
	spriteRender_F2C20.cos_0x11 = 65536;
	terrainTiles_E9C38[0].pnt1_16 = 33637;
	spriteRender_F2C20.sin_0x0d = 0;
	terrainTiles_E9C38[0].pnt2_20 = -9735;
	spriteRender_F2C20.centerX_0x24 = 960;

	//Run original code
	int32_t pnt1_16o = ((spriteRender_F2C20.cos_0x11 * terrainTiles_E9C38[0].pnt1_16 - spriteRender_F2C20.sin_0x0d * terrainTiles_E9C38[0].pnt2_20) >> 16) + spriteRender_F2C20.centerX_0x24;

	//Run current code
	int32_t pnt1_16n = GameRenderHD::CalculateRotationTranslationX(spriteRender_F2C20.cos_0x11, terrainTiles_E9C38[0].pnt1_16, spriteRender_F2C20.sin_0x0d, terrainTiles_E9C38[0].pnt2_20);

	//Check results match
	ASSERT_EQ(pnt1_16o, -30939);
	ASSERT_EQ(pnt1_16n, 34597);
}

TEST(GameRenderHD, TextureIndexU)
{
	//Arrange
	lineType* line = new lineType();
	line->X = 41153240;
	line->Y = 53215268;
	line->U = 368126;
	line->V = 1729025;

	const char* bytesArray = (char*)line;

	//Run current code
	int32_t textureIndexU_New = 5375;
	LOBYTE(textureIndexU_New) = BYTE2(line->U);

	//Run original code
	int32_t textureIndexU_Old = 5375;
	bytesArray = (char*)line;
	LOBYTE(textureIndexU_Old) = bytesArray[10];

	//Check results match
	ASSERT_EQ(textureIndexU_Old, 5125);
	ASSERT_EQ(textureIndexU_New, 5125);

	delete line;
}