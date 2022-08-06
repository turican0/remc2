#include <gtest/gtest.h>

#include "CommandLineParser.h"


TEST (DebugParameters, CommandLineParser) { 
    int args = 3;
    char *with_test_reg_game_group[] = {
        (char*)"exe filename",
        (char*)"--test_regressions_game",
        (char*)"dummy",
        nullptr
    };
    char *without_group[] = {
        (char*)"exe filename",
        (char*)"dummy1",
        (char*)"dummy2",
        nullptr
    };

    CommandLineParams.Init(args, with_test_reg_game_group);

    EXPECT_EQ (true, CommandLineParams.ModeTestRegressionsGame());
    EXPECT_EQ (true, CommandLineParams.DoCopySkipConfig());      // set by group
    EXPECT_EQ (true, CommandLineParams.DoFixMouse());            // set by group
    EXPECT_EQ (false, CommandLineParams.DoLoadEditedLevel());    // not set by group

    CommandLineParams.Init(args, without_group);

    EXPECT_EQ (true, CommandLineParams.ModeReleaseGame());          // default group if no other group is selected
    EXPECT_EQ (false, CommandLineParams.ModeTestRegressionsGame()); // group should not be set
}
