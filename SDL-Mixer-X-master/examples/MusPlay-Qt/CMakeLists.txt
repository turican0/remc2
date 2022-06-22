cmake_minimum_required (VERSION 2.8.11)
project(SDLMixerMusPlayer)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(USE_SDL_MIXER_X_PREFIX "" CACHE STRING "Path to SDL Mixer X")
option(USE_OFFICIAL_SDL_MIXER "Compile player in compatibility with official SDL Mixer variant" OFF)

if(USE_OFFICIAL_SDL_MIXER)
    set(SDL_MIXER_X_LIB SDL2_mixer)
else()
    add_definitions(-DUSE_SDL_MIXER_X)
    set(SDL_MIXER_X_LIB SDL2_mixer_ext)
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

if(APPLE)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CARBON_LIBRARY Carbon)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(FORCEFEEDBAK_LIBRARY ForceFeedback)
    find_library(METAL_LIBRARY Metal)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox)
    find_library(AUDIOUNIT_LIBRARY AudioUnit)
    add_definitions(-DQ_OS_MACX) # Workaround for MOC
endif()

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(
    ${Qt5Widgets_INCLUDES}
    ${Qt5Network_INCLUDES}
    ${USE_SDL_MIXER_X_PREFIX}/include)

link_directories(
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Network_LIBRARIES}
    ${USE_SDL_MIXER_X_PREFIX}/lib)

if(APPLE) # To use XCode-built SDL Mixer X
    string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_lower)
    if(CMAKE_BUILD_TYPE_lower STREQUAL "release")
        link_directories(
            ${USE_SDL_MIXER_X_PREFIX}/lib/Release)
    else()
        link_directories(
            ${USE_SDL_MIXER_X_PREFIX}/lib/Debug)
    endif()
endif()

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Network_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

qt5_wrap_ui(UIS_HDRS
    MainWindow/mainwindow.ui
    AssocFiles/assoc_files.ui
)

set(RESOURCE ${CMAKE_SOURCE_DIR}/_resources/musicplayer.qrc)
qt5_add_resources(RESOURCE_ADDED ${RESOURCE})

set(SDLMixer_MusPlay_SRC)

list(APPEND SDLMixer_MusPlay_SRC
    AssocFiles/assoc_files.cpp
    Effects/reverb.cpp
    MainWindow/musplayer_base.cpp
    MainWindow/musplayer_qt.cpp
    Player/mus_player.cpp
    SingleApplication/localserver.cpp
    SingleApplication/pge_application.cpp
    SingleApplication/singleapplication.cpp
    main.cpp
    wave_writer.c
    ${UIS_HDRS}
    ${RESOURCE_ADDED}
)

if(WIN32)
    list(APPEND SDLMixer_MusPlay_SRC _resources/musicplayer.rc)
endif()

add_executable(sdlmixer_musplay
    ${SDLMixer_MusPlay_SRC}
)

set(PGE_MUSPLAY_LINK_LIBS)
list(APPEND PGE_MUSPLAY_LINK_LIBS
    ${SDL_MIXER_X_LIB}
    FLAC
    opusfile
    opus
    vorbisfile
    vorbis
    ogg
    mad
    id3tag
    modplug
    ADLMIDI
    OPNMIDI
    timidity
    gme
    zlib
)

if(WIN32)
    target_link_libraries(sdlmixer_musplay SDL2main version ${PGE_MUSPLAY_LINK_LIBS} SDL2 comctl32)
elseif(APPLE)
    target_link_libraries(sdlmixer_musplay ${PGE_MUSPLAY_LINK_LIBS} SDL2 iconv
        ${COREAUDIO_LIBRARY} ${COREVIDEO_LIBRARY} ${COCOA_LIBRARY} ${CARBON_LIBRARY}
        ${FORCEFEEDBAK_LIBRARY} ${IOKIT_LIBRARY} ${COREFOUNDATION_LIBRARY}
        ${AUDIOTOOLBOX_LIBRARY} ${AUDIOUNIT_LIBRARY}
        ${METAL_LIBRARY}
    )
else()
    target_link_libraries(sdlmixer_musplay ${PGE_MUSPLAY_LINK_LIBS} SDL2)
endif()

qt5_use_modules(sdlmixer_musplay Widgets Network)
