AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libopnmidi_plugin.la

CFLAGS = -DVLC_MODULE_COPYRIGHT="\"Copyright \(c\) Vitaly Novichkov\""
CFLAGS += -DVLC_MODULE_LICENSE=\"GPLv3\"

libopnmidi_plugin_la_SOURCES = libopnmidi.c
libopnmidi_plugin_la_CFLAGS = -Wall -DMODULE_STRING=\"opnmidi\" $(vlc_CFLAGS)
libopnmidi_plugin_la_CPPFLAGS = -DMODULE_STRING=\"opnmidi\" $(vlc_CFLAGS)
libopnmidi_plugin_la_LIBADD = $(vlc_LIBS)
libopnmidi_plugin_la_LDFLAGS = -avoid-version -module -export-symbol-regex ^vlc_entry -lOPNMIDI -lm -lstdc++ $(vlc_LDFLAGS)

