#include "Graphics.h"

#include "Basic.h"

GameRenderInterface* m_ptrGameRender;

uint8_t vgaBuffer_A0000[307200];
TColor tempPalette_EA3B8x[256]; // weak?langStrings_E9C4C[475]
int16_t videoModeNumber_180662; // weak
bitmap_pos_struct2_t* webSpritesData_D4188 = 0; // weak
bitmap_pos_struct2_t* webSpritesTab_D418C = 0; // weak
bitmap_pos_struct2_t* webSpritesTabEnd_D4190 = 0; // weak


//----- (000A0D50) --------------------------------------------------------
void SetViewport_A0D50(uint16_t posX, uint16_t posY, uint16_t width, uint16_t height)//281d50
{
	viewportX_18063C = posX;
	viewportY_180650 = posY;
	clipWidth_180648 = width;
	clipHeight_180644 = height;
	viewportWidth_180634 = width + posX;
	viewportHeight_180630 = height + posY;
}
