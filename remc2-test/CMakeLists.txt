project(remc2-test C CXX)

if (USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-unused-parameters")
endif ()

################################################################################
# Source groups
################################################################################
set(SOURCE_FILES_UNITTESTS
    "../remc2/engine/CommandLineParser.cpp"
    "../remc2/engine/CommandLineParser.h"
    "engine/CommandLineParserUT.cpp"
    "../remc2/portability/fcaseopen.cpp"
    "../remc2/portability/fcaseopen.h"
    "portability/fcaseopenUT.cpp"
    "../remc2/engine/ViewPort.cpp"
    "engine/SetRenderViewPortSize_40C50UT.cpp"
    "../remc2/engine/Terrain.cpp"
    "../remc2/engine/Terrain.h"
    "comparison/engine/OriginalTerrain.cpp"
    "comparison/engine/OriginalTerrain.h"
    "engine/TerrainUT.cpp"
    "UnitTests.cpp"
)
source_group("Source Files\\unittests" FILES ${SOURCE_FILES_UNITTESTS})

set(ROOT_NAMESPACE remc2-test)


################################################################################
# Unit tests
################################################################################
include(GoogleTest)

add_executable(${PROJECT_NAME} ${SOURCE_FILES_UNITTESTS})
target_include_directories(${PROJECT_NAME} PUBLIC
    "../remc2/engine"
    "../remc2/portability"
)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    gtest
    pthread
)
add_compile_definitions(COMPILE_FOR_64BIT)
gtest_add_tests(
    TARGET      ${PROJECT_NAME}
    TEST_SUFFIX .noArgs
    TEST_LIST   noArgsTests
)
set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)
install(
    TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug;Release;RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)


if (USE_SANITIZERS)
    set(SANITIZER_OPTIONS
        -fsanitize=address
        -fsanitize=pointer-compare
        -fsanitize=pointer-subtract
        -fsanitize=leak
        -fsanitize=undefined
        -static-libasan
        -static-liblsan
        -static-libubsan
    )
    target_link_options(${PROJECT_NAME} PRIVATE ${SANITIZER_OPTIONS})
endif ()