#include <gtest/gtest.h>

#include "engine/CommandLineParser.h"


TEST (CommandLineParser, DebugParameters) { 
    int args = 3;
    char *with_test_reg_game_group[] = {
        (char*)"exe filename",
        (char*)"--mode_test_regressions",
        (char*)"2",
        (char*)"dummy",
        nullptr
    };
    char *without_group[] = {
        (char*)"exe filename",
        (char*)"dummy1",
        (char*)"dummy2",
        nullptr
    };
    char *memimage_check[] = {
        (char*)"exe filename",
        (char*)"--memimages_path",
        (char*)"/home/user/remc2/memimages",
        nullptr
    };

    CommandLineParams.Init(args, with_test_reg_game_group);

    EXPECT_EQ (2, CommandLineParams.ModeRegressionsTestType());
    EXPECT_EQ (true, CommandLineParams.DoCopySkipConfig());      // set by mode
    EXPECT_EQ (true, CommandLineParams.DoFixMouse());            // set by mode
    EXPECT_EQ (false, CommandLineParams.DoLoadEditedLevel());    // not set by mode

    CommandLineParams.Init(args, without_group);

    EXPECT_EQ (true, CommandLineParams.ModeReleaseGame());          // default mode if no other mode is selected
    EXPECT_EQ (-1, CommandLineParams.ModeRegressionsTestType()); // this mode should not be set

    CommandLineParams.Init(args, memimage_check);

    EXPECT_EQ ("/home/user/remc2/memimages", CommandLineParams.GetMemimagesPath());
}
