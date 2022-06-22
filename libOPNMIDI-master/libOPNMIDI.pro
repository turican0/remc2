#
#  Project file for the Qt Creator IDE
#

TEMPLATE = lib
CONFIG  -= qt
CONFIG  += staticlib

TARGET = OPNMIDI
INSTALLINCLUDES = $$PWD/include/*
INSTALLINCLUDESTO = OPNMIDI
include($$PWD/../audio_codec_common.pri)

DEFINES += ENABLE_END_SILENCE_SKIPPING

macx: QMAKE_CXXFLAGS_WARN_ON += -Wno-absolute-value

INCLUDEPATH += $$PWD $$PWD/include

HEADERS += \
    include/opnmidi.h \
    src/chips/gens_opn2.h \
    src/chips/gens/Ym2612_Emu.h \
    src/chips/mame/mamedef.h \
    src/chips/mame/mame_ym2612fm.h \
    src/chips/mame_opn2.h \
    src/chips/nuked_opn2.h \
    src/chips/nuked/ym3438.h \
    src/chips/gx_opn2.h \
    src/chips/gx/gx_ym2612.h \
    src/chips/opn_chip_base.h \
    src/chips/opn_chip_base.tcc \
    src/cvt_mus2mid.hpp \
    src/cvt_xmi2mid.hpp \
    src/midi_sequencer.h \
    src/midi_sequencer.hpp \
    src/midi_sequencer_impl.hpp \
    src/fraction.hpp \
    src/opnbank.h \
    src/opnmidi_private.hpp \
    src/wopn/wopn_file.h

SOURCES += \
    src/chips/gens_opn2.cpp \
    src/chips/gens/Ym2612_Emu.cpp \
    src/chips/gx/gx_ym2612.c \
    src/chips/gx_opn2.cpp \
    src/chips/mame/mame_ym2612fm.c \
    src/chips/mame_opn2.cpp \
    src/chips/nuked_opn2.cpp \
    src/chips/nuked/ym3438.c \
    src/opnmidi.cpp \
    src/opnmidi_load.cpp \
    src/opnmidi_midiplay.cpp \
    src/opnmidi_opn2.cpp \
    src/opnmidi_private.cpp \
    src/opnmidi_sequencer.cpp \
    src/wopn/wopn_file.c
