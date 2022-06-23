LOCAL_PATH := $(call my-dir)/../..
include $(CLEAR_VARS)

LOCAL_MODULE     := OPNMIDI

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/src

LOCAL_ARM_MODE   := arm
LOCAL_CPP_FEATURES := exceptions
LOCAL_LDLIBS     := -llog
LOCAL_SRC_FILES := src/opnmidi.cpp src/Ym2612_ChipEmu.cpp \
                   src/opnmidi_load.cpp src/opnmidi_midiplay.cpp \
                   src/opnmidi_opn2.cpp src/opnmidi_private.cpp \
                   src/opnmidi_xmi2mid.c src/opnmidi_mus2mid.c

include $(BUILD_SHARED_LIBRARY)

