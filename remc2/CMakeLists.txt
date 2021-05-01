project(remc2 C CXX)

if (USE_CLANG_TIDY)
    #set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
    #set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-unused-parameters;-fix")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-unused-parameters")
endif ()

################################################################################
# Source groups
################################################################################
if (WIN32)
    set(HEADER_FILES
        "stdafx.h"
        "targetver.h"
    )
else ()
    set(HEADER_FILES
    )
endif ()
source_group("Header Files" FILES ${HEADER_FILES})

if (WIN32)
    set(SOURCE_FILES
        "remc2.cpp"
        "stdafx.cpp"
    )
else()
    set(SOURCE_FILES
        "remc2.cpp"
    )
endif()
source_group("Source Files" FILES ${SOURCE_FILES})

set(SOURCE_FILES_ENGINE
    "engine/ail_sound.h"
    "engine/defs.h"
    "engine/engine_support.cpp"
    "engine/engine_support.h"
    "engine/engine_support_converts.cpp"
    "engine/engine_support_converts.h"
    "engine/Type_WORD_D951C.cpp"
    "engine/Type_WORD_D951C.h"
    "engine/Type_D93C0_Bldgprmbuffer.cpp"
    "engine/Type_D93C0_Bldgprmbuffer.h"
    "utilities/Maths.cpp"
    "utilities/Maths.h"
    "utilities/BitmapIO.cpp"
    "utilities/BitmapIO.h"
    "utilities/DataFileIO.cpp"
    "utilities/DataFileIO.h"
    "engine/read_config.cpp"
    "engine/read_config.h"
    "engine/GameRender.cpp"
    "engine/GameRender.h"
    "sub_main.cpp"
    "sub_main.h"
    "engine/Sound.cpp"
    "engine/Sound.h"
    "engine/Basic.cpp"
    "engine/Basic.h"
    "engine/Animation.cpp"
    "engine/Animation.h"
    "engine/Terrain.cpp"
    "engine/Terrain.h"
    "engine/sub_main_mouse.h"
)
source_group("Source Files\\engine" FILES ${SOURCE_FILES_ENGINE})

set(SOURCE_FILES__libs__inih
    "../inih/ini.c"
    "../inih/ini.h"
    "../inih/INIReader.cpp"
    "../inih/INIReader.h"
)
source_group("Source Files\\libs\\inih" FILES ${SOURCE_FILES__libs__inih})

set(SOURCE_FILES__libs__findfirst
    "../findfirst/findfirst.c"
    "../findfirst/findfirst.h"
    "../findfirst/spec.c"
    "../findfirst/spec.h"
)
source_group("Source Files\\libs\\findfirst" FILES ${SOURCE_FILES__libs__findfirst})


set(SOURCE_FILES__portability
    "portability/mctypes.h"
    "portability/port_filesystem.cpp"
    "portability/port_filesystem.h"
    "portability/port_inputs.cpp"
    "portability/port_inputs.h"
    "portability/port_outputs.cpp"
    "portability/port_outputs.h"
    "portability/port_sdl_sound.cpp"
    "portability/port_sdl_sound.h"
    "portability/port_sdl_vga_mouse.cpp"
    "portability/port_sdl_vga_mouse.h"
    "portability/port_time.cpp"
    "portability/port_time.h"
    "portability/xmi2mid.cpp"
    "portability/xmi2mid.h"
)
source_group("Source Files\\portability" FILES ${SOURCE_FILES__portability})

set(ALL_FILES
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${SOURCE_FILES_ENGINE}
    ${SOURCE_FILES__libs__inih}
    ${SOURCE_FILES__portability}
)

################################################################################
# targets
################################################################################
add_library(findfirst ${SOURCE_FILES__libs__findfirst})
add_executable(${PROJECT_NAME} ${ALL_FILES})
if (USE_64_BIT)
    set_target_properties(findfirst PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
else ()
    set_target_properties(findfirst PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif ()

install(
    TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug;Release;RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
install(
    FILES 
    ${CMAKE_SOURCE_DIR}/Debug/font/16x16-font.bmp
    DESTINATION bin/font
)
install(
    FILES 
    ${CMAKE_SOURCE_DIR}/config.ini
    DESTINATION bin
)
install(
    DIRECTORY
    ${CMAKE_SOURCE_DIR}/enhancedassets/music-ogg
    ${CMAKE_SOURCE_DIR}/enhancedassets/biggraphics
    DESTINATION bin
)

if (WIN32)
    add_precompiled_header(${PROJECT_NAME} "stdafx.h" "stdafx.cpp")
endif (WIN32)

set(ROOT_NAMESPACE remc2)


################################################################################
# Linux settings
################################################################################
if (UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_SOURCE_DIR}/inih"
        "${CMAKE_SOURCE_DIR}/findfirst"
    )
    #target_compile_definitions(${PROJECT_NAME} PRIVATE
    #)
    #target_compile_options(${PROJECT_NAME} PRIVATE
    #)
    target_link_libraries(${PROJECT_NAME} PUBLIC 
        SDL2::Main
        SDL2::Mixer
        ${PNG_LIBRARY}
        findfirst
        ADLMIDI_static
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
    )
endif ()


################################################################################
# Unit tests
################################################################################
include(GoogleTest)

add_executable(remc2unittests UnitTests.cpp)
target_include_directories(remc2unittests PUBLIC
    "${CMAKE_SOURCE_DIR}/findfirst"
)
target_link_libraries(remc2unittests PUBLIC 
    findfirst
    gtest
    pthread
)
if (USE_64_BIT)
    add_compile_definitions(COMPILE_FOR_64BIT)
    set_target_properties(remc2unittests PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
else ()
    set_target_properties(remc2unittests PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif ()
gtest_add_tests(TARGET      remc2unittests
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)
set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)

# editor
add_subdirectory(editor)


if (UNIX)
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
        target_link_options(remc2 PRIVATE ${SANITIZER_OPTIONS})
        target_link_options(remc2unittests PRIVATE ${SANITIZER_OPTIONS})
        target_link_options(remc2editor PRIVATE ${SANITIZER_OPTIONS})
    endif ()
endif ()