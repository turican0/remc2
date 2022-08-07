#include <gtest/gtest.h>

#include "CommandLineParser.h"


TEST (DebugParameters, CommandLineParser) { 
    int args = 3;
    char *with_test_reg_game_group[] = {
        (char*)"exe filename",
        (char*)"--mode_test_regressions_game",
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
        (char*)"--memimage_path",
        (char*)"/home/user/remc2/memimages",
        nullptr
    };

    CommandLineParams.Init(args, with_test_reg_game_group);

    EXPECT_EQ (true, CommandLineParams.ModeTestRegressionsGame());
    EXPECT_EQ (true, CommandLineParams.DoCopySkipConfig());      // set by mode
    EXPECT_EQ (true, CommandLineParams.DoFixMouse());            // set by mode
    EXPECT_EQ (false, CommandLineParams.DoLoadEditedLevel());    // not set by mode

    CommandLineParams.Init(args, without_group);

    EXPECT_EQ (true, CommandLineParams.ModeReleaseGame());          // default mode if no other mode is selected
    EXPECT_EQ (false, CommandLineParams.ModeTestRegressionsGame()); // this mode should not be set

    CommandLineParams.Init(args, memimage_check);

    EXPECT_EQ ("/home/user/remc2/memimages", CommandLineParams.GetMemimagePath());
}
