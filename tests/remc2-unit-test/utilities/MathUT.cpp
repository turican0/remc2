#include <cstdint>
#include <gtest/gtest.h>
#include "remc2/utilities/Maths.h"

TEST(Math, Curve)
{
	std::vector<Maths::Zone> zones;
	zones.push_back(Maths::Zone{ 0,		4095, 0.5 });
	zones.push_back(Maths::Zone{ 4095,	8190, 0.5 });
	zones.push_back(Maths::Zone{ 8190,	12285, 0.5 });
	zones.push_back(Maths::Zone{ 12285, 16380, 0.5 });
	zones.push_back(Maths::Zone{ 16380, 20475, 0.5 });
	zones.push_back(Maths::Zone{ 20475, 28665, 0.5 });
	zones.push_back(Maths::Zone{ 28665, 32767, 0.5 });

	ASSERT_EQ(Maths::CurveCoords(0, 0, zones), 0);
	ASSERT_EQ(Maths::CurveCoords(4095, 4095, zones), 2047);
	ASSERT_EQ(Maths::CurveCoords(8190, 8190, zones), 4095);
	ASSERT_EQ(Maths::CurveCoords(12285, 12285, zones), 6142);
	ASSERT_EQ(Maths::CurveCoords(16380, 16380, zones), 8190);
	ASSERT_EQ(Maths::CurveCoords(20475, 20475, zones), 10237);
	ASSERT_EQ(Maths::CurveCoords(28665, 28665, zones), 14332);
	ASSERT_EQ(Maths::CurveCoords(32767, 32767, zones), 32767);

}
