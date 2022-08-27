#pragma once

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Rectangle.h"
#include "Basic.h"

class ViewPort: public Rectangle
{
public:
	uint16_t PreWidth_EA3C4 = 0;
	uint16_t PreHeight_EA3C0 = 0;

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
	ViewPort::Rectangle SetRenderViewPortSize_40C50(uint8_t viewPortSizeSetting, uint32_t screenWidth, uint32_t screenHeight);
};

extern ViewPort viewPort;

#endif //VIEWPORT_H