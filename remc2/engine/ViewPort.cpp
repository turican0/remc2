#include "ViewPort.h"

//View Port Dimensions for game world render
ViewPort viewPort = { 0,0,0,0,0,0 };
//int iViewPortWidth_DE564 = 0;  // DE564 viewPort.Width_DE564
//int iViewPortHeight_DE568 = 0; // DE568 viewPort.Height_DE568
// //int iViewPortPosX_EA3D0; // weak?x_DWORD_E9C4C_langindexbuffer[481] viewPort.PosX_EA3D0
//int iViewPortPosY_EA3CC; // weak?x_DWORD_E9C4C_langindexbuffer[480] viewPort.PosY_EA3CC

void SetRenderViewPortSize_40BF0(int width, int height, int viewPortWidth, int viewPortHeight)//221bf0
{
	int width_help;
	int height_help;
	int viewPortWidth_help;
	int viewPortHeight_help;

	width_help = width;
	height_help = height;
	viewPortWidth_help = viewPortWidth;
	viewPortHeight_help = viewPortHeight;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		width_help = width >> 1;
		height_help = height >> 1;
		viewPortWidth_help = viewPortWidth >> 1;
		viewPortHeight_help = viewPortHeight >> 1;
	}
	str_F2C20ar.dword0x0e_ptrScreenRenderBufferStart = width_help + screenWidth_18062C * height_help;
	SetRenderViewPortSize_BCD45(str_F2C20ar.dword0x0e_ptrScreenRenderBufferStart + pdwScreenBuffer_351628, screenWidth_18062C, viewPortWidth_help, viewPortHeight_help);
}

void SetRenderViewPortSize_40C50(uint8_t viewPortSizeSetting)//221c50
{
	int v1; // eax
	int v2; // esi
	int height; // eax
	int width; // bx

	double koefWidth = (double)screenWidth_18062C / 40;
	double koefHeight = (double)screenHeight_180624 / 40;
	v1 = 40 - viewPortSizeSetting;
	v2 = screenWidth_18062C * ((int)((koefWidth * v1) / 2)) + ((koefHeight * v1) / 2);
	width = koefWidth * viewPortSizeSetting;
	height = koefHeight * viewPortSizeSetting;
	str_F2C20ar.dword0x0e_ptrScreenRenderBufferStart = v2;
	SetRenderViewPortSize_BCD45(v2 + pdwScreenBuffer_351628, screenWidth_18062C, width, height);
}

void SetRenderViewPortSize_BCD45(uint8_t* ptrScreenBufferStart, uint16_t screenWidth, uint16_t viewPortWidth, uint16_t viewPortHeight)
{
	if (screenWidth)
	{
		iScreenWidth_DE560 = screenWidth;
	}

	if (ptrScreenBufferStart)
	{
		ViewPortRenderBufferStart_DE558 = ptrScreenBufferStart;
		ViewPortRenderBufferAltStart_DE554 = ptrScreenBufferStart - iScreenWidth_DE560;
	}

	if (viewPortWidth)
	{
		viewPort.Width_DE564 = viewPortWidth;
	}

	if (viewPortHeight)
	{
		viewPort.Height_DE568 = viewPortHeight;
	}
}

//----- (0002CA60) --------------------------------------------------------
void SetViewPortScreenCoordinates_2CA60(int16_t viewPortX, int16_t viewPortY, uint16_t viewPortWidth, uint16_t viewPortHeight)//20da60
{
	viewPort.PosX_EA3D0 = viewPortX;
	viewPort.PosY_EA3CC = viewPortY;
	viewPort.PreWidth_EA3C4 = viewPortWidth;
	viewPort.PreHeight_EA3C0 = viewPortHeight;
}

//----- (0002CA90) --------------------------------------------------------
void ResizeViewPort_2CA90(__int16 a1)//20da90
{
	int v1; // eax
	double widthKoef = (double)screenWidth_18062C / 80;
	double heightKoef = (double)screenHeight_180624 / 80;

	v1 = 40 - a1;
	viewPort.PosX_EA3D0 = widthKoef * v1;
	viewPort.PreWidth_EA3C4 = widthKoef * 2 * a1;
	viewPort.PreHeight_EA3C0 = heightKoef * 2 * a1;
	viewPort.PosY_EA3CC = heightKoef * v1;
	//return result;
}
