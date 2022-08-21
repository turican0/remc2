
#
# MultiSound Proteus interface XMIDI driver
#

multisnd.adv: xmidi.asm multisnd.inc ail.inc ail.mac msmpak.mak multisnd.equ
   tasm /zi /m /w+ /ml /dMULTISND xmidi.asm;
   tlink /c /x xmidi;
   tlink /v /c xmidi;
   tdstrip -s xmidi

   tlink /c /x xmidi;

   exe2bin xmidi.exe multisnd.adv
   del xmidi.exe

