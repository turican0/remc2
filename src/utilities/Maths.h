#pragma once

#ifndef MATHS_H
#define MATHS_H

#include <vector>
#include <cmath>
#ifdef REMC2_CODE
#include <SDL2/SDL_stdinc.h>
#else
#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288   /**< pi */
#endif
#endif
#include "engine/axis_3d.h"

class Maths {

private:

	static uint16_t x_WORD_727B0[224];
	static uint16_t x_WORD_DE350[257];

public:

	struct Zone {
		uint16_t m_xStart;
		uint16_t m_xEnd;
		double m_factor;
	};

	struct Vec3 {
		float x, y, z;
	};

	static int32_t sin_DB750[2560]; //2ac750 //speed table1 //sin and cos table
	static char x_BYTE_D41D8[328];

	static unsigned int EuclideanDistXYZ_58490(axis_3d* a1, axis_3d* a2);
	static unsigned int sub_7277A_radix_3d(unsigned int a1);
	static int16_t sub_58210_radix_tan(axis_3d* a1, axis_3d* a2);
	static uint16_t sub_72633_maybe_tan(int16_t a1, int16_t a2);
	static int16_t sub_581E0_maybe_tan2(axis_3d* a1, axis_3d* a2);
	static void x_BitScanReverse(uint32_t* Destination, uint32_t Source);
	static int SubtrackUntilZero(int x, int y);
	static uint16_t CurveCoords(uint16_t x, uint16_t y, std::vector<Maths::Zone>& zones);
	static uint32_t EuclideanDistXY_584D0(axis_3d* a1, axis_3d* a2);
	static uint32_t EuclideanDistXYFromZero(axis_3d_32* a1);
	static axis_3d_32 RelativeXYZCoordinate(axis_3d* a1, axis_3d* a2);
	static float ConvertYawToDegrees(int16_t yaw);
	static Vec3 RotateX(const Vec3& v, float angle);
	static Vec3 RotateY(const Vec3& v, float angle);
	static Vec3 RotateZ(const Vec3& v, float angle);
	static float MeasureYawAngleDegrees(const Vec3& v);
	static int32_t Abs16(int16_t value);
};

#endif //MATHS_H
