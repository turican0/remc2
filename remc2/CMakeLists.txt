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
    "utilities/Maths.cpp"
    "utilities/Maths.h"
    "engine/engine_support_converts.cpp"
    "engine/engine_support_converts.h"
    "engine/Type_D93C0_Bldgprmbuffer.cpp"
    "engine/Type_D93C0_Bldgprmbuffer.h"
    "engine/Type_D94F0_Bldgprmbuffer.cpp"
    "engine/Type_D94F0_Bldgprmbuffer.h"
    "engine/Type_MapScreenPortals_E17CC.cpp"
    "engine/Type_MapScreenPortals_E17CC.h"
    "engine/Type_SecretMapScreenPortals_E2970.cpp"
    "engine/Type_SecretMapScreenPortals_E2970.h"
    "engine/Type_D404C.cpp"
    "engine/Type_D404C.h"
    "engine/Type_WORD_D951C.cpp"
    "engine/Type_WORD_D951C.h"
    "engine/XUnk_D4350.cpp"
    "engine/XUnk_D4350.h"
    "engine/Unk_D4A30.cpp"
    "engine/Unk_D4A30.h"
    "engine/Unk_D47E0.cpp"
    "engine/Unk_D47E0.h"
    "engine/RenderThread.cpp"
    "engine/RenderThread.h"
    "utilities/BitmapIO.cpp"
    "utilities/BitmapIO.h"
    "utilities/DataFileIO.cpp"
    "utilities/DataFileIO.h"
    "engine/read_config.cpp"
    "engine/read_config.h"
    "engine/TextureMaps.cpp"
    "engine/TextureMaps.h"
    "engine/GameRenderOriginal.cpp"
    "engine/GameRenderOriginal.h"
    "engine/GameRenderHD.cpp"
    "engine/GameRenderHD.h"
    "engine/GameRenderGL.cpp"
    "engine/GameRenderGL.h"
    "engine/GameRenderInterface.h"
    "sub_main.cpp"
    "sub_main.h"
    "engine/ProjectionPolygon.cpp"
    "engine/ProjectionPolygon.h"
    "engine/Sound.cpp"
    "engine/Sound.h"
    "engine/Network.cpp"
    "engine/Network.h"
    "engine/Basic.cpp"
    "engine/Basic.h"
    "engine/Animation.cpp"
    "engine/Animation.h"
    "engine/Terrain.cpp"
    "engine/Terrain.h"
    "engine/sub_main_mouse.h"
    "engine/ViewPort.cpp"
    "engine/ViewPort.h"
    "engine/CommandLineParser.cpp"
    "engine/CommandLineParser.h"
    "engine/MenusAndIntros.cpp"
    "engine/MenusAndIntros.h"
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

set(SOURCE_FILES__libs__spdlog
    "../spdlog/include/spdlog/spdlog.h"
    "../spdlog/include/spdlog/sinks/rotating_file_sink.h"
    "../spdlog/include/spdlog/sinks/stdout_color_sinks.h"
)
source_group("Source Files\\libs\\spdlog" FILES ${SOURCE_FILES__libs__spdlog})

set(SOURCE_FILES__portability
    "portability/mctypes.h"
    "portability/fcaseopen.cpp"
    "portability/fcaseopen.h"
    "portability/port_net.cpp"
    "portability/port_net.h"
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
    ${SOURCE_FILES__networklib}
)

################################################################################
# targets
################################################################################
add_library(findfirst ${SOURCE_FILES__libs__findfirst})
add_library(spdlog ${SOURCE_FILES__libs__spdlog})
add_executable(${PROJECT_NAME} ${ALL_FILES})

install(
    TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug;Release;RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
install(
    FILES 
    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/font/16x16-font.bmp
    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/font/8x8-font.bmp
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
        "${CMAKE_SOURCE_DIR}/spdlog/include"
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
        Boost::system
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
    )
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:DEBUG>:_DEBUG>"
    )
endif ()


# editor
if (EDITOR)
    add_subdirectory(editor)
endif ()


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
        if (EDITOR)
            target_link_options(remc2editor PRIVATE ${SANITIZER_OPTIONS})
        endif ()
    endif ()
endif ()