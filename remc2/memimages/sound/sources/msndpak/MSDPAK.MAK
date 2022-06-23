
multisnd.com:   msnd.asm msnd_dsp.equ msnd.inc
                tasm -zi /m3 /dMSND=1 msnd
                tlink /v msnd
                tdstrip -s msnd
                exe2bin msnd.exe multisnd.com
                erase msnd.exe

