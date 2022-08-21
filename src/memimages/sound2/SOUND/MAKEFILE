MAKETOP = $(CWD)

PROJECT = tech

%include $(MRC)\env.rc

%include $(MRC)\init.rc

LIBTARGS = sound
EXETARGS = sndtest miditest testsnd1

sound_OBJS = mixer qmixer dmixer
sound_win32_OBJS = sndfmt sample lgsndid fade midiplay dsnddynf waildynf timelog qsample dsample

sound_HDRS = lgsound.h lgsndid.h sndfmt.h midiplay.h timelog.h
#sound_DEFINES = SND_TEST

miditest_OBJS = miditest
miditest_LIBS = sound dsound winmm wail32

sndtest_OBJS = sndtest
sndtest_LIBS = sound winmm
sndtest_COMPILER_LIBS = uuid
sndtest_RC = sndtest

#GSG_LIBS    = $(GSG_COMMON_LIBS) cpptools comtools compapis appcore \
#              templgen lg winshell windisp sound
#LG_LIBS    = 3d adjud dstruct fault fix gfile input lg mprintf range res \
#             rnd timer palette dev2d font g2

testsnd1_OBJS = testsnd1
testsnd1_LIBS = lg mprintf range res \
                cpptools comtools compapis appcore templgen sound dsound wail32
testsnd1_COMPILER_LIBS = uuid winmm

%include $(MRC)\make.rc

