#include <gtest/gtest.h>
#include "pch.h"
#include "portability/fcaseopen.h"
#include <algorithm>
std::string g_exe_path;
TEST (fcaseopen, tokens) {
#ifdef __linux__
    std::string path1 = "/asdf/dummy/file.c";
    std::string path2 = "./dummy2/file2.c";

    auto tokens1 = GetTokensFromPath(path1);
    auto tokens2 = GetTokensFromPath(path2);

    ASSERT_EQ("asdf", tokens1[0]);
    ASSERT_EQ("dummy", tokens1[1]);
    ASSERT_EQ("file.c", tokens1[2]);
    ASSERT_EQ(".", tokens2[0]);
    ASSERT_EQ("dummy2", tokens2[1]);
    ASSERT_EQ("file2.c", tokens2[2]);
#else //__linux__
    ASSERT_EQ(1, 1);
#endif //__linux__
}

TEST (fcaseopen, casepath) {
#ifdef __linux__
    ASSERT_EQ(casepath("/usr/share"), "/usr/share");
    ASSERT_EQ(casepath("/usr/share/"), "/usr/share/");
    ASSERT_EQ(casepath("/usr/../usr/share"), "/usr/../usr/share");
    ASSERT_EQ(casepath("/USR/SHARE"), "/usr/share");
	std::string lower = g_exe_path;
	std::transform(lower.begin(), lower.end(), lower.begin(),[](unsigned char c) { return std::tolower(c); });
	std::string upper = g_exe_path;
	std::transform(upper.begin(), upper.end(), upper.begin(),[](unsigned char c) { return std::toupper(c); });
	ASSERT_EQ(casepath(upper), lower);
    ASSERT_EQ(casepath("./not_existing_file"), "./not_existing_file");
#else //__linux__
    ASSERT_EQ(1, 1);
#endif //__linux__
}
