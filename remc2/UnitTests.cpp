#include <gtest/gtest.h>

#include "engine/engine_support.h"

TEST (StructuresSizeTest, Engine) { 
    EXPECT_EQ (224791 , sizeof(type_D41A0_BYTESTR_0));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}