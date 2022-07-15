#include "ViewPort.h"

ViewPort::ViewPort()
{
	PosX_EA3D0 = 0;
	PosY_EA3CC = 0;
	Width_DE564 = 0;
	Height_DE568 = 0;
	PreWidth_EA3C4 = 0;
	PreHeight_EA3C0 = 0;
};

ViewPort::ViewPort(const ViewPort& v)
{
	PosX_EA3D0 = v.PosX_EA3D0;
	PosY_EA3CC = v.PosY_EA3CC;
	Width_DE564 = v.Width_DE564;
	Height_DE568 = v.Height_DE568;

	PreWidth_EA3C4 = v.PreWidth_EA3C4;
	PreHeight_EA3C0 = v.PreHeight_EA3C0;
};

ViewPort::ViewPort(uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight)
{
	PosX_EA3D0 = viewPortPosX;
	PosY_EA3CC = viewPortPosY;
	Width_DE564 = viewPortWidth;
	Height_DE568 = viewPortHeight;
};

ViewPort::ViewPort(uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t viewPortPreWidth, uint16_t viewPortPreHeight)
{
	PosX_EA3D0 = viewPortPosX;
	PosY_EA3CC = viewPortPosY;
	Width_DE564 = viewPortWidth;
	Height_DE568 = viewPortHeight;

	PreWidth_EA3C4 = viewPortPreWidth;
	PreHeight_EA3C0 = viewPortPreHeight;
};

void ViewPort::SetRenderViewPortSize_40BF0(int width, int height, int viewPortWidth, int viewPortHeight)//221bf0
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

void ViewPort::SetRenderViewPortSize_40C50(uint8_t viewPortSizeSetting)//221c50
{
	int v1; // eax
	int v2; // esi
	int height; // eax
	int width; // bx

	double koefWidth = (double)screenWidth_18062C / 40;
	double koefHeight = (double)screenHeight_180624 / 40;
	v1 = 40 - viewPortSizeSetting;
	v2 = screenWidth_18062C * ((koefWidth * v1) / 2) + ((koefHeight * v1) / 2);
	width = koefWidth * viewPortSizeSetting;
	height = koefHeight * viewPortSizeSetting;
	str_F2C20ar.dword0x0e_ptrScreenRenderBufferStart = v2;
	SetRenderViewPortSize_BCD45(v2 + pdwScreenBuffer_351628, screenWidth_18062C, width, height);
}

void ViewPort::SetRenderViewPortSize_BCD45(uint8_t* ptrScreenBufferStart, uint16_t screenWidth, uint16_t viewPortWidth, uint16_t viewPortHeight)
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
		Width_DE564 = viewPortWidth;
	}

	if (viewPortHeight)
	{
		Height_DE568 = viewPortHeight;
	}
}

//----- (0002CA60) --------------------------------------------------------
void ViewPort::SetViewPortScreenCoordinates_2CA60(int16_t viewPortX, int16_t viewPortY, uint16_t viewPortWidth, uint16_t viewPortHeight)//20da60
{
	PosX_EA3D0 = viewPortX;
	PosY_EA3CC = viewPortY;
	PreWidth_EA3C4 = viewPortWidth;
	PreHeight_EA3C0 = viewPortHeight;
}

//----- (0002CA90) --------------------------------------------------------
void ViewPort::ResizeViewPort_2CA90(__int16 a1)//20da90
{
	int v1; // eax
	double widthKoef = (double)screenWidth_18062C / 80;
	double heightKoef = (double)screenHeight_180624 / 80;

	v1 = 40 - a1;
	PosX_EA3D0 = widthKoef * v1;
	PreWidth_EA3C4 = widthKoef * 2 * a1;
	PreHeight_EA3C0 = heightKoef * 2 * a1;
	PosY_EA3CC = heightKoef * v1;
}
