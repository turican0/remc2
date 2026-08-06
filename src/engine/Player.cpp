#include "Player.h"

#include "utilities/Maths.h"

//----- (00057FA0) --------------------------------------------------------
void MoveEntity_57FA0(axis_3d* position, uint16_t yaw, int16_t pitch, int16_t speed)//238fa0 // move player
{
	if (speed)
	{
		pitch &= 0x7ffu;
		yaw &= 0x7ffu;
		if (pitch)
		{
			position->z -= (int)(speed * Maths::sin_DB750[pitch]) >> 16;//change z axis
			speed = (int)(speed * Maths::sin_DB750[0x200 + pitch]) >> 16;
		}
		position->x += (int)(speed * Maths::sin_DB750[yaw]) >> 16;
		position->y -= (int)(speed * Maths::sin_DB750[0x200 + yaw]) >> 16;
	}
}
