#include <gtest/gtest.h>
#include "../pch.h"
#include "SDL_stdinc.h"
#include "../../remc2/engine/ViewPort.cpp"
#include "../../remc2/engine/engine_support.cpp"
#include "../../remc2/portability/port_time.cpp"
#include "../../remc2/portability/port_filesystem.cpp"
#include "../../remc2/portability/dirent.c"
#include "../../remc2/portability/fcaseopen.cpp"
#include "../../remc2/portability/port_sdl_vga_mouse.cpp"
#include "../../remc2/portability/port_sdl_sound.cpp"


TEST(viewport, SetRenderViewPortSize_40C50) {

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
}
