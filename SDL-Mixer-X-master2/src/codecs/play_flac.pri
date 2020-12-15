DEFINES += FLAC__NO_DLL

LIBS += -lFLAC -logg

HEADERS += \
    $$PWD/music_flac.h

SOURCES += \
    $$PWD/music_flac.c

