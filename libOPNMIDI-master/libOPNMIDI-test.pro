TEMPLATE=app
CONFIG-=qt
CONFIG+=console

TARGET=opnplay
# DESTDIR=$$PWD/bin/

#INCLUDEPATH += /home/vitaly/_git_repos/PGE-Project/_Libs/_builds/linux/include
#LIBS += -L/home/vitaly/_git_repos/PGE-Project/_Libs/_builds/linux/lib
# LIBS += -Wl,-Bstatic -lSDL2 -Wl,-Bdynamic -lpthread -ldl
LIBS += -lSDL2 -lpthread -ldl

DEFINES += ENABLE_END_SILENCE_SKIPPING
# DEFINES += DEBUG_DUMP_RAW_STREAM
# DEFINES += DEBUG_TRACE_ALL_CHANNELS

QMAKE_CFLAGS    += -std=c90 -pedantic
QMAKE_CXXFLAGS  += -std=c++98 -pedantic

INCLUDEPATH += $$PWD/include $$PWD/src

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
    src/wopn/wopn_file.c \
    utils/midiplay/opnplay.cpp
