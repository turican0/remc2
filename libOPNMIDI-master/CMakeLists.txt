cmake_minimum_required (VERSION 3.2)
project(libOPNMIDI VERSION 1.4.0 LANGUAGES C CXX)

include(GNUInstallDirs)

#===========================================================================================
# Strip garbage
if(APPLE)
    set(LINK_FLAGS_RELEASE  "${LINK_FLAGS_RELEASE} -dead_strip")
elseif(NOT MSVC AND NOT MSDOS)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -s -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -s -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s")
    set(LINK_FLAGS_RELEASE  "${LINK_FLAGS_RELEASE} -Wl,--gc-sections -Wl,-s")
endif()

# Global optimization flags
if(NOT MSVC AND NOT MSDOS)
# Global optimization flags
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-omit-frame-pointer")
# Turn on all warnings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
# Deny undefined symbols
    if(NOT APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined" )
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined" )
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-undefined" )
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)
if(CMAKE_BUILD_TYPE_LOWER EQUAL "release")
    add_definitions(-DNDEBUG)
ENDIF()

# Disable bogus MSVC warnings
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

# -fPIC thing
IF(NOT WIN32 AND NOT DJGPP AND NOT MSDOS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF()

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

function(set_legacy_standard destTarget)
    if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER 3.2)
        if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
            # Turn on warnings and legacy C/C++ standards to support more compilers
            target_compile_options(${destTarget} PRIVATE
                $<$<COMPILE_LANGUAGE:C>:-Wall -pedantic -std=c90>
                $<$<COMPILE_LANGUAGE:CXX>:-Wall -pedantic -std=gnu++98>
            )
        endif()
    endif()
endfunction()

function(set_visibility_hidden destTarget)
    if(CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang)$" AND NOT DJGPP)
        if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER 3.2)
            target_compile_options(${destTarget} PRIVATE
              $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden> -fvisibility=hidden)
        else()
            target_compile_options(${destTarget} PRIVATE -fvisibility=hidden)
        endif()
        target_compile_definitions(${destTarget} PRIVATE -DLIBOPNMIDI_VISIBILITY)
    endif()
endfunction()
#===========================================================================================

option(libOPNMIDI_STATIC   "Build static library of libOPNMIDI" ON)
option(libOPNMIDI_SHARED   "Build shared library of libOPNMIDI" OFF)

option(WITH_MIDI_SEQUENCER  "Build with embedded MIDI sequencer. Disable this if you want use library in real-time MIDI drivers or plugins.)" ON)
option(WITH_HQ_RESAMPLER    "Build with support for high quality resampling" OFF)
option(WITH_MUS_SUPPORT     "Build with support for DMX MUS files)" ON)
option(WITH_XMI_SUPPORT     "Build with support for AIL XMI files)" ON)
option(USE_MAME_EMULATOR    "Use MAME YM2612 emulator (for most of hardware)" ON)
option(USE_GENS_EMULATOR    "Use GENS 2.10 emulator (fastest, very outdated, inaccurate)" ON)
option(USE_NUKED_EMULATOR   "Use Nuked OPN2 emulator (most accurate, heavy)" ON)
option(USE_GX_EMULATOR      "Use Genesis Plus GX emulator (experimental)" OFF)
# WIP FEATURES
# option(WITH_CPP_EXTRAS      "Build with support for C++ extras (features are can be found in 'adlmidi.hpp' header)" OFF)

option(WITH_MIDIPLAY        "Build also demo MIDI player" OFF)
option(WITH_VLC_PLUGIN      "Build also a plugin for VLC Media Player" OFF)
option(VLC_PLUGIN_NOINSTALL "Don't install VLC plugin into VLC directory" OFF)
option(WITH_DAC_UTIL        "Build also OPN2 DAC testing utility" OFF)

if(CMAKE_VERSION VERSION_EQUAL "3.1" OR CMAKE_VERSION VERSION_GREATER "3.1")
    set(CMAKE_CXX_STANDARD 11)
endif()

set(libOPNMIDI_INSTALLS)

include_directories(${libOPNMIDI_SOURCE_DIR}/include)
include_directories(${libOPNMIDI_SOURCE_DIR}/src/)
link_directories(${libOPNMIDI_BINARY_DIR}/)

set(libOPNMIDI_SOURCES)

list(APPEND libOPNMIDI_SOURCES
    ${libOPNMIDI_SOURCE_DIR}/src/opnmidi.cpp
    ${libOPNMIDI_SOURCE_DIR}/src/opnmidi_load.cpp
    ${libOPNMIDI_SOURCE_DIR}/src/opnmidi_midiplay.cpp
    ${libOPNMIDI_SOURCE_DIR}/src/opnmidi_opn2.cpp
    ${libOPNMIDI_SOURCE_DIR}/src/opnmidi_private.cpp
    ${libOPNMIDI_SOURCE_DIR}/src/wopn/wopn_file.c
)

if(WITH_MIDI_SEQUENCER)
    list(APPEND libOPNMIDI_SOURCES
        ${libOPNMIDI_SOURCE_DIR}/src/opnmidi_sequencer.cpp
    )
    add_definitions(-DENABLE_END_SILENCE_SKIPPING)
endif()

if(NOT WITH_MUS_SUPPORT OR NOT WITH_MIDI_SEQUENCER)
    add_definitions(-DBWMIDI_DISABLE_MUS_SUPPORT)
endif()

if(NOT WITH_XMI_SUPPORT OR NOT WITH_MIDI_SEQUENCER)
    add_definitions(-DBWMIDI_DISABLE_XMI_SUPPORT)
endif()

if(USE_GENS_EMULATOR)
    list(APPEND libOPNMIDI_SOURCES
        ${libOPNMIDI_SOURCE_DIR}/src/chips/gens_opn2.cpp
        ${libOPNMIDI_SOURCE_DIR}/src/chips/gens/Ym2612_Emu.cpp
    )
    set(HAS_EMULATOR TRUE)
else()
    add_definitions(-DOPNMIDI_DISABLE_GENS_EMULATOR)
endif()

if(USE_MAME_EMULATOR)
    list(APPEND libOPNMIDI_SOURCES
        ${libOPNMIDI_SOURCE_DIR}/src/chips/mame_opn2.cpp
        ${libOPNMIDI_SOURCE_DIR}/src/chips/mame/mame_ym2612fm.c
    )
    set(HAS_EMULATOR TRUE)
else()
    add_definitions(-DOPNMIDI_DISABLE_MAME_EMULATOR)
endif()

if(USE_NUKED_EMULATOR)
    list(APPEND libOPNMIDI_SOURCES
        ${libOPNMIDI_SOURCE_DIR}/src/chips/nuked_opn2.cpp
        ${libOPNMIDI_SOURCE_DIR}/src/chips/nuked/ym3438.c
    )
    set(HAS_EMULATOR TRUE)
else()
    add_definitions(-DOPNMIDI_DISABLE_NUKED_EMULATOR)
endif()

if(USE_GX_EMULATOR)
    list(APPEND libOPNMIDI_SOURCES
        ${libOPNMIDI_SOURCE_DIR}/src/chips/gx_opn2.cpp
        ${libOPNMIDI_SOURCE_DIR}/src/chips/gx/gx_ym2612.c
    )
    set(HAS_EMULATOR TRUE)
else()
    add_definitions(-DOPNMIDI_DISABLE_GX_EMULATOR)
endif()

if(NOT HAS_EMULATOR)
    message(FATAL_ERROR "No emulators enabled! You must enable at least one emulator!")
endif()

if(NOT WITH_MIDI_SEQUENCER)
    add_definitions(-DOPNMIDI_DISABLE_MIDI_SEQUENCER)
endif()

# WIP
#if(NOT WITH_CPP_EXTRAS)
#    add_definitions(-DADLMIDI_DISABLE_CPP_EXTRAS)
#endif()

# === Static library ====
if(libOPNMIDI_STATIC OR WITH_VLC_PLUGIN)
    add_library(OPNMIDI_static STATIC ${libOPNMIDI_SOURCES})
    set_target_properties(OPNMIDI_static PROPERTIES OUTPUT_NAME OPNMIDI)
    target_include_directories(OPNMIDI_static PUBLIC ${libOPNMIDI_SOURCE_DIR}/include)
    set_legacy_standard(OPNMIDI_static)
    set_visibility_hidden(OPNMIDI_static)
    list(APPEND libOPNMIDI_INSTALLS OPNMIDI_static)
    if(NOT libOPNMIDI_STATIC)
        set_target_properties(OPNMIDI_static PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()
endif()

# === Shared library ====
if(libOPNMIDI_SHARED)
    add_library(OPNMIDI_shared SHARED ${libOPNMIDI_SOURCES})
    set_target_properties(OPNMIDI_shared PROPERTIES OUTPUT_NAME OPNMIDI)
    target_include_directories(OPNMIDI_shared PUBLIC ${libOPNMIDI_SOURCE_DIR}/include)
    set_legacy_standard(OPNMIDI_shared)
    set_visibility_hidden(OPNMIDI_shared)
    list(APPEND libOPNMIDI_INSTALLS OPNMIDI_shared)
    if(WIN32)
        target_compile_definitions(OPNMIDI_shared PRIVATE "-DOPNMIDI_BUILD_DLL")
        if(CMAKE_COMPILER_IS_GNUCXX)
            set_property(TARGET OPNMIDI_shared APPEND_STRING PROPERTY LINK_FLAGS " -static-libgcc -static-libstdc++")
            set_property(TARGET OPNMIDI_shared APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-Bstatic,--whole-archive -lpthread -Wl,-Bdynamic,--no-whole-archive")
        endif()
    endif()
    set_target_properties(OPNMIDI_shared PROPERTIES SOVERSION "1")
endif()

if(NOT libOPNMIDI_STATIC AND NOT libOPNMIDI_SHARED)
    message(FATAL_ERROR "Both static and shared libOPNMIDI builds are disabled!
You must enable at least one of them!")
endif()

add_library(OPNMIDI_IF INTERFACE)

if(libOPNMIDI_STATIC OR WITH_VLC_PLUGIN)
    add_library(OPNMIDI_IF_STATIC INTERFACE)
    target_link_libraries(OPNMIDI_IF_STATIC INTERFACE OPNMIDI_static)
endif()

if(libOPNMIDI_SHARED)
  target_link_libraries(OPNMIDI_IF INTERFACE OPNMIDI_shared)
else()
  target_link_libraries(OPNMIDI_IF INTERFACE OPNMIDI_static)
endif()

if(WITH_MIDIPLAY OR WITH_DAC_UTIL)
    find_library(SDL2_LIBRARY SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
    message("Found ${SDL2_LIBRARY}")

    set(SDL2_NEEDED_LIBRARIES)
    if(WIN32)
        if(MSVC)
            list(APPEND SDL2_NEEDED_LIBRARIES ${SDL2_LIBRARY})
        else()
            list(APPEND SDL2_NEEDED_LIBRARIES ${SDL2_LIBRARY} pthread)
        endif()
    else()
        list(APPEND SDL2_NEEDED_LIBRARIES ${SDL2_LIBRARY} pthread dl m stdc++)
    endif()

endif()

if(WITH_MIDIPLAY)
    add_executable(opnmidiplay
        ${libOPNMIDI_SOURCE_DIR}/utils/midiplay/opnplay.cpp
        ${libOPNMIDI_SOURCE_DIR}/utils/midiplay/wave_writer.c
        # ${libOPNMIDI_SOURCE_DIR}/utils/midiplay/wave_writer.c # WIP
    )
    target_link_libraries(opnmidiplay OPNMIDI_IF ${SDL2_NEEDED_LIBRARIES})

    if(libOPNMIDI_SHARED)
        add_dependencies(opnmidiplay OPNMIDI_shared)
        # ========= WIP =========
        # set_target_properties(opnmidiplay PROPERTIES COMPILE_FLAGS "-Wl,-rpath='$$ORIGIN/../lib'")
    else()
        if(NOT libOPNMIDI_STATIC)
            message(FATAL_ERROR "libOPNMIDI is required to be built!")
        endif()
        add_dependencies(opnmidiplay OPNMIDI_static)
    endif()

    if(NOT WIN32 AND NOT APPLE)
        install(FILES ${libOPNMIDI_SOURCE_DIR}/fm_banks/xg.wopn DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/opnmidiplay/)
    endif()

    list(APPEND libOPNMIDI_INSTALLS opnmidiplay)
endif()


if(WITH_VLC_PLUGIN)
    include(cmake/FindLIBVLC.cmake)
    message("Found ${LIBVLCCORE_LIBRARY} of version ${LIBVLC_VERSION} with includes ${LIBVLC_INCLUDE_DIR}")

    set(opnmidi_vlc_src)
    list(APPEND opnmidi_vlc_src
        ${libOPNMIDI_SOURCE_DIR}/utils/vlc_codec/libopnmidi.c)

    add_library(opnmidi_vlc_plugin MODULE ${opnmidi_vlc_src})
    set_target_properties(opnmidi_vlc_plugin PROPERTIES OUTPUT_NAME opnmidi_plugin)
    target_compile_options(opnmidi_vlc_plugin PUBLIC
                "-DVLC_MODULE_COPYRIGHT=\"Copyright \(c\) Vitaly Novichkov\""
                "-DVLC_MODULE_LICENSE=\"GPLv3\""
                "-DMODULE_STRING=\"opnmidi\""
                ${VLCPLUGIN_DEFINITIONS})
    target_include_directories(opnmidi_vlc_plugin PRIVATE
            ${LIBVLC_INCLUDE_DIR}/vlc/plugins
            ${LIBVLC_INCLUDE_DIR}/vlc)

    set(OPNMIDIVLC_LIBS OPNMIDI_IF_STATIC ${LIBVLCCORE_LIBRARY} m)

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_COMPILER_IS_GNUCXX)
        set_property(TARGET opnmidi_vlc_plugin APPEND_STRING PROPERTY LINK_FLAGS " -static-libgcc -static-libstdc++")
        set_property(TARGET opnmidi_vlc_plugin APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-Bstatic,--whole-archive -lpthread -Wl,-Bdynamic,--no-whole-archive")
    elseif(NOT WIN32)
        list(APPEND OPNMIDIVLC_LIBS stdc++)
    endif()

    target_link_libraries(opnmidi_vlc_plugin ${OPNMIDIVLC_LIBS})
    # target_link_libraries(opnmidi_vlc_plugin INTERFACE "-export-symbol-regex ^vlc_entry")

    if(NOT VLC_PLUGIN_NOINSTALL)
        install(TARGETS opnmidi_vlc_plugin DESTINATION "${VLCPLUGIN_CODEC_INSTALL_PATH}")
    else()
        install(TARGETS opnmidi_vlc_plugin DESTINATION "${CMAKE_INSTALL_LIBDIR}/vlc-codec/")
    endif()

    add_dependencies(opnmidi_vlc_plugin OPNMIDI_static)

endif()

if(WITH_DAC_UTIL)
    add_executable(dac_test
        ${libOPNMIDI_SOURCE_DIR}/utils/dac_test/dac_test.cpp
        ${libOPNMIDI_SOURCE_DIR}/src/chips/nuked_opn2.cpp
        ${libOPNMIDI_SOURCE_DIR}/src/chips/nuked/ym3438.c
    )
    target_include_directories(dac_test PRIVATE
            ${libOPNMIDI_SOURCE_DIR}/src/chips/
    )
    target_link_libraries(dac_test ${SDL2_NEEDED_LIBRARIES})
endif()

if(WITH_HQ_RESAMPLER)
    find_library(ZITA_RESAMPLER_LIBRARY "zita-resampler" REQUIRED)
    add_definitions(-DOPNMIDI_ENABLE_HQ_RESAMPLER)
    if(libOPNMIDI_SHARED)
        target_link_libraries(OPNMIDI_shared PUBLIC "${ZITA_RESAMPLER_LIBRARY}")
    endif()
    if(libADLMIDI_STATIC OR WITH_VLC_PLUGIN)
        target_link_libraries(OPNMIDI_static PUBLIC "${ZITA_RESAMPLER_LIBRARY}")
    endif()
    if(WITH_DAC_UTIL)
        target_link_libraries(dac_test "${ZITA_RESAMPLER_LIBRARY}")
    endif()
endif()

install(TARGETS ${libOPNMIDI_INSTALLS}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES
        include/opnmidi.h
        #include/opnmidi.hpp # WIP
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

if(NOT ANDROID)
    configure_file(libOPNMIDI.pc.in libOPNMIDI.pc @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libOPNMIDI.pc"
                  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
endif()

option(WITH_UNIT_TESTS   "Enable unit testing" OFF)
if(WITH_UNIT_TESTS)
    enable_testing ()
    add_subdirectory(test)
endif()


message("==== libOPNMIDI options ====")
message("libOPNMIDI_STATIC        = ${libOPNMIDI_STATIC}")
message("libOPNMIDI_SHARED        = ${libOPNMIDI_SHARED}")
message("WITH_UNIT_TESTS          = ${WITH_UNIT_TESTS}")

# message("WITH_CPP_EXTRAS          = ${WITH_CPP_EXTRAS}")
message("WITH_MIDI_SEQUENCER      = ${WITH_MIDI_SEQUENCER}")
message("WITH_HQ_RESAMPLER        = ${WITH_HQ_RESAMPLER}")
message("WITH_MUS_SUPPORT         = ${WITH_MUS_SUPPORT}")
message("WITH_XMI_SUPPORT         = ${WITH_XMI_SUPPORT}")
message("USE_MAME_EMULATOR        = ${USE_MAME_EMULATOR}")
message("USE_GENS_EMULATOR        = ${USE_GENS_EMULATOR}")
message("USE_NUKED_EMULATOR       = ${USE_NUKED_EMULATOR}")
message("USE_GX_EMULATOR          = ${USE_GX_EMULATOR}")

message("===== Utils and extras =====")
message("WITH_MIDIPLAY            = ${WITH_MIDIPLAY}")
message("WITH_VLC_PLUGIN          = ${WITH_VLC_PLUGIN}")
message("WITH_DAC_UTIL            = ${WITH_DAC_UTIL}")
