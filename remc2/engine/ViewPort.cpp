#include "ViewPort.h"
#include "defs.h"

//View Port Dimensions for game world render
ViewPort viewPort = ViewPort(0, 0, 0, 0, 0, 0);
//int iViewPortWidth_DE564 = 0;  // DE564 viewPort.Width_DE564
//int iViewPortHeight_DE568 = 0; // DE568 viewPort.Height_DE568
// //int iViewPortPosX_EA3D0; // weak?x_DWORD_E9C4C_langindexbuffer[481] viewPort.PosX_EA3D0
//int iViewPortPosY_EA3CC; // weak?x_DWORD_E9C4C_langindexbuffer[480] viewPort.PosY_EA3CC

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
	Rectangle rectangle = SetRenderViewPortSize_40C50(viewPortSizeSetting, screenWidth_18062C, screenHeight_180624);
	int32_t ptr = (screenWidth_18062C * rectangle.PosY_EA3CC) + rectangle.PosX_EA3D0;
	str_F2C20ar.dword0x0e_ptrScreenRenderBufferStart = ptr;
	SetRenderViewPortSize_BCD45(pdwScreenBuffer_351628 + ptr, screenWidth_18062C, rectangle.Width_DE564, rectangle.Height_DE568);
}

Rectangle ViewPort::SetRenderViewPortSize_40C50(uint8_t viewPortSizeSetting, uint32_t screenWidth, uint32_t screenHeight)//221c50
{
	Rectangle rectangle;

	int factor = 40 - viewPortSizeSetting; // eax
	double widthKoef = (double)screenWidth / 80;
	double heightKoef = (double)screenHeight / 80;

	rectangle.PosX_EA3D0 = widthKoef * factor;
	rectangle.PosY_EA3CC = heightKoef * factor;
	rectangle.Width_DE564 = widthKoef * 2 * viewPortSizeSetting;
	rectangle.Height_DE568 = heightKoef * 2 * viewPortSizeSetting;

	return rectangle;
}

void ViewPort::SetRenderViewPortSize_BCD45(uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t screenWidth, uint16_t screenHeight)
{
	iScreenWidth_DE560 = screenWidth;
	SetViewPortScreenCoordinates_2CA60(viewPortPosX, viewPortPosY, viewPortWidth, viewPortHeight);

	if (viewPortWidth > screenWidth)
	{
		Width_DE564 = screenWidth;
	}

	if (viewPortHeight > screenHeight)
	{
		Height_DE568 = screenHeight;
	}

	if (viewPortPosX > screenWidth)
	{
		PosX_EA3D0 = screenWidth - 1;
	}

	if (viewPortPosY > screenHeight)
	{
		PosY_EA3CC = screenHeight - 1;
	}

	int32_t ptrScreenRenderBufferStart = PosX_EA3D0 + iScreenWidth_DE560 * PosY_EA3CC;
	SetRenderViewPortSize_BCD45(ptrScreenRenderBufferStart + pdwScreenBuffer_351628, iScreenWidth_DE560, Width_DE564, Height_DE568);
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
	PreWidth_EA3C4 = Width_DE564;
	PreHeight_EA3C0 = Height_DE568;
	PosX_EA3D0 = viewPortX;
	PosY_EA3CC = viewPortY;
	Width_DE564 = viewPortWidth;
	Height_DE568 = viewPortHeight;
}

//----- (0002CA90) --------------------------------------------------------
void ViewPort::ResizeViewPort_2CA90(uint8_t viewPortSizeSetting)//20da90
{
	int factor = 40 - viewPortSizeSetting; // eax
	double widthKoef = (double)screenWidth_18062C / 80;
	double heightKoef = (double)screenHeight_180624 / 80;

	PosX_EA3D0 = widthKoef * factor;
	PosY_EA3CC = heightKoef * factor;
	PreWidth_EA3C4 = widthKoef * 2 * viewPortSizeSetting;
	PreHeight_EA3C0 = heightKoef * 2 * viewPortSizeSetting;
}
