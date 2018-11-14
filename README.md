# remc2
# Reverse engineering of game Magic Carpet 2 from assembler to c/c++ language<br />
## STATUS: unplayable, in development<br />
<br />
To start the game, it needs data from the original distribution, which you can download at the minimum fee here:<br />
https://www.gog.com/game/magic_carpet_2_the_netherworlds<br />
(or from any abbadone archive, if it is legal in your country)<br />
<br />
simple dev blog:https://github.com/turican0/remc2/wiki<br />
## 0.17:<br />
-fixed Load, Save, Restart and Exit button in Map Menu<br />
-add czech translation<br />
-fix load level initialize<br />
-begin of fixing game draw pipeline<br />
<br />
## 0.16:<br />
-map menu graphics full fixed!<br />
-added config<br />
-added alternative music<br />
<br />
## 0.15:<br />
-BIG work on SOUND and MUSIC<br />
--rewrite mss and connect it to SDL<br />
(full function WAV or MIDI music - at now native MIDI, I test timiditi or fluidsynth, I maybe add OPL syntesis later)<br />
-add SDL2 support<br />
<br />
## 0.14:<br />
-can open load menu<br />
-can open save menu<br />
-fixed setting keys<br />
-fixed setting keys menu<br />
-can open game progress menu<br />
<br />
## 0.13:<br />
-fixed language menu<br />
-fixed change name menu<br />
-added basic get key routine<br />
-can exit from game!<br />
<br />
## 0.12:<br />
-main menu is fixed<br />
<br />
## 0.11:<br />
-fixed text rendering<br />
-can play all intro<br />
<br />
## 0.10:<br />
-fixed load and save language config<br />
-can play part of intro<br />
<br />
## 0.09:<br />
-fix cursor draw subroutine<br />
-can select language<br />
<br />
## 0.08:<br />
-added mouse cursor!<br />
<br />
## 0.07:<br />
-render of language select screen is same as original!<br />
<br />
## 0.06:<br />
-some sprites works<br />
<br />
## 0.05:<br />
-add primitive console mode<br />
-repairing fade pallete<br />
<br />
## 0.04:<br />
-fixed pallette<br />
<br />
## 0.03:<br />
-fixed resolution<br />
<br />
## 0.02 - first graphics:<br />
-show first graphics<br />
<br />
## 0.01 - first release:<br />
-can compile under Windows/Visual Studio<br />
-repair some structures<br />
-replace rnc utilites<br />

For develop I use modified version of Dosbox-x:
https://github.com/turican0/dosbox-x-remc2
