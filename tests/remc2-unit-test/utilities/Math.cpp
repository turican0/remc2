#include <cstdint>
#include <gtest/gtest.h>
#include "utilities/Maths.h"

unsigned int pseudoRand()
{
	static unsigned int nSeed = 5323;
	nSeed = (8253729 * nSeed + 2396403);
	return nSeed % 32767;
}

TEST(Math, RotateX)
{
	Maths::Vec3 point{ 0, 1, 1 };
	auto rotatedPoint = Maths::RotateX(point, 90);

	ASSERT_EQ((int)round(rotatedPoint.x), 0);
	ASSERT_EQ((int)round(rotatedPoint.y), -1);
	ASSERT_EQ((int)round(rotatedPoint.z), 1);
}

TEST(Math, RotateY)
{
	Maths::Vec3 point{ 1, 0, 1 };
	auto rotatedPoint = Maths::RotateY(point, 90);

	ASSERT_EQ((int)round(rotatedPoint.x), 1);
	ASSERT_EQ((int)round(rotatedPoint.y), 0);
	ASSERT_EQ((int)round(rotatedPoint.z), -1);
}

TEST(Math, RotateZ)
{
	Maths::Vec3 point{ 1, 1, 0};
	auto rotatedPoint = Maths::RotateZ(point, 90);

	ASSERT_EQ((int)round(rotatedPoint.x), -1);
	ASSERT_EQ((int)round(rotatedPoint.y), 1);
	ASSERT_EQ((int)round(rotatedPoint.z), 0);
}

TEST(Math, MeasureYawAngleDegrees)
{
	auto degrees = Maths::MeasureYawAngleDegrees(Maths::Vec3{ 0, -1, 0 });
	ASSERT_EQ((int)round(degrees), 0);

	degrees = Maths::MeasureYawAngleDegrees(Maths::Vec3 { 1, -1, 0 });
	ASSERT_EQ((int)round(degrees), 45);

	degrees = Maths::MeasureYawAngleDegrees(Maths::Vec3 { 1, 0, 0 });
	ASSERT_EQ((int)round(degrees), 90);

	degrees = Maths::MeasureYawAngleDegrees(Maths::Vec3{ 0, 1, 0 });
	ASSERT_EQ((int)round(degrees), 180);

	degrees = Maths::MeasureYawAngleDegrees(Maths::Vec3{ -1, 1, 0 });
	ASSERT_EQ((int)round(degrees), 225);

	degrees = Maths::MeasureYawAngleDegrees(Maths::Vec3{ -1, 0, 0 });
	ASSERT_EQ((int)round(degrees), 270);

	degrees = Maths::MeasureYawAngleDegrees(Maths::Vec3{ -1, -1, 0 });
	ASSERT_EQ((int)round(degrees), 315);
}

TEST(Math, CalculateAngleFromPosition)
{
	auto playerYawDeg = 0;
	auto rotatedCoords = Maths::RotateZ(Maths::Vec3{ 10000, -10000, 0 }, -playerYawDeg);
	float degrees = Maths::MeasureYawAngleDegrees(rotatedCoords);
	ASSERT_EQ((int)round(degrees), 45);

	playerYawDeg = 45;
	rotatedCoords = Maths::RotateZ(Maths::Vec3{ 10000, -10000, 0 }, -playerYawDeg);
	degrees = Maths::MeasureYawAngleDegrees(rotatedCoords);
	ASSERT_EQ((int)round(degrees), 0);

	rotatedCoords = Maths::RotateZ(Maths::Vec3{ -10000, -10000, 0 }, -playerYawDeg);
	degrees = Maths::MeasureYawAngleDegrees(rotatedCoords);
	ASSERT_EQ((int)round(degrees), 270);
}
