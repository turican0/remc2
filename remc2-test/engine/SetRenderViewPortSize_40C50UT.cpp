#include <cstdint>
#include <gtest/gtest.h>
#include "../../remc2/engine/ViewPort.cpp"

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
    EXPECT_EQ(viewPort1.PosY_EA3CC, 3);
    EXPECT_EQ(viewPort1.Width_DE564, 312);
    EXPECT_EQ(viewPort1.Height_DE568, 194);

    delete[] pdwScreenBuffer_351628;
}
