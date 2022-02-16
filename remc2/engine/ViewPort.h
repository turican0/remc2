#pragma once

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <cstdint>

struct ViewPort
{
	uint16_t PosX = 0;
	uint16_t PosY = 0;
	uint16_t Width = 0;
	uint16_t Height = 0;

	ViewPort()
	{
		PosX = 0;
		PosY = 0;
		Width = 0;
		Height = 0;
	};

	ViewPort(const ViewPort &v)
	{
		PosX = v.PosX;
		PosY = v.PosY;
		Width = v.Width;
		Height = v.Height;
	};

	ViewPort(uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight)
	{
		PosX = viewPortPosX;
		PosY = viewPortPosY;
		Width = viewPortWidth;
		Height = viewPortHeight;
	};
};

#endif //VIEWPORT_H