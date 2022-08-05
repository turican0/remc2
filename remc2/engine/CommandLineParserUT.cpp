#include <gtest/gtest.h>

#include "CommandLineParser.h"


TEST (DebugParameters, CommandLineParser) { 
    int args = 3;
    char *with_reg_test[] = {
        (char*)"exe filename",
        (char*)"--test_regressions_game",
        (char*)"dummy",
        nullptr
    };
    char *without_reg_test[] = {
        (char*)"exe filename",
        (char*)"dummy1",
        (char*)"dummy2",
        nullptr
    };

    CommandLineParams.Init(args, with_reg_test);

    EXPECT_EQ (true , CommandLineParams.DoTestRegressionsGame());

    CommandLineParams.Init(args, without_reg_test);

    EXPECT_EQ (false , CommandLineParams.DoTestRegressionsGame());
}
