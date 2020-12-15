TEMPLATE = lib
CONFIG -= app_bundle
CONFIG -= qt
static-sdlmixer:{
    CONFIG -= dll
    CONFIG += static
} else {
    CONFIG += dll
    CONFIG -= static
}

exists($$PWD/../../_common/lib_destdir.pri):{
    # As part of PGE Project
    include($$PWD/../../_common/lib_destdir.pri)
} else {
    DESTDIR=$$PWD/build/lib/
}
include($$PWD/cflags.pri)

static-sdlmixer:{
    TARGET = SDL_Mixer_Xstatic
} else {
    TARGET = SDL2_mixer_ext
    win32:enable-stdcalls:{ #Required for building a VB6-compatible version
        TARGET = SDL2MixerVB
    }
}

# Dependent audio codecs
include($$PWD/../AudioCodecs/audio_codec_includes.pri)

# Codecs
DEFAULT_CODECS += \
    MUSIC_WAV \
    MUSIC_MID_TIMIDITY \
    #MUSIC_MID_FLUIDSYNTH \
    #USE_FLUIDLIGHT \
    MUSIC_MID_ADLMIDI \
    MUSIC_MID_OPNMIDI \
    MUSIC_OGG \
    MUSIC_FLAC \
    #MUSIC_MP3_MPG123 \
    MUSIC_MP3_ID3TAG\
    MUSIC_MP3_MAD \
    MUSIC_MP3_MAD_GPL_DITHERING \
    MUSIC_GME \
    #MUSIC_MOD_MIKMOD \
    MUSIC_MOD_MODPLUG \
    #CMD_MUSIC

COPY=cp
win32:{
    DEFAULT_CODECS += MUSIC_MID_NATIVE
    enable-stdcalls:{ #Useful for VB6 usage
        TARGET = SDL2MixerVB
        QMAKE_LFLAGS += -Wl,--add-stdcall-alias
        DEFINES += FORCE_STDCALLS
    }
    RC_FILE = version.rc
    COPY=copy
}
macx:{
    DEFAULT_CODECS += MUSIC_MID_NATIVE
}

INCLUDEPATH += $$PWD/include/SDL_mixer_ext
INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/src/
INCLUDEPATH += $$PWD/src/codecs

# Build properties
exists($$PWD/../../_common/build_props.pri):{
    # As part of PGE Project
    include($$PWD/../../_common/build_props.pri)
    DESTDIR = $$PWD/../_builds/$$TARGETOS/lib
    LIBS += -L$$PWD/../_builds/$$TARGETOS/lib
    INCLUDEPATH += $$PWD/../_builds/$$TARGETOS/include
    INCLUDEPATH += $$PWD/../_builds/$$TARGETOS/include/SDL2
    QMAKE_POST_LINK = \
        $$COPY $$shell_path($$PWD/include/SDL_mixer_ext/SDL_mixer_ext.h) \
        $$shell_path($$PWD/../_builds/$$TARGETOS/include/SDL2)

    SDL2MixerH.path =  $$PWD/../_builds/$$TARGETOS/include/SDL2
    SDL2MixerH.files += $$PWD/include/SDL_mixer_ext/SDL_mixer_ext.h
    INSTALLS += SDL2MixerH

    win32: {
        SDL2MixerSO.path = $$PWD/../_builds/win32/bin
        SDL2MixerSO.files += $$PWD/../_builds/win32/lib/*.dll
        INSTALLS += SDL2MixerSO
    }
} else {
    # As standalone library
    exists($$PWD/../AudioCodecs/build/lib/):{
        LIBS += -L$$PWD/../AudioCodecs/build/lib/
        INCLUDEPATH += $$PWD/../AudioCodecs/build/include/SDL2/
    }
    LIBS += -L$$PWD/build/lib/
    INCLUDEPATH += -L$$PWD/build/include/
    include($$PWD/_common/build_props.pri)
}

linux-g++||unix:!macx:!android:{
    DEFINES += HAVE_INTTYPES_H HAVE_SETENV HAVE_SINF HAVE_FORK
    CONFIG -= dll
    CONFIG -= static
    CONFIG += static_and_shared
    CONFIG += unversioned_libname
    CONFIG += skip_target_version_ext
}
android:{
    DEFINES += HAVE_INTTYPES_H HAVE_SETENV HAVE_SINF
}
macx:{
    DEFINES += HAVE_INTTYPES_H HAVE_SETENV HAVE_SINF
    # Build as static library
    CONFIG -= dll
    CONFIG += static
    CONFIG += staticlib
}

android:{
    LIBS += -lvorbisfile -lvorbis -logg #-lvorbisidec
} else {
    win32:{
        DEFINES -= UNICODE _UNICODE
        enable-stdcalls: {
            LIBS += -static -l:libSDL2.a -static-libgcc -static-libstdc++ -static -lpthread -luuid
            include($$PWD/src/vb6_binding/vb6_binding.pri)
        } else {
            !win*-msvc*:{
                LIBS += -lSDL2main -lSDL2.dll
            } else {
                LIBS += -lSDL2main -lSDL2
            }
        }
        LIBS += -lwinmm -lole32 -limm32 -lversion -loleaut32 -luser32 -lgdi32
    } else {
        LIBS += -lSDL2
    }
}

!win*-msvc*:{
    LIBS += -lm
}


# Let user disable or enable component by adding extra CONFIG+= values
enable-flac:  DEFAULT_CODECS += MUSIC_FLAC
disable-flac: DEFAULT_CODECS -= MUSIC_FLAC

enable-midi:  DEFAULT_CODECS += MUSIC_MIDI
disable-midi: DEFAULT_CODECS -= MUSIC_MIDI

enable-midi-adlmidi:  DEFAULT_CODECS += MUSIC_MID_ADLMIDI
disable-midi-adlmidi: DEFAULT_CODECS -= MUSIC_MID_ADLMIDI

enable-midi-timidity:  DEFAULT_CODECS += MUSIC_MID_TIMIDITY
disable-midi-timidity: DEFAULT_CODECS -= MUSIC_MID_TIMIDITY

enable-midi-native:  DEFAULT_CODECS += MUSIC_MID_NATIVE
disable-midi-native: DEFAULT_CODECS -= MUSIC_MID_NATIVE

enable-midi-opnmidi:  DEFAULT_CODECS += MUSIC_MID_OPNMIDI
disable-midi-opnmidi: DEFAULT_CODECS -= MUSIC_MID_OPNMIDI

enable-midi-fluidsynth:  DEFAULT_CODECS += MUSIC_MID_FLUIDSYNTH
disable-midi-fluidsynth: DEFAULT_CODECS -= MUSIC_MID_FLUIDSYNTH

enable-id3tag:  DEFAULT_CODECS += MUSIC_MP3_ID3TAG
disable-id3tag: DEFAULT_CODECS -= MUSIC_MP3_ID3TAG

enable-mp3:  DEFAULT_CODECS += MUSIC_MP3_MAD
disable-mp3: DEFAULT_CODECS -= MUSIC_MP3_MAD

enable-mp3-mad:  DEFAULT_CODECS += MUSIC_MP3_MAD
disable-mp3-mad: DEFAULT_CODECS -= MUSIC_MP3_MAD

enable-mp3-mpg123:  DEFAULT_CODECS += MUSIC_MP3_MPG123
disable-mp3-mpg123: DEFAULT_CODECS -= MUSIC_MP3_MPG123

enable-ogg:  DEFAULT_CODECS += MUSIC_OGG
disable-ogg: DEFAULT_CODECS -= MUSIC_OGG

enable-gme:  DEFAULT_CODECS += MUSIC_GME
disable-gme: DEFAULT_CODECS -= MUSIC_GME

enable-mikmod:  DEFAULT_CODECS += MUSIC_MOD_MIKMOD
disable-mikmod: DEFAULT_CODECS -= MUSIC_MOD_MIKMOD

enable-modplug:  DEFAULT_CODECS += MUSIC_MOD_MODPLUG
disable-modplug: DEFAULT_CODECS -= MUSIC_MOD_MODPLUG

enable-cmd:  DEFAULT_CODECS += MUSIC_CMD
disable-cmd: DEFAULT_CODECS -= MUSIC_CMD

android: {
DEFINES += \
    HAVE_STRCASECMP \
    HAVE_STRNCASECMP \
    MUSIC_MID_ADLMIDI \
    MUSIC_MID_OPNMIDI \
    MUSIC_CMD \
    NO_OLDNAMES \
    SPC_MORE_ACCURACY
    #OGG_USE_TREMOR
    DEFAULT_CODECS -= FLAC_MUSIC #temopary with no FLAC, because I wasn't built it because compilation bug
}

DEFINES += \
    $$DEFAULT_CODECS \
    main=SDL_main \
    PIC \
    _REENTRANT \
    _USE_MATH_DEFINES \

include($$PWD/src/codecs/play_wave.pri)
contains(DEFINES, MUSIC_MID_ADLMIDI):   include($$PWD/src/codecs/play_midi_adl.pri)
contains(DEFINES, MUSIC_MID_OPNMIDI):   include($$PWD/src/codecs/play_midi_opn.pri)
contains(DEFINES, MUSIC_MID_TIMIDITY):  include($$PWD/src/codecs/play_midi_timidity.pri)
contains(DEFINES, MUSIC_MID_NATIVE):    include($$PWD/src/codecs/play_midi_native.pri)
contains(DEFINES, MUSIC_MID_FLUIDSYNTH):include($$PWD/src/codecs/play_midi_fluid.pri)
contains(DEFINES, MUSIC_OGG):           include($$PWD/src/codecs/play_ogg.pri)
contains(DEFINES, MUSIC_FLAC):          include($$PWD/src/codecs/play_flac.pri)
contains(DEFINES, MUSIC_MOD_MIKMOD):    include($$PWD/src/codecs/play_mikmod.pri)
contains(DEFINES, MUSIC_MOD_MODPLUG):   include($$PWD/src/codecs/play_modplug.pri)
contains(DEFINES, MUSIC_MP3_ID3TAG):    include($$PWD/src/codecs/play_id3tag.pri)
contains(DEFINES, MUSIC_MP3_MAD):       include($$PWD/src/codecs/play_mad.pri)
contains(DEFINES, MUSIC_MP3_MPG123):    include($$PWD/src/codecs/play_mpg123.pri)
contains(DEFINES, MUSIC_GME):           include($$PWD/src/codecs/play_gme.pri)
contains(DEFINES, MUSIC_CMD):           include($$PWD/src/codecs/play_cmdmusic.pri)

HEADERS += \
    include/SDL_mixer_ext/SDL_mixer_ext.h \
    include/SDL_mixer_ext/begin_code.h \
    include/SDL_mixer_ext/close_code.h \
    src/effects_internal.h \
    src/mixer.h \

SOURCES += \
    src/effect_position.c \
    src/effect_stereoreverse.c \
    src/effects_internal.c \
    src/mixer.c \
    src/music.c \
    src/mixer_x_deprecated.c \

