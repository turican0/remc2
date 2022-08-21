head	 1.11;
branch   ;
access   ;
symbols  ;
locks    ; strict;
comment  @# @;


1.11
date	 1998.03.20.13.01.29;  author PATMAC;  state Exp;
branches ;
next	 1.10;

1.10
date	 1996.10.23.12.03.31;  author TOML;  state Exp;
branches ;
next	 1.9;

1.9
date	 1996.10.03.18.28.42;  author PATMAC;  state Exp;
branches ;
next	 1.8;

1.8
date	 1996.08.16.17.11.02;  author PATMAC;  state Exp;
branches ;
next	 1.7;

1.7
date	 1996.07.30.19.03.09;  author PATMAC;  state Exp;
branches ;
next	 1.6;

1.6
date	 1996.07.30.17.36.46;  author PATMAC;  state Exp;
branches ;
next	 1.5;

1.5
date	 1996.07.24.22.40.29;  author PATMAC;  state Exp;
branches ;
next	 1.4;

1.4
date	 1996.07.24.19.04.00;  author PATMAC;  state Exp;
branches ;
next	 1.3;

1.3
date	 1996.06.17.13.15.27;  author PATMAC;  state Exp;
branches ;
next	 1.2;

1.2
date	 1996.04.18.16.10.35;  author JOHNB;  state Exp;
branches ;
next	 1.1;

1.1
date	 1996.04.11.16.47.40;  author JOHNB;  state Exp;
branches ;
next	 ;


desc
@new sound lib
@


1.11
log
@Add dsample, dmixer, qsample. qmixer modules
@
text
@MAKETOP = $(CWD)

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

@


1.10
log
@Made DOS buildabl
@
text
@d12 2
a13 2
sound_OBJS = mixer
sound_win32_OBJS = sndfmt sample lgsndid fade midiplay dsnddynf waildynf timelog
@


1.9
log
@Add timelog to module list
@
text
@d12 2
a13 1
sound_OBJS = sndfmt sample mixer lgsndid fade midiplay dsnddynf waildynf timelog
@


1.8
log
@Add sound test app
@
text
@d12 1
a12 2
#sound_OBJS = sndfmt sample mixer lgsndid fade midiplay dsnddynf waildynf timelog
sound_OBJS = sndfmt sample mixer lgsndid fade midiplay dsnddynf waildynf
@


1.7
log
@Remove dsound & wail32 from lib list
@
text
@d10 1
a10 1
EXETARGS = sndtest miditest
d25 10
@


1.6
log
@Added dsnddynf & waildynf modules
@
text
@d22 1
a22 1
sndtest_LIBS = sound dsound winmm wail32
@


1.5
log
@Don't include timelog in normal build of library
@
text
@d12 2
a13 2
#sound_OBJS = sndfmt sample mixer lgsndid fade midiplay timelog
sound_OBJS = sndfmt sample mixer lgsndid fade midiplay
@


1.4
log
@Add WAIL, midiplay obj & midiplay header
@
text
@d12 2
a13 1
sound_OBJS = sndfmt sample mixer lgsndid fade midiplay timelog
@


1.3
log
@added sndfmt.h to sound_HDRS
@
text
@d10 1
a10 1
EXETARGS = sndtest
d12 1
a12 1
sound_OBJS = sndfmt sample mixer lgsndid fade
d14 1
a14 1
sound_HDRS = lgsound.h lgsndid.h sndfmt.h
d17 3
d21 1
a21 1
sndtest_LIBS = sound dsound winmm
@


1.2
log
@added fade.cpp
@
text
@d14 1
a14 1
sound_HDRS = lgsound.h lgsndid.h
@


1.1
log
@Initial revision
@
text
@d10 1
d12 1
a12 1
sound_OBJS = sndfmt sample mixer lgsndid
d15 6
@
