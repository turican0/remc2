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
	GameRenderInterface();
	virtual ~GameRenderInterface() {}
	virtual void DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov) = 0;
	ViewPort* GetViewPort();
protected:
	ViewPort m_viewPort;
};

#endif //GAME_RENDER_INTERFACE