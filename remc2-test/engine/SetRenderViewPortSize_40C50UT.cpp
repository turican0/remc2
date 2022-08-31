#include <cstdint>
#include <gtest/gtest.h>
#include "../../remc2/engine/ViewPort.h"

uint8_t* pdwScreenBuffer_351628;
int16_t x_WORD_180660_VGA_type_resolution;
type_F2C20ar str_F2C20ar;
uint32_t screenHeight_180624;
uint32_t screenWidth_18062C;
int iScreenWidth_DE560;
uint8_t* ViewPortRenderBufferStart_DE558;
uint8_t* ViewPortRenderBufferAltStart_DE554;

TEST(viewport, SetRenderViewPortSize_40C50) {
    pdwScreenBuffer_351628 = new uint8_t[1920*1080*2];
    ViewPort viewPort;

    ViewPort::Rectangle viewPort1 = viewPort.SetRenderViewPortSize_40C50(39, 320, 200);
    ViewPort::Rectangle viewPort2 = viewPort.SetRenderViewPortSize_40C50(38, 320, 200);

    ViewPort::Rectangle viewPort3 = viewPort.SetRenderViewPortSize_40C50(38, 640, 480);

    ViewPort::Rectangle viewPort4 = viewPort.SetRenderViewPortSize_40C50(40, 960, 540);
    ViewPort::Rectangle viewPort5 = viewPort.SetRenderViewPortSize_40C50(38, 960, 540);
    ViewPort::Rectangle viewPort6 = viewPort.SetRenderViewPortSize_40C50(37, 960, 540);

    ViewPort::Rectangle viewPort7 = viewPort.SetRenderViewPortSize_40C50(40, 1920, 1080);
    ViewPort::Rectangle viewPort8 = viewPort.SetRenderViewPortSize_40C50(38, 1920, 1080);

    EXPECT_EQ(viewPort1.PosX_EA3D0, 4);
    EXPECT_EQ(viewPort1.PosY_EA3CC, 2);
    EXPECT_EQ(viewPort1.Width_DE564, 312);
    EXPECT_EQ(viewPort1.Height_DE568, 195);

    EXPECT_EQ(viewPort2.PosX_EA3D0, 8);
    EXPECT_EQ(viewPort2.PosY_EA3CC, 5);
    EXPECT_EQ(viewPort2.Width_DE564, 304);
    EXPECT_EQ(viewPort2.Height_DE568, 190);

    EXPECT_EQ(viewPort3.PosX_EA3D0, 16);
    EXPECT_EQ(viewPort3.PosY_EA3CC, 12);
    EXPECT_EQ(viewPort3.Width_DE564, 608);
    EXPECT_EQ(viewPort3.Height_DE568, 456);

    EXPECT_EQ(viewPort4.PosX_EA3D0, 0);
    EXPECT_EQ(viewPort4.PosY_EA3CC, 0);
    EXPECT_EQ(viewPort4.Width_DE564, 960);
    EXPECT_EQ(viewPort4.Height_DE568, 540);

    EXPECT_EQ(viewPort5.PosX_EA3D0, 24);
    EXPECT_EQ(viewPort5.PosY_EA3CC, 13);
    EXPECT_EQ(viewPort5.Width_DE564, 912);
    EXPECT_EQ(viewPort5.Height_DE568, 513);

    EXPECT_EQ(viewPort6.PosX_EA3D0, 36);
    EXPECT_EQ(viewPort6.PosY_EA3CC, 20);
    EXPECT_EQ(viewPort6.Width_DE564, 888);
    EXPECT_EQ(viewPort6.Height_DE568, 499);

    EXPECT_EQ(viewPort7.PosX_EA3D0, 0);
    EXPECT_EQ(viewPort7.PosY_EA3CC, 0);
    EXPECT_EQ(viewPort7.Width_DE564, 1920);
    EXPECT_EQ(viewPort7.Height_DE568, 1080);

    EXPECT_EQ(viewPort8.PosX_EA3D0, 48);
    EXPECT_EQ(viewPort8.PosY_EA3CC, 27);
    EXPECT_EQ(viewPort8.Width_DE564, 1824);
    EXPECT_EQ(viewPort8.Height_DE568, 1026);

    delete[] pdwScreenBuffer_351628;
}
