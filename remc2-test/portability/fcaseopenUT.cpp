#include <gtest/gtest.h>
#include "../pch.h"
#include "../../remc2/portability/fcaseopen.h"
TEST (fcaseopen, tokens) {
#ifdef __linux__
    std::string path1 = "/asdf/dummy/file.c";
    std::string path2 = "./dummy2/file2.c";

    auto tokens1 = GetTokensFromPath(path1);
    auto tokens2 = GetTokensFromPath(path2);

    EXPECT_EQ ("asdf", tokens1[0]);
    EXPECT_EQ ("dummy", tokens1[1]);
    EXPECT_EQ ("file.c", tokens1[2]);
    EXPECT_EQ (".", tokens2[0]);
    EXPECT_EQ ("dummy2", tokens2[1]);
    EXPECT_EQ ("file2.c", tokens2[2]);
#else //__linux__
    EXPECT_EQ(1, 1);
#endif //__linux__
}

TEST (fcaseopen, casepath) {
#ifdef __linux__
    EXPECT_EQ(casepath("/usr/share"), "/usr/share");
    EXPECT_EQ(casepath("/usr/share/"), "/usr/share/");
    EXPECT_EQ(casepath("/usr/../usr/share"), "/usr/../usr/share");
    EXPECT_EQ(casepath("/USR/SHARE"), "/usr/share");
    EXPECT_EQ(casepath("remc2-test"), "remc2-test");
    EXPECT_EQ(casepath("./remc2-test"), "./remc2-test");
    EXPECT_EQ(casepath("./REMC2-test"), "./remc2-test");
    EXPECT_EQ(casepath("./not_existing_file"), "./not_existing_file");
#else //__linux__
    EXPECT_EQ(1, 1);
#endif //__linux__
}
