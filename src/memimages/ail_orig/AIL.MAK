###############################################################
#                                                             #
#  MAKEFILE for AIL development                               #             
#  13-Mar-91 John Miles                                       #
#                                                             #
#  Execute with Microsoft (or compatible) MAKE                #
#                                                             #
###############################################################

##########################################################################
#                                                                        #
# TASM: Case sensitivity on, enable multiple passes to optimize forward  #
#       references/jumps, generate debug info, enable all warnings       #
#                                                                        #
#   TC: Compile only, generate debug info, large memory model            #
#                                                                        #
##########################################################################

#
# Application Program Interface (API) module
#

ail.obj: ail.asm ail.inc ail.mac
   tasm /m /w+ /ml ail.asm;

#
# MT-32 family with Roland MPU-401-compatible interface XMIDI driver
#

mt32mpu.adv: xmidi.asm mt32.inc mpu401.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dMT32 /dMPU401 xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe mt32mpu.adv
   del xmidi.exe

#
# Standard Ad Lib XMIDI driver
#

adlib.adv: xmidi.asm yamaha.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dADLIBSTD xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe adlib.adv
   del xmidi.exe

#
# Standard Sound Blaster XMIDI driver
#

sbfm.adv: xmidi.asm yamaha.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dSBSTD xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe sbfm.adv
   del xmidi.exe

#
# Sound Blaster Pro I XMIDI driver
#

sbp1fm.adv: xmidi.asm yamaha.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dSBPRO1 xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe sbp1fm.adv
   del xmidi.exe

#
# Sound Blaster Pro II XMIDI driver
#

sbp2fm.adv: xmidi.asm yamaha.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dSBPRO2 xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe sbp2fm.adv
   del xmidi.exe

#
# Pro Audio Spectrum (original YM3812 8-bit card) XMIDI driver
#

pasfm.adv: xmidi.asm yamaha.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dPAS xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe pasfm.adv
   del xmidi.exe

#
# Pro Audio Spectrum Plus/16 XMIDI driver
#

pasopl.adv: xmidi.asm yamaha.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dPASOPL xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe pasopl.adv
   del xmidi.exe

#
# Ad Lib Gold XMIDI driver
#

adlibg.adv: xmidi.asm yamaha.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dADLIBG xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe adlibg.adv
   del xmidi.exe

#
# Tandy 3-voice XMIDI driver
#

tandy.adv: xmidi.asm spkr.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dTANDY xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe tandy.adv
   del xmidi.exe

#
# IBM internal speaker XMIDI driver
#

pcspkr.adv: xmidi.asm spkr.inc ail.inc ail.mac
   tasm /zi /m /w+ /ml /dIBMPC xmidi.asm;
   tlink /c /x xmidi;
   exe2bin xmidi.exe pcspkr.adv
   del xmidi.exe

#
# Pro Audio Spectrum 8-bit digital sound driver
#

pasdig.adv: dmasound.asm ail.inc ail.mac
   tasm /zi /m /w+ /ml /dPAS dmasound.asm;
   tlink /c /x dmasound;
   exe2bin dmasound.exe pasdig.adv
   del dmasound.exe

#
# Ad Lib Gold digital sound driver
#

algdig.adv: dmasound.asm ail.inc ail.mac
   tasm /zi /m /w+ /ml /dADLIBG dmasound.asm;
   tlink /c /x dmasound;
   exe2bin dmasound.exe algdig.adv
   del dmasound.exe

#
# Sound Blaster digital sound driver
#

sbdig.adv: dmasound.asm ail.inc ail.mac
   tasm /zi /m /w+ /ml /dSBSTD dmasound.asm;
   tlink /c /x dmasound;
   exe2bin dmasound.exe sbdig.adv
   del dmasound.exe

#
# Sound Blaster Pro digital sound driver
#

sbpdig.adv: dmasound.asm ail.inc ail.mac
   tasm /zi /m /w+ /ml /dSBPRO dmasound.asm;
   tlink /c /x dmasound;
   exe2bin dmasound.exe sbpdig.adv
   del dmasound.exe

#
# Cakewalk interface to XMIDI drivers                                   
#

cakeport.drv: cakeport.asm ail.inc ail.mac
   tasm /zi /m /w+ /ml cakeport.asm;
   tlink /c /x cakeport;
   exe2bin cakeport.exe cakeport.drv
   del cakeport.exe
   copy cakeport.drv midiport.drv

#
# Assorted tools and sample programs
#

midiecho.exe: midiecho.c ail.h gen.h gen.lib ail.obj
   bcc -ml -c -v midiecho.c
   tlink @midiecho.lls

midiform.exe: midiform.c gen.h gen.lib
   bcc -ml -c -v midiform.c
   tlink @midiform.lls

midilog.exe: midilog.c gen.h gen.lib
   bcc -ml -c -v midilog.c
   tlink @midilog.lls

midirec.exe: midirec.c gen.h ail.h gen.lib ail.obj
   bcc -ml -c -v midirec.c
   tlink @midirec.lls

mlist.exe: mlist.c gen.h
   bcc -ml -c -v mlist.c
   tlink @mlist.lls

clad.exe: clad.c gen.h gen.lib
   bcc -ml -c -v clad.c
   tlink @clad.lls

glib.exe: glib.c gen.h gen.lib
   bcc -ml -c -v glib.c
   tlink @glib.lls

vtor.exe: vtor.c gen.h gen.lib
   bcc -ml -c -v vtor.c
   tlink @vtor.lls

mixdemo.exe: mixdemo.c gen.h ail.h gen.lib ail.obj
   bcc -ml -c -v mixdemo.c
   tlink @mixdemo.lls

xplay.exe: xplay.c gen.h ail.h gen.lib ail.obj
   bcc -ml -c -v xplay.c
   tlink @xplay.lls

digiplay.exe: digiplay.c gen.h ail.h gen.lib ail.obj
   bcc -ml -c -v digiplay.c
   tlink @digiplay.lls

stplay.exe: stplay.c gen.h ail.h gen.lib ail.obj
   bcc -ml -c -v stplay.c
   tlink @stplay.lls

vocplay.exe: vocplay.c gen.h ail.h gen.lib ail.obj
   bcc -ml -c -v vocplay.c
   tlink @vocplay.lls

soundfx.obj: soundfx.c soundfx.h ail.h
   bcc -ml -c -v soundfx.c

blaster.exe: blaster.c ail.h ail.obj
   bcc -ml -c -v blaster.c
   tlink @blaster.lls

tail.obj: tail.c
   bcc -ml -c -v tail.c

#
# Sample XMIDI sequence builds
#

demo.xmi: backgnd.mid shanty.mid choral.mid midiform.exe
  midiform demo.xmi backgnd.mid shanty.mid choral.mid

spkrdemo.xmi: spkrdemo.mid midiform.exe
  midiform spkrdemo.xmi spkrdemo.mid

#
# GTL maintenance
#

sample.ad: sample.cat sample.bnk glib.exe
   glib sample.cat
   copy sample.ad sample.opl
