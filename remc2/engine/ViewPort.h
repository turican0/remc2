#pragma once

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <cstdint>

#include "Basic.h"

class ViewPort
{
public:
	uint16_t PreWidth_EA3C4 = 0;
	uint16_t PreHeight_EA3C0 = 0;
	//uint16_t x_DWORD_EA3C8 = 0;
	uint16_t PosX_EA3D0 = 0;
	uint16_t PosY_EA3CC = 0;

	uint16_t Width_DE564 = 0;
	uint16_t Height_DE568 = 0;


	ViewPort();
	ViewPort(const ViewPort& v);
	ViewPort(uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight);
	ViewPort(uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t viewPortPreWidth, uint16_t viewPortPreHeight);
	
	void SetRenderViewPortSize_40BF0(int width, int height, int viewPortWidth, int viewPortHeight);//221bf0
	void SetViewPortScreenCoordinates_2CA60(int16_t viewPortX, int16_t viewPortY, uint16_t viewPortWidth, uint16_t viewPortHeight);//20da60
	void ResizeViewPort_2CA90(uint8_t viewPortSizeSetting);//20da90
	void SetRenderViewPortSize_BCD45(uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t screenWidth, uint16_t screenHeight);
	void SetRenderViewPortSize_BCD45(uint8_t* ptrScreenBufferStart, uint16_t screenWidth, uint16_t viewPortWidth, uint16_t viewPortHeight);
	void SetRenderViewPortSize_40C50(uint8_t viewPortSizeSetting);
};

extern ViewPort viewPort;

#endif //VIEWPORT_H