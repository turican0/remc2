#include "GameRenderInterface.h"

GameRenderInterface::GameRenderInterface()
{
	m_viewPort = ViewPort(0, 0, 0, 0);
}

ViewPort* GameRenderInterface::GetViewPort()
{
	return &m_viewPort;
}
